/* FreeLinX/ports - base/compat/sigstub.c : BSD signal-mask + misc stubs.
 *
 * Implementations for interfaces NetBSD's <sys/signal.h>-era base
 * utilities use that musl does not provide: the sigblock(3)-style mask
 * ops, raise_default_signal(3) (abort-with-default-disposition helper used
 * by pr/error/csplit), BSD stime(2), flock(2) and openpty(3).  These are
 * FreeLinX Linux-native backends (real work on musl): sigprocmask for the
 * mask ops, kill+restore for raise_default_signal, settimeofday for
 * stime, fcntl(2) record locks for flock, posix_openpt for openpty.
 */
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/resource.h>
#include <sys/xattr.h>

static sigset_t
sigset_from_mask(int mask)
{
	sigset_t set;
	int s;
	sigemptyset(&set);
	for (s = 1; s < NSIG; s++)
		if (mask & (1 << (s - 1)))
			sigaddset(&set, s);
	return set;
}

static int
mask_from_sigset(const sigset_t *set)
{
	int mask = 0;
	int s;
	for (s = 1; s < NSIG; s++)
		if (sigismember(set, s))
			mask |= (1 << (s - 1));
	return mask;
}

int
sigmask(int signo)
{
	return 1 << (signo - 1);
}

int
sigblock(int mask)
{
	sigset_t set = sigset_from_mask(mask);
	sigset_t oset;

	if (sigprocmask(SIG_BLOCK, &set, &oset) == -1)
		return -1;
	return mask_from_sigset(&oset);
}

int
sigsetmask(int mask)
{
	sigset_t set = sigset_from_mask(mask);
	sigset_t oset;

	if (sigprocmask(SIG_SETMASK, &set, &oset) == -1)
		return -1;
	return mask_from_sigset(&oset);
}

int
raise_default_signal(int sig)
{
	struct sigaction sa, osa;
	sigset_t nmask, omask;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	if (sigaction(sig, &sa, &osa) == -1)
		return -1;
	sigemptyset(&nmask);
	sigaddset(&nmask, sig);
	sigprocmask(SIG_UNBLOCK, &nmask, &omask);
	(void)kill(getpid(), sig);
	sigprocmask(SIG_SETMASK, &omask, NULL);
	(void)sigaction(sig, &osa, NULL);
	return 0;
}

int
stime(const time_t *t)
{
	struct timeval tv;

	tv.tv_sec = *t;
	tv.tv_usec = 0;
	return settimeofday(&tv, NULL);
}

int
flock(int fd, int operation)
{
	struct flock fl;
	int cmd;

	if ((operation & ~LOCK_NB) != LOCK_SH &&
	    (operation & ~LOCK_NB) != LOCK_EX &&
	    (operation & ~LOCK_NB) != LOCK_UN) {
		errno = EINVAL;
		return -1;
	}
	memset(&fl, 0, sizeof(fl));
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;	/* to end of file */
	switch (operation & ~LOCK_NB) {
	case LOCK_SH:
		fl.l_type = F_RDLCK;
		break;
	case LOCK_EX:
		fl.l_type = F_WRLCK;
		break;
	default:
		fl.l_type = F_UNLCK;
		break;
	}
	cmd = (operation & LOCK_NB) ? F_SETLK : F_SETLKW;
	return fcntl(fd, cmd, &fl);
}

int
openpty(int *amaster, int *aslave, char *name,
    const struct termios *termp, const struct winsize *winp)
{
	int master, slave;
	char *slavename;

	master = posix_openpt(O_RDWR | O_NOCTTY);
	if (master == -1)
		return -1;
	if (grantpt(master) == -1 || unlockpt(master) == -1) {
		close(master);
		return -1;
	}
	slavename = ptsname(master);
	if (slavename == NULL) {
		close(master);
		errno = EINVAL;
		return -1;
	}
	slave = open(slavename, O_RDWR | O_NOCTTY);
	if (slave == -1) {
		close(master);
		return -1;
	}
	if (termp != NULL)
		(void)tcsetattr(slave, TCSAFLUSH, termp);
	if (winp != NULL)
		(void)ioctl(slave, TIOCSWINSZ, (char *)winp);
	if (name != NULL)
		(void)strcpy(name, slavename);
	*amaster = master;
	*aslave = slave;
	return 0;
}

int
login_tty(int fd)
{
	int i;

	if (setsid() == -1)
		return -1;
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return -1;
	for (i = 0; i < 3; i++)
		if (fd != i)
			(void)close(i);
	if (fd > 2) {
		for (i = 0; i < 3; i++)
			if (dup2(fd, i) == -1)
				return -1;
		(void)close(fd);
	}
	return 0;
}

long long
strtonum(const char *numstr, long long minval, long long maxval,
    const char **errstrp)
{
	const char *ep;
	unsigned long long uval;
	long long val;
	int base = 10;
	int neg = 0;
	const char *errstr = "invalid";

	if (minval > maxval) {
		errno = EINVAL;
		if (errstrp != NULL)
			*errstrp = "invalid";
		return 0;
	}

	ep = numstr;
	while (isspace((unsigned char)*ep))
		ep++;
	if (*ep == '-') {
		neg = 1;
		ep++;
	} else if (*ep == '+') {
		ep++;
	}
	if (*ep == '0' && (ep[1] == 'x' || ep[1] == 'X')) {
		base = 16;
		ep += 2;
	} else if (*ep == '0' && isdigit((unsigned char)ep[1])) {
		base = 8;
		ep++;
	}
	if (*ep == '\0')
		goto done;

	errno = 0;
	uval = strtoull(ep, (char **)&ep, base);
	if (errno == ERANGE) {
		errstr = "too large";
		goto done;
	}
	if (*ep != '\0') {
		errstr = "invalid";
		goto done;
	}
	if (neg) {
		if (uval > (unsigned long long)LLONG_MAX + 1) {
			errstr = "too small";
			goto done;
		}
		val = (uval == (unsigned long long)LLONG_MAX + 1) ?
		    LLONG_MIN : -(long long)uval;
	} else {
		if (uval > (unsigned long long)maxval) {
			errstr = "too large";
			goto done;
		}
		val = (long long)uval;
	}
	if (val < minval || val > maxval) {
		errstr = "out of range";
		goto done;
	}
	errno = 0;
	if (errstrp != NULL)
		*errstrp = NULL;
	return val;

done:
	errno = ERANGE;
	if (errstrp != NULL)
		*errstrp = errstr;
	return 0;
}

int
uid_from_user(const char *name, uid_t *uidp)
{
	struct passwd *pw;

	if (name == NULL)
		return -1;
	pw = getpwnam(name);
	if (pw == NULL)
		return -1;
	*uidp = pw->pw_uid;
	return 0;
}

int
gid_from_group(const char *name, gid_t *gidp)
{
	struct group *gr;

	if (name == NULL)
		return -1;
	gr = getgrnam(name);
	if (gr == NULL)
		return -1;
	*gidp = gr->gr_gid;
	return 0;
}

int
fsync_range(int fd, int how, off_t off, off_t len)
{
	/* NetBSD fsync_range(2); Linux has no equivalent, fdatasync is the
	 * closest semantic (persist data for `fd`).  how/off/len ignored. */
	(void)how;
	(void)off;
	(void)len;
	return fdatasync(fd);
}

/*
 * libutil stubs for usr.bin/xinstall: BSD file-|flags and user-db helpers
 * that have no Linux analogue.  FreeLinX stores no chflags(2) metadata, so
 * string_to_flags()/flags_to_string() accept/return an empty flag set; the
 * -N (alternate user/group database) install mode is a no-op.
 */
int
string_to_flags(char **stringp, unsigned long *setp, unsigned long *clrp)
{
	if (stringp != NULL)
		*stringp = NULL;
	if (setp != NULL)
		*setp = 0;
	if (clrp != NULL)
		*clrp = 0;
	return 0;
}

char *
flags_to_string(unsigned long flags, const char *def)
{
	return (flags == 0 && def != NULL) ? (char *)def : "";
}

int
setup_getid(const char *dbbase)
{
	(void)dbbase;
	return 1;
}

/*
 * NetBSD snprintf_ss(3)/vsnprintf_ss(3)/vsnprintf_ssm(3): like the plain
 * *snprintf(3) family but return -1 when the output does not fit (so the
 * caller can detect truncation as an error).  FreeLinX implements all three
 * on top of musl vsnprintf; the _ssm (multibyte) form is identical.
 */
int
vsnprintf_ss(char *buf, size_t size, const char *fmt, va_list ap)
{
	int len;

	len = vsnprintf(buf, size, fmt, ap);
	if (len < 0 || (size_t)len >= size)
		return -1;
	return len;
}

int
snprintf_ss(char *buf, size_t size, const char *fmt, ...)
{
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsnprintf_ss(buf, size, fmt, ap);
	va_end(ap);
	return len;
}

int
vsnprintf_ssm(char *buf, size_t size, const char *fmt, va_list ap)
{
	return vsnprintf_ss(buf, size, fmt, ap);
}

/*
 * libutil sockaddr_snprintf(3): format a sockaddr per BSD-style fmt.
 * usr.bin/getaddrinfo uses the %a (numeric address), %p (port) and,
 * for non-INET sockets, %F (family name) conversions.  Real Linux
 * backends: inet_ntop for the address, ntohs for the port.
 */
#include <netinet/in.h>
#include <arpa/inet.h>

static const char *
addr_family_name(int fam)
{
	switch (fam) {
	case AF_INET:	return "inet";
	case AF_INET6:	return "inet6";
	case AF_UNIX:	return "local";
	case AF_UNSPEC:	return "unspec";
	default:	return "unknown";
	}
}

int
sockaddr_snprintf(char *buf, size_t buflen, const char *fmt,
    const struct sockaddr *sa)
{
	size_t off = 0;
	const char *p;

	if (buflen == 0)
		return 0;
	for (p = fmt; *p != '\0' && off + 1 < buflen; p++) {
		if (*p != '%') {
			buf[off++] = *p;
			continue;
		}
		switch (*++p) {
		case 'a': {
			char abuf[INET6_ADDRSTRLEN];
			const struct sockaddr_in *sin =
			    (const struct sockaddr_in *)sa;
			const struct sockaddr_in6 *sin6 =
			    (const struct sockaddr_in6 *)sa;
			const void *addr = NULL;

			if (sa->sa_family == AF_INET)
				addr = &sin->sin_addr;
			else if (sa->sa_family == AF_INET6)
				addr = &sin6->sin6_addr;
			if (addr != NULL &&
			    inet_ntop(sa->sa_family, addr, abuf,
			    sizeof(abuf)) != NULL)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%s", abuf);
			else
				off += (size_t)snprintf(buf + off,
				    buflen - off, "?"); 
			break;
		}
		case 'p':
			if (sa->sa_family == AF_INET)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%u",
				    (unsigned)ntohs(((const struct
				    sockaddr_in *)sa)->sin_port));
			else if (sa->sa_family == AF_INET6)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%u",
				    (unsigned)ntohs(((const struct
				    sockaddr_in6 *)sa)->sin6_port));
			else
				off += (size_t)snprintf(buf + off,
				    buflen - off, "?");
			break;
		case 'F':
			off += (size_t)snprintf(buf + off, buflen - off,
			    "%s", addr_family_name(sa->sa_family));
			break;
		case 'I':
			if (sa->sa_family == AF_INET6)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%u",
				    (unsigned)((const struct
				    sockaddr_in6 *)sa)->sin6_scope_id);
			break;
		case 'R': {
			const unsigned char *u =
			    (const unsigned char *)sa;
			size_t i;
			for (i = 0; i < 16; i++)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%02x", u[i]);
			break;
		}
		case 'S':
			if (sa->sa_family == AF_INET)
				off += (size_t)snprintf(buf + off,
				    buflen - off, "%u",
				    (unsigned)ntohs(((const struct
				    sockaddr_in *)sa)->sin_port));
			break;
		default:
			off += (size_t)snprintf(buf + off, buflen - off,
			    "%c", *p);
			break;
		}
	}
	buf[off < buflen ? off : buflen - 1] = '\0';
	return (int)off;
}

/*
 * BSD libc stay-open knobs for the passwd/group databases (musl lacks
 * setpassent/setgroupent).  glibc-style: returning 0 asks the caller
 * not to keep the file descriptors open; that is musl's behaviour, so
 * return 0 and the enumeration functions behave natively.
 */
int
setpassent(int stayopen)
{
	(void)stayopen;
	return 0;
}

int
setgroupent(int stayopen)
{
	(void)stayopen;
	return 0;
}

/*
 * NetBSD-specific knob that (with an argument) makes getcap expand
 * "tc=..." entries; libtinfo expands those unconditionally, so the
 * call is a no-op here.
 */
void
csetexpandtc(int doexpand)
{
	(void)doexpand;
}

/*
 * BSD revoke(2): close all open file references to path.  musl neither
 * declares nor provides it; Linux has revoke(2) only for terminal-like
 * devices and the utmp-relevant callers (quota) use regular files.  A
 * stub is the honest analogue: no Linux semantics to preserve.
 */
int
revoke(const char *path)
{
	(void)path;
	errno = EOPNOTSUPP;
	return -1;
}

/*
 * libutil dehumanize_number(3): parse a human-size string ("42", "1.5M",
 * with optional k/m/g/t/p/e scale suffixes at 1024^k) into an int64_t.
 * Mirrors the NetBSD contract: 0 on success, -1 with errno EINVAL/ERANGE.
 */
int
dehumanize_number(const char *str, int64_t *result)
{
	double val;
	const char *p;
	char *end;
	long long scale = 1;
	int exp = 0;

	p = str;
	while (isspace((unsigned char)*p))
		p++;
	val = strtod(p, &end);
	if (end == p) {
		errno = EINVAL;
		return -1;
	}
	switch (tolower((unsigned char)*end)) {
	case 'k':	exp = 1; break;
	case 'm':	exp = 2; break;
	case 'g':	exp = 3; break;
	case 't':	exp = 4; break;
	case 'p':	exp = 5; break;
	case 'e':	exp = 6; break;
	case 'b':	scale = 512; break;
	case '\0':	break;
	default:	errno = EINVAL; return -1;
	}
	if (exp != 0) {
		end++;
		scale = 1;
		while (exp-- != 0)
			scale *= 1024;
	}
	if (tolower((unsigned char)*end) == 'b')
		end++;
	while (isspace((unsigned char)*end))
		end++;
	if (*end != '\0') {
		errno = EINVAL;
		return -1;
	}
	val *= (double)scale;
	if (val > (double)LLONG_MAX || val < (double)LLONG_MIN) {
		errno = ERANGE;
		return -1;
	}
	*result = (int64_t)val;
	return 0;
}

/*
 * NetBSD closefrom(2): close every descriptor >= fd (tip, rsh).  Linux has
 * no close_range-by-value syscall in the base musl API, so walk the fd table
 * to the resource limit.
 */
int
closefrom(int fd)
{
	struct rlimit rl;
	int i;

	if (fd < 0) {
		errno = EINVAL;
		return -1;
	}
	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
		return -1;
	for (i = fd; i < (int)rl.rlim_max; i++)
		close(i);
	return 0;
}

/*
 * NetBSD snprintb(3): fprintb-style bit-field printer.  fmt has one %llx
 * conversion for the unshifted remainder; the b bit specifiers generate hex
 * lines much like NetBSD's.  Videoctl uses it to decode device register
 * flags; a faithful subset is enough for the tool to run.
 */
char *
snprintb(char *buf, size_t len, const char *fmt, uint64_t val)
{
	const char *p;
	char *out = buf;
	size_t left = len;

	if (left == 0)
		return buf;
	for (p = fmt; *p && left > 1; ) {
		if (*p == '%' && p[1] == '\\') {
			p += 2;
			continue;
		}
		if (*p == '%' && (p[1] == 'l' || p[1] == '0')) {
			int n;
			n = snprintf(out, left, "%llx", (unsigned long long)val);
			if (n < 0 || n >= (int)left)
				break;
			out += n;
			left -= n;
			p += 2;
			continue;
		}
		if (*p == '\\') {
			char c = '\0';
			int m;
			switch (p[1]) {
			case 'b': c = '\b'; m = 2; break;
			case 't': c = '\t'; m = 2; break;
			case 'n': c = '\n'; m = 2; break;
			case 'r': c = '\r'; m = 2; break;
			default: c = p[1]; m = 2; break;
			}
			p += m;
			continue;
		}
		if ((unsigned char)*p >= 0x20 && *p != '%') {
			*out++ = *p++;
			left--;
			continue;
		}
		if (*p++ == '\n')
			*out++ = '\n';
	}
	*out = '\0';
	return buf;
}

/*
 * NetBSD warnc(3): like warn(3) but the diagnostic code comes from the
 * argument instead of errno (scmdctl).
 */
void
warnc(int code, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (fmt != NULL) {
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, ": ");
	}
	va_end(ap);
	fprintf(stderr, "%s\n", strerror(code));
}

/*
 * NetBSD extattr_* userland (usr.bin/extattr).  Linux syscalls the BSD
 * extended-attribute API (setxattr/getxattr/listxattr/removexattr) with a
 * different spelling and a string namespace argument instead of the BSD
 * attrnamespace constants; map get/list/remove so the tool works on Linux
 * extended attributes, and stub the set path (namespaces differ).
 */
int
extattr_namespace_to_string(int attrnamespace, char *name, size_t size)
{
	(void)attrnamespace;
	(void)name;
	(void)size;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_string_to_namespace(const char *name, int *attrnamespace)
{
	(void)name;
	(void)attrnamespace;
	errno = EOPNOTSUPP;
	return -1;
}

ssize_t
extattr_list_fd(int fd, int attrnamespace, void *data, size_t nbytes)
{
	(void)fd;
	(void)attrnamespace;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

ssize_t
extattr_list_file(const char *path, int attrnamespace, void *data, size_t nbytes)
{
	if (attrnamespace != EXTATTR_NAMESPACE_USER) {
		errno = EINVAL;
		return -1;
	}
	return listxattr(path, data, nbytes);
}

ssize_t
extattr_list_link(const char *path, int attrnamespace, void *data, size_t nbytes)
{
	(void)path;
	(void)attrnamespace;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

ssize_t
extattr_get_fd(int fd, int attrnamespace, const char *name, void *data, size_t nbytes)
{
	(void)fd;
	(void)attrnamespace;
	(void)name;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

ssize_t
extattr_get_file(const char *path, int attrnamespace, const char *name, void *data, size_t nbytes)
{
	if (attrnamespace != EXTATTR_NAMESPACE_USER) {
		errno = EINVAL;
		return -1;
	}
	return getxattr(path, name, data, nbytes);
}

ssize_t
extattr_get_link(const char *path, int attrnamespace, const char *name, void *data, size_t nbytes)
{
	(void)path;
	(void)attrnamespace;
	(void)name;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_set_fd(int fd, int attrnamespace, const char *name, const void *data, size_t nbytes)
{
	(void)fd;
	(void)attrnamespace;
	(void)name;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_set_file(const char *path, int attrnamespace, const char *name, const void *data, size_t nbytes)
{
	(void)path;
	(void)attrnamespace;
	(void)name;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_set_link(const char *path, int attrnamespace, const char *name, const void *data, size_t nbytes)
{
	(void)path;
	(void)attrnamespace;
	(void)name;
	(void)data;
	(void)nbytes;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_delete_fd(int fd, int attrnamespace, const char *name)
{
	(void)fd;
	(void)attrnamespace;
	(void)name;
	errno = EOPNOTSUPP;
	return -1;
}

int
extattr_delete_file(const char *path, int attrnamespace, const char *name)
{
	if (attrnamespace != EXTATTR_NAMESPACE_USER) {
		errno = EINVAL;
		return -1;
	}
	return removexattr(path, name);
}

int
extattr_delete_link(const char *path, int attrnamespace, const char *name)
{
	(void)path;
	(void)attrnamespace;
	(void)name;
	errno = EOPNOTSUPP;
	return -1;
}

