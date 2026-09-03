/* estrdup/emalloc — BSD error-checking allocators for musl/FreeLinX */
#include <err.h>
#include <stdlib.h>
#include <string.h>

char *
estrdup(const char *str)
{
	char *p = strdup(str);
	if (p == NULL)
		err(1, "strdup");
	return p;
}

void *
emalloc(size_t size)
{
	void *p = malloc(size);
	if (p == NULL)
		err(1, "malloc");
	return p;
}
