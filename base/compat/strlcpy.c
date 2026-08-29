/* FreeLinX/ports - base/compat : musl strlcpy(3) for the NetBSD tools.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's cp and mv chain strings with strlcpy(3), a BSD libc function musl
 * deliberately does not provide.  NetBSD 10.1's own libc has no portable
 * strlcpy.c either (the src set carries only per-arch .S implementations), so
 * there is nothing to vendor from the release.  This is the canonical
 * Berkeley strlcpy body (identical semantics to the libbsd/OpenBSD version),
 * written for FreeLinX; it is added to cp and mv link sets.
 *
 * Returns the length of the source string; never writes past dsize-1.
 */
#include <string.h>

size_t
strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t slen = strlen(src);

	if (dsize == 0)
		return slen;
	if (slen >= dsize)
		slen = dsize - 1;
	memcpy(dst, src, slen);
	dst[slen] = '\0';
	return strlen(src);
}