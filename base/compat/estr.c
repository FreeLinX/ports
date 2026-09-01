/*
 * FreeLinX/ports - base/compat : musl stand-in for the BSD `estr` family.
 *
 * NetBSD's <util.h> declares the emalloc(3)/ecalloc(3)/erealloc(3)/
 * estrdup(3)/estrndup(3) helpers, and its tools call them liberally.  musl
 * provides neither <util.h> nor these functions; base utilities built from
 * the NetBSD src set (column, etc.) need them.  This file implements the
 * NetBSD behaviour exactly: on allocation failure it aborts via err(3),
 * never returning NULL.
 *
 * See base/compat/util.h for the FreeLinX stand-in <util.h> this pairs with.
 */

#include <sys/types.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

void *
emalloc(size_t n)
{
	void *p;

	if ((p = malloc(n)) == NULL)
		err(1, NULL);
	return p;
}

void *
ecalloc(size_t n, size_t s)
{
	void *p;

	if ((p = calloc(n, s)) == NULL)
		err(1, NULL);
	return p;
}

void *
erealloc(void *p, size_t n)
{
	if ((p = realloc(p, n)) == NULL)
		err(1, NULL);
	return p;
}

char *
estrdup(const char *s)
{
	char *p;

	if ((p = strdup(s)) == NULL)
		err(1, NULL);
	return p;
}

char *
estrndup(const char *s, size_t n)
{
	char *p;

	if ((p = strndup(s, n)) == NULL)
		err(1, NULL);
	return p;
}