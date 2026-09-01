/* FreeLinX/ports - base/compat/errc.c : BSD error-reporting family.
 *
 * NetBSD <err.h> defines errc/warnc (and v- variants): like err/warn but the
 * syscall error number is passed in explicitly instead of taken from errno.
 * musl <err.h> lacks them; the BSD semantic is to print
 *   "<progname>: <fmt>: <strerror(code)>"
 * and (for errc) exit.  Implemented against musl's <err.h> primitives, so
 * the output format matches err(3) exactly.
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

void
warnc(int code, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarnc(code, fmt, ap);
	va_end(ap);
}