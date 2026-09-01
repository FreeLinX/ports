/* FreeLinX/ports - base/compat/easprintf.c : BSD libutil easprintf(3).
 *
 * NetBSD's libutil easprintf(3) is asprintf(3) that aborts on allocation
 * failure (calls err(1) with a malloc message); NetBSD hexdump's odsyntax.c
 * builds its printf formats through it.  musl has asprintf but no
 * err(1)-wrapping variant, so FreeLinX provides this thin wrapper.
 */
#include <err.h>
#include <stdarg.h>
#include <stdio.h>

int
easprintf(char ** __restrict ret, const char * __restrict fmt, ...)
{
	va_list ap;
	int rv;

	va_start(ap, fmt);
	rv = vasprintf(ret, fmt, ap);
	va_end(ap);

	if (rv == -1)
		err(1, "vasprintf");

	return rv;
}