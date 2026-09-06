/* fast_divide.c - fast_divide32(3) family for FreeLinX (NetBSD <sys/bitops.h>). */
#include <stdint.h>

static int
my_fls32(uint32_t n)
{
	int v;

	if (!n)
		return 0;
	v = 32;
	if ((n & 0xFFFF0000U) == 0) { n <<= 16; v -= 16; }
	if ((n & 0xFF000000U) == 0) { n <<= 8;  v -= 8;  }
	if ((n & 0xF0000000U) == 0) { n <<= 4;  v -= 4;  }
	if ((n & 0xC0000000U) == 0) { n <<= 2;  v -= 2;  }
	if ((n & 0x80000000U) == 0) { n <<= 1;  v -= 1;  }
	return v;
}

void
fast_divide32_prepare(uint32_t div, uint32_t *m, uint8_t *s1, uint8_t *s2)
{
	uint64_t mt;
	int l = my_fls32(div - 1);
	mt = (uint64_t)(0x100000000ULL * ((1ULL << l) - div));
	*m = (uint32_t)(mt / div + 1);
	*s1 = (l > 1) ? 1U : (uint8_t)l;
	*s2 = (l == 0) ? 0 : (uint8_t)(l - 1);
}

uint32_t
fast_divide32(uint32_t v, uint32_t div, uint32_t m, uint8_t s1, uint8_t s2)
{
	uint32_t t = (uint32_t)(((uint64_t)v * m) >> 32);
	return (t + ((v - t) >> s1)) >> s2;
}

uint32_t
fast_remainder32(uint32_t v, uint32_t div, uint32_t m, uint8_t s1, uint8_t s2)
{
	return v - div * fast_divide32(v, div, m, s1, s2);
}
