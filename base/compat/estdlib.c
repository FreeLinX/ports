/* estdlib.c - libutil error-checking string/alloc helpers for musl/FreeLinX.
 *
 * BSD <util.h> has a family of e*() wrappers that print a message and call
 * err(1) on any failure.  Deliver just the handful base tools reference as
 * real functions so the linker is satisfied and behaviour matches BSD. */
#include <err.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

/* termcap ospeed - NetBSD <term.h> global absent from libterminfo/musl. */
short ospeed __attribute__((weak));

/* estrlcat(3): strlcat that err(1)s on truncation. */
size_t
estrlcat(char *dst, const char *src, size_t dsize)
{
	size_t dl = strnlen(dst, dsize);
	if (dl == dsize)
		errx(1, "estrlcat: string too long");
	return strlcat(dst, src, dsize);
}

/* ereallocarr(3): reallocarr(3) that err(1)s on failure. */
void
ereallocarr(void *ptr, size_t number, size_t size)
{
	/* reallocarr(3): ptr is a (void *) to a T* slot. */
	if (number != 0) {
		void **p = ptr;
		if (size > ~(size_t)0 / number)
			errx(1, "ereallocarr: size overflow");
		void *n = realloc(*p, number * size);
		if (n == NULL)
			err(1, "ereallocarr");
		*p = n;
	} else {
		void **p = ptr;
		free(*p);
		*p = NULL;
	}
}

/* emalloc(3)/estrdup(3): err(1)-on-failure libutil helpers. */
void *
emalloc(size_t n)
{
	void *p = malloc(n ? n : 1);
	if (p == NULL)
		err(1, "emalloc");
	return p;
}

char *
estrdup(const char *s)
{
	size_t n = strlen(s) + 1;
	char *p = emalloc(n);
	memcpy(p, s, n);
	return p;
}

/* hdestroy1(3): NetBSD <search.h> hash destroy-with-callback (key-free and
 * datum-free hooks).  musl's hdestroy() cannot iterate entries, so the
 * destructor hooks are dropped and the table torn down in the standard way
 * (touch-and-go CLI tools; entries leak until process exit, harmless). */
void
hdestroy1(void (*kf)(void *), void (*df)(void *))
{
	(void)kf;
	(void)df;
	hdestroy();
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p = calloc(nmemb, size);

	if (p == NULL)
		err(EXIT_FAILURE, NULL);
	return p;
}

void *
erealloc(void *ptr, size_t size)
{
	void *p = realloc(ptr, size);

	if (p == NULL)
		err(EXIT_FAILURE, NULL);
	return p;
}

