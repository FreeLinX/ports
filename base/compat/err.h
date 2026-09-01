/* FreeLinX/ports - base/compat/err.h : musl <err.h> extension wrapper.
 *
 * musl's <err.h> provides err/errx/warn/warnx and their v- variants, but not
 * the NetBSD errno-taking errc(3)/warnc(3) family (or their v- forms).
 * NetBSD sources (nc, diff, dc, ...) call errc()/warnc() to emit a message
 * with an explicit errno value.  This header is found first (the FreeLinX
 * compat dir leads the include path), pulls in the real musl <err.h>, then
 * declares the four BSD additions, implemented in compat/errc.c.
 */
#ifndef _FREELINX_COMPAT_ERR_H_
#define _FREELINX_COMPAT_ERR_H_

#include_next <err.h>

#include <sys/cdefs.h>
#include <stdarg.h>

__BEGIN_DECLS
void		verrc(int, int, const char *_fmt, va_list) __printflike(3, 0);
void		errc(int, int, const char *_fmt, ...) __dead __printflike(3, 4);
void		vwarnc(int, const char *_fmt, va_list) __printflike(2, 0);
void		warnc(int, const char *_fmt, ...) __printflike(2, 3);
__END_DECLS

#endif /* !_FREELINX_COMPAT_ERR_H_ */