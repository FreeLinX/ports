/* estrlcpy.c - strlcpy that err(1)s on truncation for musl/FreeLinX. */
#include <err.h>
#include <string.h>

size_t
estrlcpy(char *dst, const char *src, size_t dsize)
{
	size_t sl = strlen(src);
	size_t cp = sl >= dsize ? dsize - 1 : sl;
	if (dsize == 0)
		errx(1, "estrlcpy: zero-sized buffer");
	memcpy(dst, src, cp);
	dst[cp] = '\0';
	if (sl >= dsize)
		errx(1, "estrlcpy: string too long");
	return sl;
}
