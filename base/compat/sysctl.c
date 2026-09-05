/*
 * FreeLinX/ports - base/compat/sysctl.c : minimal sysctl(3).
 *
 * Backs the query families the FreeLinX usr.bin set uses:
 *
 *   CTL_USER/USER_CS_PATH   the standard command-search path, from the
 *   			     compiler's _CS_PATH (usr.bin/whereis).
 *
 *   CTL_KERN/KERN_SYSVIPC   the SysV IPC tables (ipcrm(1)/ipcs(1)),
 *   			     answered from the Linux /proc/sysvipc files
 *   			     and delivered in the NetBSD 10 layout:
 *   			     a *_sysctl_info header whose msginfo/
 *   			     seminfo/shminfo block is followed by a
 *   			     flexible array of *_ds_sysctl records, one
 *   			     per /proc row.
 *
 * The three-name form ({KERN_SYSVIPC,MSG|SEM|SHM}, namelen 3) is the
 * "is this configured" probe ipcs issues; it answers a boolean.
 * Everything else fails ENOTSUP.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <sys/sysvipc.h>

#define	SVIPC_MAX	1024

struct lrow {
	unsigned long	key;
	int		id;
	unsigned long	perms;
	unsigned long	m0, m1;
	int		p0, p1;
	int		u0, u1, u2, u3;
	long		t0, t1, t2;
};

static int
svipc_which(const int *name, u_int namelen)
{
	if (name == NULL || namelen < 3 || name[0] != CTL_KERN ||
	    name[1] != KERN_SYSVIPC)
		return 0;
	switch (name[2]) {
	case KERN_SYSVIPC_MSG:
		return 'm';
	case KERN_SYSVIPC_SEM:
		return 's';
	case KERN_SYSVIPC_SHM:
		return 'h';
	case KERN_SYSVIPC_INFO:
		if (namelen >= 4) {
			switch (name[3]) {
			case KERN_SYSVIPC_MSG_INFO:
				return 'm';
			case KERN_SYSVIPC_SEM_INFO:
				return 's';
			case KERN_SYSVIPC_SHM_INFO:
				return 'h';
			}
		}
		return 0;
	}
	return 0;
}

static const char *
svipc_path(int kind)
{
	switch (kind) {
	case 'm': return "/proc/sysvipc/msg";
	case 's': return "/proc/sysvipc/sem";
	default:  return "/proc/sysvipc/shm";
	}
}

static int
svipc_probe(int kind)
{
	FILE *fp = fopen(svipc_path(kind), "r");

	if (fp == NULL)
		return 0;
	fclose(fp);
	return 1;
}

/* Linux prints the times as HH:MM:SS up to nearly a day on some
 * kernels, or as raw epoch seconds on others; answer in seconds so
 * ctime(3)-style formatting gives a plausible stamp. */
static long
hms_to_sec(unsigned h, unsigned m, unsigned s)
{
	return (long)h * 3600L + (long)m * 60L + (long)s;
}

static int
svipc_parse(int kind, struct lrow *out, size_t *nout)
{
	FILE *fp;
	char line[512];
	size_t n = 0;

	fp = fopen(svipc_path(kind), "r");
	if (fp == NULL)
		return -1;
	/* Discard the header line. */
	if (fgets(line, sizeof(line), fp) == NULL) {
		fclose(fp);
		*nout = 0;
		return 0;
	}
	while (n < SVIPC_MAX && fgets(line, sizeof(line), fp) != NULL) {
		struct lrow *r = &out[n];
		unsigned h0, m0t, s0, h1, m1t, s1, h2, m2t, s2;
		int conv;
		int has_colon;

		if (line[0] == '#')
			continue;
		switch (kind) {
		case 'm':
			/*
			 * key msqid perms cbytes qnum lspid lrpid
			 * uid gid cuid cgid stime rtime ctime
			 * (times are HH:MM:SS or raw epoch seconds)
			 */
			conv = sscanf(line,
			    "%lu %d %o %lu %lu %d %d %d %d %d %d"
			    " %lu",
			    &r->key, &r->id, &r->perms,
			    &r->m0, &r->m1, &r->p0, &r->p1,
			    &r->u0, &r->u1, &r->u2, &r->u3,
			    &r->t0);
			if (conv < 12)
				continue;
			has_colon = strchr(line, ':') != NULL;
			if (has_colon) {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*lu %*d %*d"
				    " %*d %*d %*d %*d"
				    " %u:%u:%u %u:%u:%u %u:%u:%u",
				    &h0, &m0t, &s0, &h1, &m1t, &s1,
				    &h2, &m2t, &s2);
				if (conv < 9)
					continue;
				r->t0 = hms_to_sec(h0, m0t, s0);
				r->t1 = hms_to_sec(h1, m1t, s1);
				r->t2 = hms_to_sec(h2, m2t, s2);
			} else {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*lu %*d %*d"
				    " %*d %*d %*d %*d"
				    " %lu %lu %lu",
				    &r->t0, &r->t1, &r->t2);
				if (conv < 3)
					continue;
			}
			n++;
			break;
		case 's':
			/*
			 * key semid perms nsems uid gid cuid cgid
			 * otime ctime
			 */
			conv = sscanf(line,
			    "%lu %d %o %lu %d %d %d %d",
			    &r->key, &r->id, &r->perms, &r->m0,
			    &r->u0, &r->u1, &r->u2, &r->u3);
			if (conv < 8)
				continue;
			has_colon = strchr(line, ':') != NULL;
			if (has_colon) {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*d %*d %*d %*d"
				    " %u:%u:%u %u:%u:%u",
				    &h0, &m0t, &s0, &h1, &m1t, &s1);
				if (conv < 6)
					continue;
				r->t0 = hms_to_sec(h0, m0t, s0);
				r->t1 = hms_to_sec(h1, m1t, s1);
			} else {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*d %*d %*d %*d"
				    " %lu %lu",
				    &r->t0, &r->t1);
				if (conv < 2)
					continue;
			}
			n++;
			break;
		default:
			/*
			 * key shmid perms size cpid lpid nattch
			 * uid gid cuid cgid atime dtime ctime
			 */
			conv = sscanf(line,
			    "%lu %d %o %lu %d %d %lu %d %d %d %d"
			    " %lu",
			    &r->key, &r->id, &r->perms, &r->m0,
			    &r->p0, &r->p1, &r->m1,
			    &r->u0, &r->u1, &r->u2, &r->u3,
			    &r->t0);
			if (conv < 12)
				continue;
			has_colon = strchr(line, ':') != NULL;
			if (has_colon) {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*d %*d %*lu"
				    " %*d %*d %*d %*d"
				    " %u:%u:%u %u:%u:%u %u:%u:%u",
				    &h0, &m0t, &s0, &h1, &m1t, &s1,
				    &h2, &m2t, &s2);
				if (conv < 9)
					continue;
				r->t0 = hms_to_sec(h0, m0t, s0);
				r->t1 = hms_to_sec(h1, m1t, s1);
				r->t2 = hms_to_sec(h2, m2t, s2);
			} else {
				conv = sscanf(line,
				    "%*lu %*d %*o %*lu %*d %*d %*lu"
				    " %*d %*d %*d %*d"
				    " %lu %lu %lu",
				    &r->t0, &r->t1, &r->t2);
				if (conv < 3)
					continue;
			}
			n++;
			break;
		}
	}
	fclose(fp);
	*nout = n;
	return 0;
}

/* mode low bits: permission bits (fmt_perm only reads the 9 rwx)
 * top 16 bits: the real kernel ipcid /proc reported, recovered by
 * IXSEQ_TO_IPCID().  allocbit flags the slot alive for the caller. */
static void
perm_fill(struct ipc_perm *p, const struct lrow *r, unsigned allocbit)
{
	memset(p, 0, sizeof(*p));
	p->_key = (key_t)r->key;
	p->uid = (uid_t)r->u0;
	p->gid = (gid_t)r->u1;
	p->cuid = (uid_t)r->u2;
	p->cgid = (gid_t)r->u3;
	p->mode = (mode_t)((r->perms & 07777) |
	    ((unsigned long)r->id << 16) | allocbit);
}

static size_t
svipc_info_size(int kind, size_t n)
{
	switch (kind) {
	case 'm':
		return sizeof(struct msg_sysctl_info) +
		    n * sizeof(struct msgid_ds_sysctl);
	case 's':
		return sizeof(struct sem_sysctl_info) +
		    n * sizeof(struct semid_ds_sysctl);
	default:
		return sizeof(struct shm_sysctl_info) +
		    n * sizeof(struct shmid_ds_sysctl);
	}
}

static int
svipc_probe_call(int kind, void *oldp, size_t *oldlenp)
{
	long lv = svipc_probe(kind) ? 1 : 0;
	size_t cp = *oldlenp;

	if (oldp == NULL) {
		*oldlenp = sizeof(lv);
		return 0;
	}
	if (cp > sizeof(lv))
		cp = sizeof(lv);
	memcpy(oldp, &lv, cp);
	*oldlenp = cp;
	return 0;
}

int
sysctl(const int *name, u_int namelen, void *oldp, size_t *oldlenp,
    const void *newp, size_t newlen)
{
	struct lrow rows[SVIPC_MAX];
	size_t n = 0;
	size_t sz;
	size_t cp;
	int kind;
	int i;

	(void)newp;
	(void)newlen;

	if (oldlenp == NULL) {
		errno = EINVAL;
		return -1;
	}

	/* CTL_USER/USER_CS_PATH: standard path list. */
	if (name != NULL && namelen >= 2 && name[0] == CTL_USER &&
	    name[1] == USER_CS_PATH) {
		size_t got;
		char cs[1024];

		got = confstr(_CS_PATH, cs, sizeof(cs));
		if (got == 0 || got >= sizeof(cs))
			goto unsup;
		sz = strlen(cs) + 1;
		if (oldp == NULL) {
			*oldlenp = sz;
			return 0;
		}
		if (*oldlenp < sz) {
			errno = ENOMEM;
			return -1;
		}
		memcpy(oldp, cs, sz);
		*oldlenp = sz;
		return 0;
	}

	kind = svipc_which(name, namelen);
	if (kind == 0)
		goto unsup;

	/* {KERN_SYSVIPC,MSG|SEM|SHM}: "is it configured" probe. */
	if (namelen == 3)
		return svipc_probe_call(kind, oldp, oldlenp);

	if (svipc_parse(kind, rows, &n) != 0)
		goto unsup;

	sz = svipc_info_size(kind, n);
	if (oldp == NULL) {
		*oldlenp = sz;
		return 0;
	}

	/* Truncating write for the "totals only" calls, NetBSD style:
	 * they pass len = sizeof(struct msginfo) and only read that. */
	cp = *oldlenp < sz ? *oldlenp : sz;
	*oldlenp = cp;
	if (cp == 0)
		return 0;

	switch (kind) {
	case 'm': {
		struct msg_sysctl_info *mi = oldp;
		struct msgid_ds_sysctl *ms = mi->msgids;

		memset(mi, 0, cp);
		mi->msginfo.msgmax = 4096;
		mi->msginfo.msgmni = (int)n;
		mi->msginfo.msgmnb = 16384;
		mi->msginfo.msgtql = -1;
		mi->msginfo.msgssz = 8;
		mi->msginfo.msgseg = 2048;
		for (i = 0; i < (int)n; i++) {
			perm_fill(&ms[i].msg_perm, &rows[i], 0);
			ms[i].msg_stime = rows[i].t0;
			ms[i].msg_rtime = rows[i].t1;
			ms[i].msg_ctime = rows[i].t2;
			ms[i]._msg_cbytes = (size_t)rows[i].m0;
			ms[i].msg_qnum = (msgqnum_t)rows[i].m1;
			ms[i].msg_qbytes = 16384;	/* alive: != 0 */
			ms[i].msg_lspid = rows[i].p0;
			ms[i].msg_lrpid = rows[i].p1;
		}
		break;
	}
	case 's': {
		struct sem_sysctl_info *si = oldp;
		struct semid_ds_sysctl *ss = si->semids;

		memset(si, 0, cp);
		si->seminfo.semmns = (int)n;
		si->seminfo.semmni = (int)n;
		si->seminfo.semmsl = 0;
		si->seminfo.semopm = 0;
		si->seminfo.semume = 0;
		si->seminfo.semusz = 0;
		si->seminfo.semvmx = 0;
		si->seminfo.semaem = 0;
		for (i = 0; i < (int)n; i++) {
			perm_fill(&ss[i].sem_perm, &rows[i], SEM_ALLOC);
			ss[i].sem_otime = rows[i].t0;
			ss[i].sem_ctime = rows[i].t1;
			ss[i].sem_nsems = (int)rows[i].m0;
		}
		break;
	}
	default: {
		struct shm_sysctl_info *hi = oldp;
		struct shmid_ds_sysctl *hs = hi->shmids;

		memset(hi, 0, cp);
		hi->shminfo.shmmax = 0x200000;
		hi->shminfo.shmmin = 1;
		hi->shminfo.shmmni = (int)n;
		hi->shminfo.shmseg = 0;
		hi->shminfo.shmall = 0;
		for (i = 0; i < (int)n; i++) {
			perm_fill(&hs[i].shm_perm, &rows[i], 0x0800);
			hs[i].shm_atime = rows[i].t0;
			hs[i].shm_dtime = rows[i].t1;
			hs[i].shm_ctime = rows[i].t2;
			hs[i].shm_segsz = (size_t)rows[i].m0;
			hs[i].shm_cpid = rows[i].p0;
			hs[i].shm_lpid = rows[i].p1;
			hs[i].shm_nattch = (int)rows[i].m1;
		}
		break;
	}
	}
	return 0;

 unsup:
	errno = ENOTSUP;
	return -1;
}

int
sysctlbyname(const char *name, void *oldp, size_t *oldlenp,
    const void *newp, size_t newlen)
{
	static const struct {
		const char *nm;
		int mib[4];
		u_int len;
	} names[] = {
		{ "user.cs_path",
		    { CTL_USER, USER_CS_PATH }, 2 },
		{ "kern.sysvipc.info.msg",
		    { CTL_KERN, KERN_SYSVIPC, KERN_SYSVIPC_INFO,
		      KERN_SYSVIPC_MSG_INFO }, 4 },
		{ "kern.sysvipc.info.sem",
		    { CTL_KERN, KERN_SYSVIPC, KERN_SYSVIPC_INFO,
		      KERN_SYSVIPC_SEM_INFO }, 4 },
		{ "kern.sysvipc.info.shm",
		    { CTL_KERN, KERN_SYSVIPC, KERN_SYSVIPC_INFO,
		      KERN_SYSVIPC_SHM_INFO }, 4 },
	};
	size_t i;

	for (i = 0; i < sizeof(names) / sizeof(names[0]); i++)
		if (strcmp(name, names[i].nm) == 0)
			return sysctl(names[i].mib, names[i].len, oldp,
			    oldlenp, newp, newlen);
	errno = ENOTSUP;
	return -1;
}