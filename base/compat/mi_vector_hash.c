/* mi_vector_hash.c - NetBSD mi_vector_hash(3) for FreeLinX.
 *
 * lookup3 (Bob Jenkins, public domain) as used by NetBSD common/lib/libc
 * for cdb; FIXED_SEED matches the NetBSD value so cdb databases written by
 * netbsd tools can be read and vice versa. */
#include <stdint.h>
#include <string.h>

#define FIXED_SEED 0x3bd2b6b0

static uint32_t
rot(uint32_t x, int k)
{
	return (x << k) | (x >> (32 - k));
}

static void
mix(uint32_t *a, uint32_t *b, uint32_t *c)
{
	*a -= *c; *a ^= rot(*c, 4);  *c += *b;
	*b -= *a; *b ^= rot(*a, 6);  *a += *c;
	*c -= *b; *c ^= rot(*b, 8);  *b += *a;
	*a -= *c; *a ^= rot(*c, 16); *c += *b;
	*b -= *a; *b ^= rot(*a, 19); *a += *c;
	*c -= *b; *c ^= rot(*b, 4);  *b += *a;
}

static void
final(uint32_t *a, uint32_t *b, uint32_t *c)
{
	*c ^= *b; *c -= rot(*b, 14);
	*a ^= *c; *a -= rot(*c, 11);
	*b ^= *a; *b -= rot(*a, 25);
	*c ^= *b; *c -= rot(*b, 16);
	*a ^= *c; *a -= rot(*c, 4);
	*b ^= *a; *b -= rot(*a, 14);
	*c ^= *b; *c -= rot(*b, 24);
}

static uint32_t
mvh_le32dec(const void *p)
{
	const uint8_t *b = p;
	return (uint32_t)b[0] | ((uint32_t)b[1] << 8) |
	    ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

void
mi_vector_hash(const void * __restrict key, size_t len, uint32_t seed,
    uint32_t hashes[3])
{
	const uint8_t *k = key;
	uint32_t a = FIXED_SEED, b = FIXED_SEED, c = seed;
	uint32_t orig_len = (uint32_t)len;

	while (len > 12) {
		a += mvh_le32dec(k);
		b += le32dec(k + 4);
		c += le32dec(k + 8);
		mix(&a, &b, &c);
		k += 12;
		len -= 12;
	}
	c += orig_len;
	switch (len) {
	case 12: b += le32dec(k + 4); a += mvh_le32dec(k); break;
	case 11: c += (uint32_t)k[10] << 24; /* FALLTHROUGH */
	case 10: c += (uint32_t)k[9] << 16;  /* FALLTHROUGH */
	case 9:  c += (uint32_t)k[8] << 8;   /* FALLTHROUGH */
	case 8:  b += le32dec(k + 4); a += mvh_le32dec(k); break;
	case 7:  b += (uint32_t)k[6] << 16; /* FALLTHROUGH */
	case 6:  b += (uint32_t)k[5] << 8;  /* FALLTHROUGH */
	case 5:  b += k[4];                 /* FALLTHROUGH */
	case 4:  a += mvh_le32dec(k); break;
	case 3:  a += (uint32_t)k[2] << 16; /* FALLTHROUGH */
	case 2:  a += (uint32_t)k[1] << 8;  /* FALLTHROUGH */
	case 1:  a += k[0];
	default: break;
	}
	final(&a, &b, &c);
	hashes[0] = a;
	hashes[1] = b;
	hashes[2] = c;
}
