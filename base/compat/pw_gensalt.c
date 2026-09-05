/*
 * FreeLinX/ports - base/compat/pw_gensalt.c : pw_gensalt(3).
 *
 * NetBSD spells the salt/rounds prep for crypt(3) in libc via
 * pw_gensalt(salt, saltlen, alg, rnd).  FreeLinX supports the styles
 * the bundled musl libcrypt provides:
 *
 *   "md5"        -> "$1$..."    (crypt-md5)
 *   "des"/""     -> 2-char SALT  (classic crypt)
 *
 * The strokes NetBSD enables at build time (blowfish, sha1, argon2)
 * are not in musl's libcrypt, so those report EOPNOTSUPP and the
 * caller's "Cannot generate salt" path fires, mirroring a default
 * FreeLinX configuration.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/random.h>
#include <util.h>

#define	_PW_ALPHABET	"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
			    "abcdefghijklmnopqrstuvwxyz"

static void
rand_bytes(char *out, size_t len)
{
	unsigned char b[16];
	size_t i;

	if (getrandom(b, sizeof(b), 0) != (ssize_t)sizeof(b)) {
		unsigned t = (unsigned)__builtin_ia32_rdtsc() * 2654435761u;
		unsigned j;

		for (j = 0; j < sizeof(b); j++) {
			t = t * 1103515245u + 12345u;
			b[j] = (unsigned char)(t >> 16);
		}
	}
	for (i = 0; i < len; i++)
		out[i] = _PW_ALPHABET[b[i % sizeof(b)] % 64];
}

int
pw_gensalt(char *salt, size_t saltlen, const char *alg, const char *rnd)
{
	char mid[16];
	size_t midlen = 8;

	(void)rnd;

	if (salt == NULL || saltlen == 0)
		return -1;
	if (alg == NULL || alg[0] == '\0' ||
	    strcmp(alg, "des") == 0 || strcmp(alg, "crypt") == 0) {
		if (saltlen < 3)
			return -1;
		rand_bytes(salt, 2);
		salt[2] = '\0';
		return 0;
	}
	if (strcmp(alg, "md5") == 0 || strcmp(alg, "md5crypt") == 0) {
		rand_bytes(mid, midlen);
		if (snprintf(salt, saltlen, "$1$%.*s$", (int)midlen, mid)
		    >= (int)saltlen)
			return -1;
		return 0;
	}
	/* blowfish, sha1, argon2: no musl libcrypt backend */
	errno = EOPNOTSUPP;
	return -1;
}