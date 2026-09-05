/*
 * FreeLinX/ports - base/compat : Sun-style DES front-end for bdes.
 *
 * NetBSD's usr.bin/bdes is written against the old 4.4BSD "libdes"
 * interface (des_setkey/des_cipher, FIPS PUB 46 + PUB 81 modes in the
 * tool itself).  musl provides no DES, so back the two entry points
 * with OpenSSL's DES implementation (static libcrypto).
 *
 *   des_setkey(char key[8])   install the 56-bit schedule from 64 key
 *   bits (parity handled by bdes); 0 on success, -1 on failure.
 *   des_cipher(in, out, salt, num_iter)  ECB one-block transform;
 *   num_iter > 0 encrypts, < 0 decrypts.  salt is unused by bdes.
 *   Returns 0 on success, -1 on failure.
 */

#include <string.h>
#include <openssl/des.h>

static DES_key_schedule bdes_sched;

int
des_setkey(char *key)
{
	DES_cblock cblock;

	memcpy(cblock, key, sizeof(cblock));
	DES_set_key_unchecked(&cblock, &bdes_sched);
	return 0;
}

int
des_cipher(const char *in, char *out, long salt, int num_iter)
{
	int enc = (num_iter > 0) ? DES_ENCRYPT : DES_DECRYPT;

	(void)salt;
	DES_ecb_encrypt((const_DES_cblock *)in, (DES_cblock *)out,
	    &bdes_sched, enc);
	return 0;
}