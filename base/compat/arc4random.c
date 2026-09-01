/*
 * FreeLinX compat: uint32_t arc4random(void).
 *
 * NetBSD bin/rm's -P overwrite pass draws its pattern bytes from
 * arc4random(3).  The musl build used by FreeLinX does not ship
 * arc4random(3), so a minimal, cryptographically-sane FreeLinX version is
 * provided here on top of the Linux getrandom(2) syscall (same source
 * musl itself uses on kernels that have it).
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

uint32_t
arc4random(void)
{
	uint32_t v;
	ssize_t n;

	do {
		n = getrandom(&v, sizeof(v), 0);
	} while (n < 0 && errno == EINTR);

	if (n == (ssize_t)sizeof(v))
		return v;

	/* Kernel without getrandom(2): fall back to /dev/urandom.  getrandom
	 * on Linux has been available since 3.17 (2014), so this branch is
	 * only defensive. */
	{
		FILE *f = fopen("/dev/urandom", "r");
		if (f == NULL)
			abort();
		if (fread(&v, sizeof(v), 1, f) != 1)
			abort();
		fclose(f);
		return v;
	}
}

/* BSD arc4random_uniform(3): unbiased value in [0, upper_bound).  Uses the
 * rejection method so every value has equal probability (no modulo bias).
 * musl ships no arc4random family; nc calls arc4random_uniform() for -r
 * (random source port) selects.  Declared in compat/stdlib.h. */
uint32_t
arc4random_uniform(uint32_t upper_bound)
{
	uint32_t r, min;

	if (upper_bound < 2)
		return 0;

	/* 2**32 % upper_bound == 2**32 - upper_bound when upper_bound is a
	 * power of two; otherwise the smallest value making the remainder a
	 * multiple of upper_bound. */
	min = -upper_bound % upper_bound;

	do {
		r = arc4random();
	} while (r < min);

	return r % upper_bound;
}