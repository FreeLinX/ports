/* FreeLinX/ports - shells/netbsd-sh : musl compatibility header.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * musl deliberately does not ship a <sys/cdefs.h> (it is a BSD/glibc-ism),
 * but every NetBSD sh source includes it for the BSD compiler-attribute
 * macros below.  This is a minimal FreeLinX-provided stand-in: it defines
 * only the subset the NetBSD 10.1 sh actually uses, plus a couple of musl
 * libc gaps that BSD conventionally provides in <sys/time.h> (timespeccmp,
 * used by bin/test/test.c).
 *
 * This file is NOT copied from NetBSD wholesale; every macro below is
 * required by the source we compile or by the link set.  Keep it that way.
 */
#ifndef _FREELINX_SYS_CDEFS_H_
#define _FREELINX_SYS_CDEFS_H_

/*
 * Include the freestanding type headers here so every sh translation unit
 * gets ptrdiff_t / size_t and the fixed-width integer types it expects.
 * (NetBSD's own <sys/types.h> pulls <sys/stdint.h>; with musl, <sys/types.h>
 * holds only u_char/quad_t-style names, so these two headers stand in.)
 */
#include <stddef.h>
#include <stdint.h>
/* uid_t/gid_t for the BSD uid/gid helpers declared below. */
#include <sys/types.h>

#define __BEGIN_DECLS
#define __END_DECLS

#define __CONCAT1(x, y) x##y
#define __CONCAT(x, y) __CONCAT1(x, y)
#define __STRING(x) #x
#define __XSTRING(x) __STRING(x)

#define __P(protos) protos

/* NetBSD system headers rename libc entry points with __RENAME(x) to keep
 * binary compatibility with older struct layouts (e.g. include/fts.h maps
 * fts_open -> __fts_open60).  When those headers are used to build a static
 * app there is no ABI contract, so the rename is a plain no-op and callers
 * get the natural symbol. */
#define __RENAME(x)

#define __IDSTRING(name, string) \
	static char const name[] __attribute__((__unused__)) = string
#define __RCSID(string)		__IDSTRING(rcsid, string)
#define __COPYRIGHT(string)	__IDSTRING(copyright, string)
#define __SCCSID(string)	__IDSTRING(sccsid, string)

#define __dead			__attribute__((__noreturn__))
#define __printflike(fmtarg, firstvararg) \
	__attribute__((__format__(__printf__, fmtarg, firstvararg)))
#define __scanflike(fmtarg, firstvararg) \
	__attribute__((__format__(__scanf__, fmtarg, firstvararg)))
#define __formatarg(fmtarg)	__attribute__((__format_arg__(fmtarg)))
#define __deprecated		__attribute__((__deprecated__))
/* NOTE: no __unused define.  musl's <sys/stat.h> uses `long __unused[3]` as
 * a literal struct-member name, which a global __unused macro (empty or
 * attribute) would corrupt.  The two NetBSD sh sites that used the macro are
 * patched (patches/patch-unused-attr-musl) so the macro is not needed. */
#define __used			__attribute__((__used__))
#define __packed		__attribute__((__packed__))
#define __aligned(x)		__attribute__((__aligned__(x)))
#define __returns_twice		__attribute__((__returns_twice__))

#define __predict_true(exp)	__builtin_expect((exp), 1)
#define __predict_false(exp)	__builtin_expect((exp), 0)

#define __UNCONST(a)		((void *)(uintptr_t)(const void *)(a))
#define __UNVOLATILE(a)		((void *)(uintptr_t)(volatile void *)(a))

#define __arraycount(__x)	(sizeof(__x) / sizeof(__x[0]))

/*
 * NetBSD's <sys/param.h> defines BSD early, so its base sources (e.g.
 * bin/sh/jobs.c) can guard headers behind `#ifdef BSD` before they ever
 * reach shell.h.  musl's <sys/param.h> defines no such macro; defining it
 * here (this header is the first thing every sh source includes) restores
 * the NetBSD behaviour.  BSD4_4/__SVR4 stay undefined on purpose so the sh
 * code takes its portable paths.
 */
#ifndef BSD
#define BSD 1
#endif

/*
 * musl libc has no getprogname(3)/setprogname(3) or setproctitle(3) (BSD
 * utilities).  The NetBSD sh and its pulled-in utilities (error.c,
 * bin/kill, usr.bin/printf, jobs.c) reference them.  Every source that uses
 * them includes this header, so the prototypes live here; the
 * implementations are in compat/getprogname.c.
 */
const char *getprogname(void);
void		setprogname(const char *);
void		setproctitle(const char *, ...);

/* BSD owner/group name lookups used by cp/mv/rm/ls; musl has neither the
 * functions nor a BSD-style cache.  Implemented in compat/user_from_uid.c
 * on top of getpwuid(3)/getgrgid(3) with the NetBSD pwcache contract.
 */
const char *	user_from_uid(uid_t, int);
const char *	group_from_gid(gid_t, int);

/* strmode(3) typeset-as-string, used by ls/mv/rm; musl omits it.
 * NetBSD's own libc implementation is vendored in compat/strmode.c. */
void		strmode(mode_t, char *);

/* BSD bounded string helpers musl omits; NetBSD 10.1 libc has no portable
 * strlcpy.c either (only per-arch .S), so compat/strlcpy.c is the canonical
 * Berkeley body written for FreeLinX.  Used by cp and mv. */
size_t		strlcpy(char *, const char *, size_t);
size_t		strlcat(char *, const char *, size_t);

/* rm -P's overwrite pass draws its pattern from arc4random(3); this musl
 * build omits it, so compat/arc4random.c provides it on getrandom(2). */
uint32_t	arc4random(void);

/* NetBSD libc interface used by the vendored fts.c to grow its FTSENT
 * array; musl has no reallocarr(3), so compat/reallocarr.c supplies one
 * with the documented contract (overflow -> EINVAL, alloc failure keeps the
 * buffer and returns ENOMEM). */
int		reallocarr(void *, size_t, size_t);

/* NetBSD's <stdlib.h> declares humanize_number(3) and the HN_* scale
 * flags; musl omits them and bin/ls -h uses it.  The implementation is
 * vendored NetBSD libc (compat/humanize_number.c). */
#define	HN_DECIMAL	0x01
#define	HN_NOSPACE	0x02
#define	HN_B	0x04
#define	HN_DIVISOR_1000	0x08
#define	HN_GETSCALE	0x10
#define	HN_AUTOSCALE	0x20
int		humanize_number(char *, size_t, int64_t, const char *, int, int);

/*
 * NetBSD-only libc interfaces used by bin/sh/miscbltin.c's umask builtin
 * (symbolic mode).  The implementations are NetBSD's own libc sources,
 * shipped into this port's compat dir (compat/setmode.c).
 */
void *setmode(const char *);
unsigned int getmode(const void *, unsigned int);

/* NetBSD-only signal name/number mapping used by bin/sh/trap.c and the
 * pulled-in kill builtin; provided in compat/signalname.c (keyed to musl's
 * actual <signal.h> numbering). */
int		signalnumber(const char *);
const char *	signalname(int);
int		signalnext(int);

/* NetBSD maps _DIAGASSERT(e) to assert(3) under _DIAGNOSTIC; tsan etc. also
 * use it.  FreeLinX releases do not want the extra checks, so: no-op. */
#ifndef _DIAGASSERT
#define _DIAGASSERT(e)	((void)0)
#endif

/* NetBSD's <sys/param.h> defines ALIGNBYTES/ALIGN; musl's does not, but
 * lib/libc/gen/fts.c (built into the cp/rm/ls ports) pads the FTSENT
 * allocation with them.  Kept beside timespeccmp() for the same reason
 * (a BSD <sys/x> convention that musl omits). */
#ifndef ALIGNBYTES
#define	ALIGNBYTES	(sizeof(long) - 1)
#define	ALIGN(p)	(((unsigned long)(p) + ALIGNBYTES) &~ ALIGNBYTES)
#endif

/* NetBSD's <sys/param.h> defines MAXBSIZE (the copy chunk size used by
 * bin/cp/utils.c); musl's does not. */
#ifndef MAXBSIZE
#define	MAXBSIZE	(64 * 1024)
#endif

/* NetBSD's <stdint.h> defines SIZE_T_MAX; musl only defines SIZE_MAX, and
 * bin/ls/util.c (vis-escaped name length check) uses the BSD spelling. */
#ifndef SIZE_T_MAX
#define	SIZE_T_MAX	__SIZE_MAX__
#endif

/* bin/ls/print.c defines SIXMONTHS from the tzfile(5) constants NetBSD
 * exposes through <tzfile.h> and timezone printing; musl has neither the
 * header nor the constants. */
#ifndef DAYSPERNYEAR
#define	DAYSPERNYEAR	365
#endif
#ifndef SECSPERDAY
#define	SECSPERDAY	86400
#endif

/* BSD <sys/time.h> helper that musl lacks; required by bin/test/test.c
 * (timespeccmp(&b1.st_mtim, &b2.st_mtim, >)). */
#define timespeccmp(tsp, usp, cmp) \
	(((tsp)->tv_sec cmp (usp)->tv_sec) || \
	 ((tsp)->tv_sec == (usp)->tv_sec && \
	  (tsp)->tv_nsec cmp (usp)->tv_nsec))

#endif /* !_FREELINX_SYS_CDEFS_H_ */