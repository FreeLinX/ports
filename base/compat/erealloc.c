/* FreeLinX/ports - base/compat/erealloc.c : musl err(1) wrappers.
 *
 * NetBSD utilities (e.g. unexpand) call erealloc/emalloc/ecalloc/estrdup from
 * <libutil.h> to get abort-on-failure heap allocations.  musl has no err(3)
 * alloc wrappers, so FreeLinX provides them.  Each is a thin, BSD-compatible
 * layer over malloc/realloc/strdup + err(1).
 */
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

void *
emalloc(size_t n)
{
	void *p;

	if ((p = malloc(n)) == NULL)
		err(1, "malloc %zu", n);
	return p;
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if ((p = calloc(nmemb, size)) == NULL)
		err(1, "calloc %zu", nmemb * size);
	return p;
}

void *
erealloc(void *p, size_t n)
{
	if ((p = realloc(p, n)) == NULL)
		err(1, "realloc %zu", n);
	return p;
}

char *
estrdup(const char *s)
{
	char *p;

	if ((p = strdup(s)) == NULL)
		err(1, "strdup");
	return p;
}

char *
estrndup(const char *s, size_t n)
{
	char *p;

	if ((p = strndup(s, n)) == NULL)
		err(1, "strndup");
	return p;
}