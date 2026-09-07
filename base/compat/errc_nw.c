/* FreeLinX/ports - base/compat/errc_nw.c : BSD error-reporting family (no warnc).
 *
 * Like errc.c, but WITHOUT warnc(3): used by ports that get warnc from
 * sigstub.c instead (xinstall.c depends on sigstub's uid/gid/flags helpers).
 * errc/vwarnc are implemented against musl's <err.h> primitives, so the
 * output format matches err(3) exactly.
 */
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void
verrc(int eval, int code, const char *fmt, va_list ap)
{
	int saved = errno;

	errno = code;
	verr(eval, fmt, ap);
	errno = saved;
}

void
errc(int eval, int code, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verrc(eval, code, fmt, ap);
	va_end(ap);
}

void
vwarnc(int code, const char *fmt, va_list ap)
{
	int saved = errno;

	errno = code;
	vwarn(fmt, ap);
	errno = saved;
}