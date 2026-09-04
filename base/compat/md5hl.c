/* FreeLinX/ports - base/compat/md5hl.c : md5hl wrappers (NetBSD libc API).
 *
 * MD5End/MD5File/MD5Data, the classic BSD high-level helpers over the
 * MD5_CTX API in md5c.c.  Used by usr.bin/xinstall for `-m md5`.
 */

#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	HEX_DIGITS	"0123456789abcdef"

static char *
MD5toa(const unsigned char digest[MD5_DIGEST_LENGTH], char *buf)
{
	int i, j;
	static const char hex[] = HEX_DIGITS;

	if (buf == NULL) {
		buf = malloc(MD5_DIGEST_STRING_LENGTH);
		if (buf == NULL)
			return NULL;
	}
	for (i = 0, j = 0; i < MD5_DIGEST_LENGTH; i++) {
		buf[j++] = hex[(int)(digest[i] >> 4)];
		buf[j++] = hex[(int)(digest[i] & 0xf)];
	}
	buf[j] = '\0';
	return buf;
}

char *
MD5End(MD5_CTX *ctx, char *buf)
{
	unsigned char digest[MD5_DIGEST_LENGTH];
	char *p;

	MD5Final(digest, ctx);
	p = MD5toa(digest, buf);
	if (p == NULL)
		return NULL;
	if (buf == NULL) {
		/* duplicate into a buffer the caller must free */
		char *dup = strdup(p);
		free(p);
		return dup;
	}
	return p;
}

char *
MD5File(const char *filename, char *buf)
{
	MD5_CTX ctx;
	unsigned char data[1024];
	unsigned char digest[MD5_DIGEST_LENGTH];
	FILE *f;
	int n;
	char *p;

	f = fopen(filename, "rb");
	if (f == NULL)
		return NULL;
	MD5Init(&ctx);
	while ((n = fread(data, 1, sizeof(data), f)) > 0)
		MD5Update(&ctx, data, (size_t)n);
	fclose(f);
	MD5Final(digest, &ctx);
	p = MD5toa(digest, buf);
	if (p == NULL)
		return NULL;
	if (buf == NULL) {
		char *dup = strdup(p);
		free(p);
		return dup;
	}
	return p;
}

char *
MD5Data(const void *data, unsigned int len, char *buf)
{
	MD5_CTX ctx;
	unsigned char digest[MD5_DIGEST_LENGTH];
	char *p;

	MD5Init(&ctx);
	MD5Update(&ctx, data, len);
	MD5Final(digest, &ctx);
	p = MD5toa(digest, buf);
	if (p == NULL)
		return NULL;
	if (buf == NULL) {
		char *dup = strdup(p);
		free(p);
		return dup;
	}
	return p;
}