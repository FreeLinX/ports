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

#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/socket.h>
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
void	csetexpandtc(int);

/* <paths.h> supply missing from musl (getent). */
#ifndef _PATH_GETTYTAB
#define	_PATH_GETTYTAB	"/etc/gettytab"
#endif
#ifndef _PATH_PRINTCAP
#define	_PATH_PRINTCAP	"/etc/printcap"
#endif

/* Hostname length bound as <netdb.h>-adjacent BSD headers provide. */
#ifndef MAXHOSTNAMELEN
#define	MAXHOSTNAMELEN	256
#endif
#endif /* !_FREELINX_FLX_BSD_H_ */