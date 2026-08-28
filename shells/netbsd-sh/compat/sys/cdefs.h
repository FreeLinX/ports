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

#define __BEGIN_DECLS
#define __END_DECLS

#define __CONCAT1(x, y) x##y
#define __CONCAT(x, y) __CONCAT1(x, y)
#define __STRING(x) #x
#define __XSTRING(x) __STRING(x)

#define __P(protos) protos

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

/* BSD <sys/time.h> helper that musl lacks; required by bin/test/test.c
 * (timespeccmp(&b1.st_mtim, &b2.st_mtim, >)). */
#define timespeccmp(tsp, usp, cmp) \
	(((tsp)->tv_sec cmp (usp)->tv_sec) || \
	 ((tsp)->tv_sec == (usp)->tv_sec && \
	  (tsp)->tv_nsec cmp (usp)->tv_nsec))

#endif /* !_FREELINX_SYS_CDEFS_H_ */