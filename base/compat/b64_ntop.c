/* FreeLinX/ports - base/compat/b64_ntop.c : BIND b64_ntop(3)/b64_pton(3).
 *
 * The classic BIND libresolv base64 codec (b64_ntop / b64_pton), ISC/BSD
 * heritage, provided because musl's <resolv.h> omits them but NetBSD's nc
 * (socks.c) needs b64_ntop() to build HTTP-Proxy Basic authorization
 * headers.  Service-disabled by default in musl.  Declared in
 * compat/resolv.h.
 */

#include <stdlib.h>
#include <string.h>

static const char Base64[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Pad64 = '=';

/* (From RFC1521 and draft-ietf-dnssec-secext-03.txt)
 * base64 encode dst into src with size src_size (bytes), using padding. */
int
b64_ntop(const unsigned char *src, size_t src_size, char *dst, size_t dst_size)
{
	size_t i, olen;
	unsigned char *dstp;

	if (src_size == 0)
		return 0;

	olen = 4 * ((src_size + 2) / 3);
	if (dst_size <= olen)
		return -1;
	if (olen + 1 > dst_size)
		return -1;

	dstp = (unsigned char *)dst;
	for (i = 0; i < src_size - 2; i += 3) {
		*dstp++ = Base64[src[i] >> 2];
		*dstp++ = Base64[((src[i] & 0x03) << 4) | (src[i + 1] >> 4)];
		*dstp++ = Base64[((src[i + 1] & 0x0f) << 2) | (src[i + 2] >> 6)];
		*dstp++ = Base64[src[i + 2] & 0x3f];
	}
	if (i < src_size) {
		*dstp++ = Base64[src[i] >> 2];
		if (i == src_size - 1) {
			*dstp++ = Base64[(src[i] & 0x03) << 4];
			*dstp++ = Pad64;
		} else {
			*dstp++ = Base64[((src[i] & 0x03) << 4) |
			    (src[i + 1] >> 4)];
			*dstp++ = Base64[(src[i + 1] & 0x0f) << 2];
		}
		*dstp++ = Pad64;
	}
	*dstp = '\0';
	return (int)(dstp - (unsigned char *)dst);
}

static const char Base64Digits[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* base64 decode.  Returns length on success, -1 on bad input. */
int
b64_pton(const char *src, unsigned char *target, size_t targsize)
{
	int tarindex = 0;
	int state = 0;
	int ch;
	const char *pos;

	while ((ch = *src++) != '\0') {
		if (ch == Pad64)
			break;

		pos = strchr(Base64Digits, ch);
		if (pos == NULL)			/* Invalid char */
			return -1;

		ch = (int)(pos - Base64Digits);

		switch (state) {
		case 0:
			if (target) {
				if ((size_t)tarindex >= targsize)
					return -1;
				target[tarindex] = (ch << 2);
			}
			state = 1;
			break;
		case 1:
			if (target) {
				if ((size_t)tarindex + 1 >= targsize)
					return -1;
				target[tarindex] |= ch >> 4;
				target[tarindex + 1] = (ch & 0x0f) << 4;
			}
			tarindex++;
			state = 2;
			break;
		case 2:
			if (target) {
				if ((size_t)tarindex + 1 >= targsize)
					return -1;
				target[tarindex] |= ch >> 2;
				target[tarindex + 1] = (ch & 0x03) << 6;
			}
			tarindex++;
			state = 3;
			break;
		case 3:
			if (target) {
				if ((size_t)tarindex >= targsize)
					return -1;
				target[tarindex] |= ch;
			}
			tarindex++;
			state = 0;
			break;
		}
	}

	/* Let's complain about padding... */
	if (state != 0)
		return -1;
	return tarindex;
}