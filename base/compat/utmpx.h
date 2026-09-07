/* FreeLinX/ports - base/compat : musl <utmpx.h> wrapper.
 *
 * WHY THIS FILE EXISTS: same reason as sys/stat.h.  musl's <utmpx.h> struct
 * utmpx has a pad member literally named `__unused` (char __unused[20]),
 * which the global BSD `__unused` attribute macro from flx_bsd.h would
 * textually corrupt.  Take the macro down for the musl struct parse and
 * restore it immediately afterwards.  NetBSD's w(1)/users(1) family read
 * utmp via compat/utmpentry.c, which includes <utmpx.h>; this wrapper is
 * found first because base/compat leads the include path.
 */
#ifndef _FREELINX_COMPAT_UTMPX_H_
#define _FREELINX_COMPAT_UTMPX_H_

#ifdef __unused
#undef __unused
#endif
#include_next <utmpx.h>
#ifndef __unused
#define	__unused	__attribute__((__unused__))
#endif

#endif /* !_FREELINX_COMPAT_UTMPX_H_ */