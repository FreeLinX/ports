/* FreeLinX/ports - base/compat/flx_bsd.h : musl BSD-macro/compat surface.
 *
 * Minimal FreeLinX stand-in for miscellaneous NetBSD <sys/param.h>,
 * <time.h>, <regex.h>, <sys/file.h> and <sys/socket.h> constants and the
 * prototypes of the BSD libc helpers vendored/implemented in this dir.
 * Included (via -include) in every base utility compile through the
 * base-port.mk default FLX_CPPFLAGS, so ports need no per-port flags for
 * the common spellings NetBSD's <sys/param.h> etc. provide.  Everything is
 * #ifndef-guarded so a port that already defines the symbol is untouched.
 */
#ifndef _FREELINX_FLX_BSD_H_
#define _FREELINX_FLX_BSD_H_

/* -include flx_bsd.h happens before any port source, so gates defined here
 * land before the first <stdio.h> in every TU.  musl exposes several
 * declarations the NetBSD base set uses - fopencookie(3)/cookie functions
 * (used by compat/funopen.c for compress(1)), strcasestr - only under
 * _GNU_SOURCE.  musl's _GNU_SOURCE changes no struct layouts. */
#if !defined(_GNU_SOURCE)
#define	_GNU_SOURCE	1
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <endian.h>
#include <pwd.h>
#include <time.h>
#include <limits.h>

/* BSD __unused is a statement attribute; musl never defines it.  But musl's
 * <bits/stat.h> (x86_64) has struct-stat members literally named `__unused[3]`,
 * which a bare `#define __unused ...` would corrupt.  FreeLinX's cross sysroot
 * renames those members (see toolchain sysroot build: bits/stat.h __flx_unused)
 * so this global macro is safe for NetBSD code. */
#ifndef __unused
#define	__unused	__attribute__((__unused__))
#endif

#ifndef ACCESSPERMS
#define	ACCESSPERMS	0777
#endif
#ifndef DAYSPERWEEK
#define	DAYSPERWEEK	7
#endif
#ifndef SECSPERMIN
#define	SECSPERMIN	60
#endif
#ifndef SECSPERHOUR
#define	SECSPERHOUR	(60 * 60)
#endif
#ifndef IPPORT_RESERVED
#define	IPPORT_RESERVED		(1024)
#endif
#ifndef IPPORT_USERRESERVED
#define	IPPORT_USERRESERVED	(0x4000)
#endif
#ifndef IPPORT_ANONMAX
#define	IPPORT_ANONMAX		(IPPORT_USERRESERVED - 1)
#endif
#ifndef MAXNAMLEN
#define	MAXNAMLEN	255
#endif
#ifndef __CTASSERT
#define	__CTASSERT(x)		_Static_assert((x), #x)
#endif
#ifndef CTASSERT
#define	CTASSERT(x)		__CTASSERT(x)
#endif
#ifndef rounddown
#define	rounddown(x, y)		(((x) / (y)) * (y))
#endif
#ifndef isleap
#define	isleap(y)		(((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))
#endif
#ifndef DAYSPERLYEAR
#define	DAYSPERLYEAR	366
#endif
#ifndef __diagused
#define	__diagused	__attribute__((__unused__))
#endif
#ifndef OFF_MAX
#define	OFF_MAX		((off_t)LLONG_MAX)
#endif
#ifndef _PATH_CSHELL
#define	_PATH_CSHELL	"/bin/csh"
#endif
extern const char * const sys_signame[NSIG + 1];
#ifndef REG_BASIC
#define	REG_BASIC	0
#endif
/* BSD flock(2) constants via LM-mode only on Linux; fcntl locks instead. */
#ifndef LOCK_SH
#define	LOCK_SH		1
#define	LOCK_EX		2
#define	LOCK_NB		4
#define	LOCK_UN		8
#endif
/* musl's struct sockaddr_storage has no ss_len member (BSD ABI detail). */
#ifndef FLX_SSLEN_HACK
#define	FLX_SSLEN_HACK	1
#define	ss_len		ss_family
#endif

/* fparseln(3): NetBSD libc line-reader (fparseln.c in this dir). */
#define	FPARSELN_UNESCAPE	0x01
#define	FPARSELN_UNESCCONT	0x02
#define	FPARSELN_UNESCNL	0x04
#define	FPARSELN_UNESCCOMM	0x08
#define	FPARSELN_UNESCREST	0x10
#define	FPARSELN_UNESCESC	0x20
#define	FPARSELN_UNESCALL	0x3f
char *fparseln(FILE *, size_t *, size_t *, const char [3], int);

/* strsuftoll(3): NetBSD libc size-suffix parser (strsuftoll.c in this dir). */
long long strsuftoll(const char *, const char *, long long, long long);
long long strsuftollx(const char *, const char *, long long, long long,
	    char *, size_t);

/* BSD signal-mask API (sigstub.c in this dir). */
int	sigblock(int);
int	sigsetmask(int);
int	sigmask(int);
int	raise_default_signal(int);

/* BSD flock(2) emulated on fcntl(2) record locks (sigstub.c). */
int	flock(int, int);

/* BSD openpty(3) emulated on posix_openpt(3) (sigstub.c). */
int	openpty(int *, int *, char *, const struct termios *,
	    const struct winsize *);

/* libutil ttymsg(3) (ttymsg.c in this dir). */
char *	ttymsg(struct iovec *, int, const char *, int);

/* ---- NetBSD misc constants musl lacks (values from NetBSD headers) ---- */
#ifndef MAXSEGSIZE		/* tftp: default transfer segment size */
#define	MAXSEGSIZE	512
#endif
#ifndef TIMER_RELTIME		/* flock(1) timer_settime(2) flag */
#define	TIMER_RELTIME	0
#endif
#define	timespecclear(tsp)	((tsp)->tv_sec = 0, (tsp)->tv_nsec = 0)
#ifndef TM_YEAR_BASE		/* calendar/date year base */
#define	TM_YEAR_BASE	1900
#endif
#ifndef MAXLOGNAME		/* newsyslog */
#define	MAXLOGNAME	8
#endif
/* tftp(1): musl <arpa/tftp.h> has SEGSIZE(512) but no PKTSIZE/OACK. */
#ifndef PKTSIZE
#define	PKTSIZE		516	/* SEGSIZE + 4 header bytes */
#endif
#ifndef OACK
#define	OACK		6
#endif
#ifndef EOPTNEG
#define	EOPTNEG		8
#endif
#ifndef MAXPKTSIZE		/* tftp: largest packet size we can receive */
#define	MAXPKTSIZE	1024
#endif
/* BSD ^T status signal; musl has no SIGINFO. */
#ifndef SIGINFO
#define	SIGINFO		SIGUSR1
#endif
/* musl only declares strcasestr under _GNU_SOURCE; NetBSD uses it plainly. */
char *strcasestr(const char *, const char *);
/* BSD regex flags that musl's <regex.h> does not define.  musl's
 * regcomp/regexec do not validate the flag bits (REG_STARTEND is honoured),
 * so defining these is safe; REG_NOSPEC degrades to a normal regex. */
#ifndef REG_STARTEND
#define	REG_STARTEND	0x40000000
#endif
#ifndef REG_NOSPEC
#define	REG_NOSPEC	0x10000000
#endif
/* NetBSD snprintf_ss(3) family: return -1 when output is truncated.
 * Implemented in compat/sigstub.c. */
#include <stdarg.h>
int snprintf_ss(char *, size_t, const char *, ...);
int vsnprintf_ss(char *, size_t, const char *, va_list);
int vsnprintf_ssm(char *, size_t, const char *, va_list);

/* tftp(1): musl's struct sockaddr has no sa_len; compute the real size. */
static inline socklen_t
flx_sa_len(const struct sockaddr *sa)
{
	switch (sa->sa_family) {
	case AF_INET:
		return sizeof(struct sockaddr_in);
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
	default:
		return sizeof(struct sockaddr);
	}
}
/* vndcompress fsync_range(2) "how" constants (NetBSD <sys/vnode.h>). */
#define	FFILESYNC	0x0001
#define	FDISKSYNC	0x0002
/* glibc-style byte-swaps; musl only exposes __bswap16/32/64. */
#ifndef bswap16
#define	bswap16(x)	__bswap16(x)
#endif
#ifndef bswap32
#define	bswap32(x)	__bswap32(x)
#endif
#ifndef bswap64
#define	bswap64(x)	__bswap64(x)
#endif

/* libutil login_tty(3) (sigstub.c). */
int	login_tty(int);
/* libc strtonum(3) (sigstub.c). */
long long strtonum(const char *, long long, long long, const char **);
/* libutil uid_from_user(3)/gid_from_group(3) (sigstub.c). */
int	uid_from_user(const char *, uid_t *);
int	gid_from_group(const char *, gid_t *);
/* NetBSD fsync_range(2) -> fdatasync(2) (sigstub.c). */
int	fsync_range(int, int, off_t, off_t);


/* BSD <sys/time.h> conversion macro (musl lacks it). */
#ifndef TIMESPEC_TO_TIMEVAL
#define	TIMESPEC_TO_TIMEVAL(tv, ts)					\
	((tv)->tv_sec = (ts)->tv_sec, (tv)->tv_usec = (ts)->tv_nsec / 1000)
#endif

/* NetBSD <sys/cdefs.h> unused-parameter marker (musl lacks __USE). */
#ifndef __USE
#define	__USE(x)	(void)(x)
#endif

/*
 * NetBSD <nls.h> catalog structs used by usr.bin/gencat.  musl's
 * nl_types is a trivial no-op, so these only need to be internally
 * consistent for gencat's reader/writer (the magic is compared against
 * itself on both sides).
 */
#ifndef _NLS_MAGIC
#define	_NLS_MAGIC	0x00040010

struct _nls_cat_hdr {
	int32_t	__magic;
	int32_t	__nsets;
	int32_t	__mem;
	int32_t	__msg_hdr_offset;
	int32_t	__msg_txt_offset;
};

struct _nls_set_hdr {
	int32_t	__setno;
	int32_t	__nmsgs;
	int32_t	__index;
	int32_t	__mem;
	int32_t	__msg_hdr_offset;
};

struct _nls_msg_hdr {
	int32_t	__msgno;
	int32_t	__msglen;
	int32_t	__offset;
};
#endif /* !_NLS_MAGIC */

/* BSD stdio wide-line reader (musl lacks fgetwln(3)) (fgetwln.c). */
wchar_t	*fgetwln(FILE *, size_t *);
/* RFC-822 header-line detector (wide) as libc provides for fmt (fgetwln.c). */
int	ishead(const wchar_t *);

/* Sun-style DES front-end (bdes) backed by OpenSSL libcrypto (des_sun.c). */
int	des_setkey(char *);
int	des_cipher(const char *, char *, long, int);

/* NetBSD tcsetattr(3) flag (musl lacks TCSASOFT); musl ignores extra bits. */
#ifndef TCSASOFT
#define	TCSASOFT	0x0010
#endif

/* libutil sockaddr_snprintf(3) (sigstub.c). */
int	sockaddr_snprintf(char *, size_t, const char *, const struct sockaddr *);
/* BSD passwd/group stay-open knobs (musl lacks them) (sigstub.c). */
int	setpassent(int);
int	setgroupent(int);

/* cget(3)/getcap(3) family, provided by libtinfo (getent uses these). */
int	cgetent(char **, char **, const char *);
int	cgetfirst(char **, char **);
int	cgetnext(char **, char **);
int	cgetclose(void);
char	*cgetcap(char *, const char *, int);
int	cgetstr(char *, const char *, char **);
int	cgetnum(char *, const char *, long *);
int	cgetustr(char *, const char *, char **);
void	csetexpandtc(int);

/* <paths.h> supply missing from musl (getent). */
#ifndef _PATH_GETTYTAB
#define	_PATH_GETTYTAB	"/etc/gettytab"
#endif
#ifndef _PATH_PRINTCAP
#define	_PATH_PRINTCAP	"/etc/printcap"
#endif

/* BSD protos <sys/stat.h> supplies (tcopy, and others). */
#ifndef DEFFILEMODE
#define	DEFFILEMODE	00666
#endif

/* Default tape device (tcopy). */
#ifndef _PATH_DEFTAPE
#define	_PATH_DEFTAPE	"/dev/nrst0"
#endif

/* BSD <sys/cdefs.h> noreturn marker (tcopy). */
#ifndef __dead
#define	__dead		__attribute__((noreturn))
#endif

/* Device block address type BSD exports via <sys/mtio.h> (tcopy);
 * musl leaves it undefined. */
#ifndef _BSD_DADDR_T_
#define	_BSD_DADDR_T_
typedef long	daddr_t;
#endif

/* BSD <limits.h> password length (pwhash). */
#ifndef _PASSWORD_LEN
#define	_PASSWORD_LEN	128
#endif

/* musl lacks the BSD EFTYPE errno (pwhash). */
#ifndef EFTYPE
#define	EFTYPE	79
#endif

/* Hostname length bound as <netdb.h>-adjacent BSD headers provide. */
#ifndef MAXHOSTNAMELEN
#define	MAXHOSTNAMELEN	256
#endif

/* musl lacks the BSD SA_NOKERNINFO sigaction(2) flag (ping).  Linux ignores
 * it; 0 keeps the flag OR-chain no-op. */
#ifndef SA_NOKERNINFO
#define	SA_NOKERNINFO	0
#endif

/* BSD <stdint.h> uquad max (quota). */
#ifndef UQUAD_MAX
#define	UQUAD_MAX	ULLONG_MAX
#endif

/* dd: NetBSD open(2) O_* flags musl lacks.  The four without a Linux
 * analogue map to 0 so dd accepts the option names; Linux open(2) rejects
 * unknown flag bits, so a real mapping could only fail anyway. */
#ifndef O_EXLOCK
#define	O_EXLOCK	0
#endif
#ifndef O_SHLOCK
#define	O_SHLOCK	0
#endif
#ifndef O_NOSIGPIPE
#define	O_NOSIGPIPE	0
#endif
#ifndef O_ALT_IO
#define	O_ALT_IO	0
#endif

/* Disk geometry as <sys/param.h> exports (quota): 512-byte blocks with the
 * NetBSD byte<->block conversion macros. */
#ifndef DEV_BSHIFT
#define	DEV_BSHIFT	9
#endif
#ifndef DEV_BSIZE
#define	DEV_BSIZE	(1 << DEV_BSHIFT)
#endif
#ifndef dbtob
#define	dbtob(x)	((x) << DEV_BSHIFT)
#endif
#ifndef btodb
#define	btodb(x)	((x) >> DEV_BSHIFT)
#endif

/* libutil dehumanize_number(3) (sigstub.c). */
int	dehumanize_number(const char *, int64_t *);
/* humanize_number(3) + HN_* flags come from compat/sys/cdefs.h (NetBSD
 * <stdlib.h> surface); do not redefine them here. */

/* BSD revoke(2) (quota): close all open references to path.  No Linux
 * analogue; a stub returning EOPNOTSUPP (sigstub.c). */
int	revoke(const char *);

/* BSD funopen(3), provided by compat/funopen.c on musl fopencookie(3)
 * (compress/zopen.c).  The seek callback signature is the BSD one; the
 * opaque cookie is the pointer compress passes in. */
FILE *	funopen(const void *,
	    int (*)(void *, char *, int),
	    int (*)(void *, const char *, int),
	    off_t (*)(void *, off_t, int),
	    int (*)(void *));

/* ---- Sweep-A additions: NetBSD spellings the tools import directly ---- */

/* NetBSD <sys/cdefs.h> "const" marker (elf2aout, login's <ttyent.h>). */
#ifndef __aconst
#define	__aconst	const
#endif

/* NetBSD <time.h> (ruptime). */
#ifndef MINSPERHOUR
#define	MINSPERHOUR	60
#endif

/* NetBSD <poll.h> INFINITE timeout (rsh). */
#ifndef INFTIM
#define	INFTIM		(-1)
#endif

/* NetBSD <machine/audioio.h>-adjacent device paths (mixerctl, videoctl). */
#ifndef _PATH_MIXER
#define	_PATH_MIXER	"/dev/mixer"
#endif
#ifndef _PATH_AUDIO0
#define	_PATH_AUDIO0	"/dev/audio"
#endif
#ifndef _PATH_VIDEO0
#define	_PATH_VIDEO0	"/dev/video0"
#endif

/* NetBSD struct utmpx record-name sizes (w).  musl's <utmpx.h> does not
 * expose the BSD UTX_* spelling. */
#ifndef UTX_USERSIZE
#define	UTX_USERSIZE	32
#endif
#ifndef UTX_LINESIZE
#define	UTX_LINESIZE	32
#endif
#ifndef UTX_HOSTSIZE
#define	UTX_HOSTSIZE	256
#endif

/* NetBSD <quota.h> idtype constants (quota, edquota). */
#ifndef QUOTA_IDTYPE_USER
#define	QUOTA_IDTYPE_USER	1
#endif
#ifndef QUOTA_IDTYPE_GROUP
#define	QUOTA_IDTYPE_GROUP	2
#endif

/* BSD control-key macro (tip, others). */
#ifndef CTRL
#define	CTRL(x)		((x)&037)
#endif

/* NetBSD PT_DUMPCORE (gcore): ask the kernel to dump a process core.  Linux
 * ptrace(2) has no analogue; the constant passes through the generic musl
 * wrapper and the call simply fails at runtime. */
#ifndef PT_DUMPCORE
#define	PT_DUMPCORE	0x404
#endif

/* NetBSD closefrom(2): close every fd >= given (tip, rsh).  Implemented in
 * compat/sigstub.c over close(2). */
int	closefrom(int);

/* NetBSD __pid_t (network headers use the OpenBSD spelling; musl defines
 * pid_t directly and no __pid_t). */
#ifndef __pid_t
typedef int	__pid_t;
#endif

/* NetBSD rcmd(3) family (rsh, rdist, telnet).  Implemented in compat/rcmd.c;
 * Linux has no privileged-source-port resolver, the connect is performed
 * unprivileged. */
int	rcmd(char **, unsigned short, const char *, const char *,
	    const char *, int *);
int	rcmd_af(char **, unsigned short, const char *, const char *,
	    const char *, int *, int);
int	iruserok(unsigned long, int, const char *, const char *);
int	ruserok(const char *, int, const char *, const char *);

/* NetBSD snprintb(3): bit-field printer (videoctl).  Implemented in
 * compat/sigstub.c. */
char *	snprintb(char *, size_t, const char *, uint64_t);

/* NetBSD warnc(3): warn(3) with an explicit code instead of errno (scmdctl).
 * Implemented in compat/sigstub.c. */
void	warnc(int, const char *, ...);

/* NetBSD extattr(2) family (usr.bin/extattr, videoctl).  musl additionally
 * has no EXTATTR_NAMESPACE_*; the FreeLinX backends map the user namespace
 * onto Linux extended attributes (sigstub.c). */
#ifndef EXTATTR_NAMESPACE_USER
#define	EXTATTR_NAMESPACE_USER	1
#endif
#ifndef EXTATTR_NAMESPACE_SYSTEM
#define	EXTATTR_NAMESPACE_SYSTEM	2
#endif
int	extattr_namespace_to_string(int, char *, size_t);
int	extattr_string_to_namespace(const char *, int *);
ssize_t	extattr_list_fd(int, int, void *, size_t);
ssize_t	extattr_list_file(const char *, int, void *, size_t);
ssize_t	extattr_list_link(const char *, int, void *, size_t);
ssize_t	extattr_get_fd(int, int, const char *, void *, size_t);
ssize_t	extattr_get_file(const char *, int, const char *, void *, size_t);
ssize_t	extattr_get_link(const char *, int, const char *, void *, size_t);
int	extattr_set_fd(int, int, const char *, const void *, size_t);
int	extattr_set_file(const char *, int, const char *, const void *, size_t);
int	extattr_set_link(const char *, int, const char *, const void *, size_t);
int	extattr_delete_fd(int, int, const char *);
int	extattr_delete_file(const char *, int, const char *);
int	extattr_delete_link(const char *, int, const char *);

/* NetBSD sysctl(3) "go away, you fool" guard (pmap) needs nothing here. */

/* NetBSD <sys/param.h> bits the nfs stat tools need (nfsstat). */
#ifndef NBBY
#define	NBBY		8
#endif

/* NetBSD struct uucred (nfs/common nfs.h uses it as a value member; the
 * kernel <nfs/nfs.h> overlay has the field decl but no type). */
#ifndef _SYS_UUCRED_H_
#define _SYS_UUCRED_H_
struct uucred {
	uid_t	cr_uid;
	gid_t	cr_gid;
	int		cr_ngroups;
	gid_t	cr_groups[NGROUPS_MAX];
};
#endif

/* NetBSD errno EJUSTRETURN (rcmd chain) - never returned by Linux glibc-isms,
 * but a bare constant the rcmd code demands. */
#ifndef EJUSTRETURN
#define	EJUSTRETURN	EINPROGRESS
#endif

#endif /* !_FREELINX_FLX_BSD_H_ */