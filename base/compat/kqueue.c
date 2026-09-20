/* FreeLinX/ports - base/compat/kqueue.c : kqueue(2)/kevent(2) on Linux.
 *
 * musl has no kqueue/kevent; NetBSD 10 usr.sbin/inetd is kqueue-core (the
 * select-based fallback was removed).  This is a faithful emulation of just
 * the kqueue surface inetd exercises, layered on epoll(7):
 *
 *   - kqueue()      : epoll_create1(EPOLL_CLOEXEC)
 *   - kevent(...)   : apply EV_ADD/EV_DELETE/EV_ENABLE/EV_DISABLE for
 *                     EVFILT_READ/EVFILT_WRITE via epoll_ctl, maintain a
 *                     per-fd registry of registered filters + udata pointers,
 *                     then epoll_wait and synthesize struct kevents.
 *   - EVFILT_SIGNAL : signalfd(2).  Signals registered with EVFILT_SIGNAL
 *                     are blocked (sigprocmask) and given a no-op handler so
 *                     they pend; the signalfd is polled with the other fds
 *                     and each delivered signal becomes an EVFILT_SIGNAL
 *                     kevent whose ident is the signal number.
 *
 * Unsupported filters (EVFILT_TIMER, EVFILT_PROC, EVFILT_VNODE, EVFILT_USER,
 * EVFILT_AIO, EVFILT_FS) fail with EV_ERROR/EOPNOTSUPP in the receipt path
 * or -1/errno otherwise.  Supported but best-effort: EV_ONESHOT (matched,
 * then the filter is left enabled - inetd never uses it), EV_CLEAR (epoll is
 * level-triggered, which already repeats ready state like BSD EV_CLEAR),
 * EV_RECEIPT (implemented).  This is a shim, not a complete BSD kqueue ABI;
 * ports using other filter types must not rely on exact emulation.
 */
#include <sys/event.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct flx_kqent {
	int		 fd;
	int		 filters;	/* bit 0 = EVFILT_READ, bit 1 = EVFILT_WRITE */
	void		*udata_read;
	void		*udata_write;
	struct flx_kqent *next;
};

/* EVFILT_PROC registrations (pwait): pid + next.  Each is delivered at most
 * once (mimicking NetBSD, where NOTE_EXIT destroys the process knote). */
struct flx_kqproc {
	pid_t		 pid;
	struct flx_kqproc *next;
};

static struct flx_kqent *flx_kqents;
static struct flx_kqproc *flx_kqprocs;
static int	 flx_signalfd = -1;
static sigset_t	 flx_sigmask;
static int	 flx_kq = -1;
static struct timespec flx_deadline;	/* absolute CLOCK_MONOTONIC deadline */

static void
flx_empty_handler(int sig)
{
	(void)sig;
}

static struct flx_kqent *
flx_kqent_find(int fd)
{
	struct flx_kqent *e;

	for (e = flx_kqents; e != NULL; e = e->next)
		if (e->fd == fd)
			return e;
	return NULL;
}

static void
flx_kqent_free(int fd)
{
	struct flx_kqent *e, **prev;

	prev = &flx_kqents;
	for (e = flx_kqents; e != NULL; e = e->next) {
		if (e->fd == fd) {
			*prev = e->next;
			free(e);
			return;
		}
		prev = &e->next;
	}
}

static int
flx_epollmask(struct flx_kqent *e)
{
	int m = 0;

	if (e->filters & 1)
		m |= EPOLLIN;
	if (e->filters & 2)
		m |= EPOLLOUT;
	if (m == 0)
		return 0;
	/* level-triggered; also watch HUP/ERR so closes are reported */
	m |= EPOLLHUP | EPOLLERR;
	return m;
}

static void
flx_ensure_signalfd(void)
{
	struct epoll_event ee;

	if (flx_kq < 0)
		return;
	if (flx_signalfd >= 0) {
		(void)epoll_ctl(flx_kq, EPOLL_CTL_DEL, flx_signalfd, NULL);
		(void)close(flx_signalfd);
	}
	flx_signalfd = signalfd(-1, &flx_sigmask, SFD_CLOEXEC | SFD_NONBLOCK);
	if (flx_signalfd < 0)
		return;
	memset(&ee, 0, sizeof(ee));
	ee.events = EPOLLIN;
	ee.data.fd = flx_signalfd;
	(void)epoll_ctl(flx_kq, EPOLL_CTL_ADD, flx_signalfd, &ee);
}

static int
flx_sig_register(int sig)
{
	sigset_t set;
	struct sigaction sa;

	if (sig < 1 || sig >= NSIG) {
		errno = EINVAL;
		return -1;
	}
	sigemptyset(&set);
	sigaddset(&set, sig);
	(void)sigprocmask(SIG_BLOCK, &set, NULL);
	sigaddset(&flx_sigmask, sig);

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = flx_empty_handler;
	sigemptyset(&sa.sa_mask);
	(void)sigaction(sig, &sa, NULL);

	flx_ensure_signalfd();
	if (flx_signalfd < 0)
		return -1;
	return 0;
}

/* Deliver pending signalfd signals as EVFILT_SIGNAL kevents. */
static size_t
flx_drain_signals(struct kevent *eventlist, size_t nevents)
{
	struct signalfd_siginfo si;
	size_t used = 0;

	while (read(flx_signalfd, &si, sizeof(si)) == (ssize_t)sizeof(si)) {
		if (used >= nevents)
			break;
		memset(&eventlist[used], 0, sizeof(eventlist[used]));
		eventlist[used].ident = (uintptr_t)si.ssi_signo;
		eventlist[used].filter = EVFILT_SIGNAL;
		eventlist[used].flags = 0;
		used++;
	}
	return used;
}

/* ---- EVFILT_PROC (pwait): /proc-based process-exit watch ---- */

static int
flx_proc_exists(const struct flx_kqproc *p)
{
	char path[32];

	if (snprintf(path, sizeof(path), "/proc/%ld", (long)p->pid) <= 0)
		return 0;
	return access(path, F_OK) == 0;
}

/* Push one NOTE_EXIT event into the eventlist; 0 when full. */
static int
flx_proc_event(struct kevent *eventlist, size_t nevents, size_t *used,
    const struct flx_kqproc *p, int status, int have_status)
{
	struct kevent *ev;

	if (*used >= nevents)
		return 0;
	ev = &eventlist[*used];
	memset(ev, 0, sizeof(*ev));
	ev->ident = (uintptr_t)p->pid;
	ev->filter = EVFILT_PROC;
	ev->fflags = NOTE_EXIT;
	/* data carries the wait status (best effort): for a zombie child the
	 * real status via waitid(WNOWAIT); otherwise 0. */
	ev->data = have_status ? status : 0;
	(*used)++;
	return 1;
}

/* Check the proc list; deliver events for exited/vanished pids.  Returns
 * the number of events delivered. */
static size_t
flx_check_procs(struct kevent *eventlist, size_t nevents)
{
	struct flx_kqproc *p, **prev;
	size_t used = 0;

	prev = &flx_kqprocs;
	for (p = flx_kqprocs; p != NULL; p = *prev) {
		int have = 0, status = 0;

		/* Reading /proc/<pid>/stat: format `pid (comm) state ...`;
		 * state Z means zombie (exited, waiting for our wait()). */
		if (!flx_proc_exists(p)) {
			/* gone without leaving a zombie: unknown status */
			have = 0;
		} else {
			char path[32], buf[256], *st;
			int fd;

			if (snprintf(path, sizeof(path), "/proc/%ld/stat",
			    (long)p->pid) <= 0)
				st = NULL;
			else {
				fd = open(path, O_RDONLY);
				st = NULL;
				if (fd >= 0) {
					ssize_t r = read(fd, buf, sizeof(buf) - 1);
					close(fd);
					if (r > 0) {
						buf[r] = '\0';
						st = strrchr(buf, ')');
						if (st != NULL && st[1] == ' ')
							st += 2;
					}
				}
			}
			if (st == NULL || *st != 'Z') {
				/* still alive (or not a zombie child) */
				prev = &p->next;
				continue;
			}
			/* exited child still zombie: recover real status */
			{
				siginfo_t si;
				memset(&si, 0, sizeof(si));
				if (waitid(P_PID, (id_t)p->pid, &si,
				    WEXITED | WNOHANG | WNOWAIT) == 0) {
					/* BSD event data carries the wait status
					 * (P_WAITSTATUS, W_EXITCODE-encoded):
					 * exit code << 8, signal raw, core
					 * dumps set WCOREFLAG (0x80). */
					if (si.si_code == CLD_EXITED)
						status = si.si_status << 8;
					else if (si.si_code == CLD_DUMPED)
						status = si.si_status | 0x80;
					else
						status = si.si_status;
					have = 1;
				}
			}
		}
		/* deliver and auto-remove (BSD destroys the knote) */
		if (flx_proc_event(eventlist, nevents, &used, p, status, have) == 0)
			break;
		*prev = p->next;
		free(p);
	}
	return used;
}

/*
 * Apply one change entry.  Returns 0 on success, -1 with errno on error.
 */
static int
flx_apply_change(const struct kevent *ev)
{
	struct flx_kqent *e;
	struct epoll_event ee;
	int was_present, mask;

	if (ev->filter == EVFILT_SIGNAL) {
		if (ev->flags & EV_DELETE)
			return 0;	/* signalfd mask stays; inetd never deletes */
		if (flx_sig_register((int)ev->ident) == -1)
			return -1;
		return 0;
	}
	if (ev->filter == EVFILT_PROC) {
		struct flx_kqproc *pr;

		if (ev->flags & EV_DELETE) {
			/* pwait never deletes; auto-removal at exit covers it */
			return 0;
		}
		if (!(ev->flags & EV_ADD))
			return 0;
		/* verify the pid exists now, like NetBSD's EVFILT_PROC ADD */
		{
			char path[32];
			if (snprintf(path, sizeof(path), "/proc/%ld",
			    (long)ev->ident) <= 0 || access(path, F_OK) != 0) {
				errno = ESRCH;
				return -1;
			}
		}
		for (pr = flx_kqprocs; pr != NULL; pr = pr->next)
			if (pr->pid == (pid_t)ev->ident)
				return 0;
		pr = calloc(1, sizeof(*pr));
		if (pr == NULL)
			return -1;
		pr->pid = (pid_t)ev->ident;
		pr->next = flx_kqprocs;
		flx_kqprocs = pr;
		return 0;
	}
	if (ev->filter != EVFILT_READ && ev->filter != EVFILT_WRITE) {
		errno = ENOTSUP;
		return -1;
	}

	if (ev->flags & EV_DELETE) {
		e = flx_kqent_find((int)ev->ident);
		if (e == NULL)
			return 0;
		mask = (ev->filter == EVFILT_READ) ? 1 : 2;
		e->filters &= ~mask;
		if (e->filters == 0) {
			(void)epoll_ctl(flx_kq, EPOLL_CTL_DEL, e->fd, NULL);
			flx_kqent_free(e->fd);
		} else {
			memset(&ee, 0, sizeof(ee));
			ee.events = flx_epollmask(e);
			ee.data.fd = e->fd;
			(void)epoll_ctl(flx_kq, EPOLL_CTL_MOD, e->fd, &ee);
		}
		return 0;
	}

	/* EV_ADD / EV_ENABLE / EV_DISABLE */
	was_present = flx_kqent_find((int)ev->ident) != NULL;
	e = flx_kqent_find((int)ev->ident);
	if (e == NULL) {
		if (!(ev->flags & EV_ADD)) {
			errno = ENOENT;
			return -1;
		}
		e = calloc(1, sizeof(*e));
		if (e == NULL)
			return -1;
		e->fd = (int)ev->ident;
		e->next = flx_kqents;
		flx_kqents = e;
	}

	if (ev->filter == EVFILT_READ) {
		if (ev->flags & EV_DISABLE)
			e->filters &= ~1;
		else {
			e->filters |= 1;
			e->udata_read = ev->udata;
		}
	} else {
		if (ev->flags & EV_DISABLE)
			e->filters &= ~2;
		else {
			e->filters |= 2;
			e->udata_write = ev->udata;
		}
	}

	if (flx_epollmask(e) == 0)
		return 0;
	memset(&ee, 0, sizeof(ee));
	ee.events = flx_epollmask(e);
	ee.data.fd = e->fd;
	if (epoll_ctl(flx_kq,
	    was_present ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, e->fd, &ee) == -1) {
		if (!was_present && errno == EEXIST) {
			/* a previous change in the same batch added it */
			if (epoll_ctl(flx_kq, EPOLL_CTL_MOD, e->fd, &ee) == -1)
				return -1;
		} else
			return -1;
	}
	return 0;
}

int
kqueue(void)
{
	flx_kq = epoll_create1(EPOLL_CLOEXEC);
	if (flx_kq < 0)
		return -1;
	flx_signalfd = -1;
	sigemptyset(&flx_sigmask);
	flx_kqents = NULL;
	flx_kqprocs = NULL;
	return flx_kq;
}

static int flx_remaining_ms(void);

int
kevent(int kq, const struct kevent *changelist, size_t nchanges,
    struct kevent *eventlist, size_t nevents, const struct timespec *timeout)
{
	struct epoll_event ee[64], *buf;
	int i, ms, used = 0, nev;
	size_t n;

	(void)kq;

	/* Apply changes; EV_RECEIPT errors go into the eventlist. */
	for (n = 0; n < nchanges; n++) {
		if (flx_apply_change(&changelist[n]) == -1) {
			if (eventlist != NULL &&
			    (changelist[n].flags & EV_RECEIPT)) {
				if ((size_t)used < nevents) {
					eventlist[used].ident = changelist[n].ident;
					eventlist[used].filter = changelist[n].filter;
					eventlist[used].flags = EV_ERROR;
					eventlist[used].data = errno;
					eventlist[used].udata = changelist[n].udata;
					used++;
				}
				continue;
			}
			return -1;
		}
	}

	if (eventlist == NULL || nevents == 0)
		return 0;

	if (timeout == NULL)
		ms = -1;
	else if (timeout->tv_sec < 0)
		ms = 0;
	else if ((long)timeout->tv_sec * 1000 + timeout->tv_nsec / 1000000 >
	    INT_MAX)
		ms = INT_MAX;
	else
		ms = (int)(timeout->tv_sec * 1000 + timeout->tv_nsec / 1000000);

	nev = (int)nevents;
	if (nev > (int)(sizeof(ee) / sizeof(ee[0])))
		nev = (int)(sizeof(ee) / sizeof(ee[0]));
	buf = ee;
	if (nev < (int)nevents) {
		buf = malloc((size_t)nev * sizeof(struct epoll_event));
		if (buf == NULL)
			buf = ee;	/* cap at the inline array */
	}

	/* Absolute deadline when the caller gave a timeout. */
	if (timeout != NULL && ms >= 0) {
		struct timespec now;

		clock_gettime(CLOCK_MONOTONIC, &now);
		flx_deadline.tv_sec = now.tv_sec + ms / 1000;
		flx_deadline.tv_nsec = now.tv_nsec +
		    (long)(ms % 1000) * 1000000L;
		if (flx_deadline.tv_nsec >= 1000000000L) {
			flx_deadline.tv_sec++;
			flx_deadline.tv_nsec -= 1000000000L;
		}
	}

	for (;;) {
		int cycle;

		/* Choose how long to block on epoll this cycle. */
		if (timeout != NULL && ms >= 0) {
			ms = flx_remaining_ms();
			if (ms <= 0)
				break;		/* deadline: return 0 events */
			cycle = (flx_kqprocs != NULL && ms > 50) ? 50 : ms;
		} else if (flx_kqprocs != NULL)
			cycle = 50;	/* wake to poll /proc for exits */
		else
			cycle = -1;

		i = epoll_wait(flx_kq, buf, nev, cycle);
		if (i < 0) {
			if (errno == EINTR)
				i = 0;	/* retry the cycle */
			else
				goto done_err;
		}

		if (i > 0) {
			for (n = 0; n < (size_t)i; n++) {
				struct flx_kqent *e;
				int evs = buf[n].events;

				if (flx_signalfd >= 0 &&
				    buf[n].data.fd == flx_signalfd) {
					used += (int)flx_drain_signals(
					    &eventlist[used],
					    nevents - (size_t)used);
					continue;
				}
				e = flx_kqent_find(buf[n].data.fd);
				if (e == NULL)
					continue;
				if ((e->filters & 1) &&
				    (evs & (EPOLLIN | EPOLLHUP | EPOLLERR))) {
					if ((size_t)used >= nevents)
						continue;
					eventlist[used].ident =
					    (uintptr_t)e->fd;
					eventlist[used].filter = EVFILT_READ;
					eventlist[used].flags =
					    (evs & (EPOLLHUP | EPOLLERR))
					    ? EV_EOF : 0;
					eventlist[used].data = 1;
					eventlist[used].udata = e->udata_read;
					used++;
				}
				if ((e->filters & 2) && (evs & EPOLLOUT)) {
					if ((size_t)used >= nevents)
						continue;
					eventlist[used].ident =
					    (uintptr_t)e->fd;
					eventlist[used].filter = EVFILT_WRITE;
					eventlist[used].flags = 0;
					eventlist[used].data = 1;
					eventlist[used].udata = e->udata_write;
					used++;
				}
			}
			if (used > 0)
				break;	/* deliver the fd events */
		}

		if (flx_kqprocs != NULL) {
			size_t pr = flx_check_procs(&eventlist[used],
			    nevents - (size_t)used);
			used += (int)pr;
			if (pr > 0)
				break;
		}

		/* No events yet: if nothing will ever arrive, stop.  With a
		 * deadline the loop-top break handles it (ms <= 0); without
		 * procs and no fd activity, an epoll timeout only happens
		 * on the last cycle. */
		if (i == 0 && flx_kqprocs == NULL) {
			if (timeout != NULL && ms <= 0)
				break;
			if (timeout == NULL && ms == -1 && nev > 0)
				break;	/* infinite wait returned 0: no fds */
		}
		if (i == 0 && flx_kqprocs == NULL && used == 0)
			break;
	}

done:
	if (buf != ee)
		free(buf);
	return used;

done_err:
	if (buf != ee)
		free(buf);
	return -1;
}

/* Milliseconds remaining until flx_deadline (CLOCK_MONOTONIC wall); 0 when
 * the deadline is past, clamping to INT_MAX ms for far futures. */
static int
flx_remaining_ms(void)
{
	struct timespec now;
	long sec, nsec;

	clock_gettime(CLOCK_MONOTONIC, &now);
	sec = flx_deadline.tv_sec - now.tv_sec;
	nsec = flx_deadline.tv_nsec - now.tv_nsec;
	if (nsec < 0) {
		sec--;
		nsec += 1000000000L;
	}
	if (sec < 0)
		return 0;
	if (sec > (INT_MAX / 1000))
		return INT_MAX;
	return (int)(sec * 1000 + nsec / 1000000);
}