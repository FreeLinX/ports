/* FreeLinX/ports - base/compat/resolv.h : musl <resolv.h> extension wrapper.
 *
 * Found first because the FreeLinX compat dir leads the include path.
 * Pulls in the real musl <resolv.h>, then adds the base64 helpers NetBSD's
 * nc uses for HTTP-Proxy Basic auth and musl omits:
 *   b64_ntop(3)/b64_pton(3)  (BIND-style base64 encode/decode)
 * Implemented in compat/b64_ntop.c with the classic BIND algorithm (public
 * domain / ISC heritage).
 */
#ifndef _FREELINX_COMPAT_RESOLV_H_
#define _FREELINX_COMPAT_RESOLV_H_

#include_next <resolv.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
int	b64_ntop(const unsigned char *, size_t, char *, size_t);
int	b64_pton(const char *, unsigned char *, size_t);
#ifdef __cplusplus
}
#endif

#endif /* !_FREELINX_COMPAT_RESOLV_H_ */