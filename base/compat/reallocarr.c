/*
 * FreeLinX compat: int reallocarr(void **, size_t, size_t).
 *
 * NetBSD's lib/libc/gen/fts.c (vendored into the cp/rm/ls ports) grows its
 * FTSENT array with reallocarr(3), a NetBSD libc interface musl does not
 * provide.  This is a reimplementation of the documented contract:
 *
 *   - a zero sized allocation is freed and cleared;
 *   - a size overflow is rejected with EINVAL;
 *   - an allocation failure leaves the buffer intact and returns ENOMEM.
 */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int
reallocarr(void *ptr, size_t nmemb, size_t size)
{
	void **p = (void **)ptr;
	void *n;

	if (nmemb == 0 || size == 0) {
		free(*p);
		*p = NULL;
		return (0);
	}
	if (nmemb > SIZE_MAX / size)
		return (EINVAL);
	n = realloc(*p, nmemb * size);
	if (n == NULL)
		return (ENOMEM);
	*p = n;
	return (0);
}