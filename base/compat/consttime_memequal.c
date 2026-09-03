/* consttime_memequal.c — constant-time memory comparison for musl/FreeLinX */
#include <string.h>

int
consttime_memequal(const void *b1, const void *b2, size_t len)
{
	const unsigned char *p1 = b1, *p2 = b2;
	unsigned char result = 0;

	for (; len > 0; len--)
		result |= *p1++ ^ *p2++;

	return (result == 0);
}
