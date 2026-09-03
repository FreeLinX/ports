/* estrlcpy.c — strlcpy that exits on truncation for musl/FreeLinX */
#include <err.h>
#include <string.h>

size_t
estrlcpy(char *dst, const char *src, size_t dsize)
{
	size_t len = strlen(src);

	if (len >= dsize) {
		err(1, "estrlcpy: string too long");
	}
	memcpy(dst, src, len + 1);
	return len;
}
