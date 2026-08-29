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