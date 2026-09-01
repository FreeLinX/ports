/* FreeLinX/ports - base/compat/stdio.h : musl <stdio.h> extension wrapper.
 *
 * musl ships fopencookie(3) but not the BSD funopen(3) helper, which
 * NetBSD's compress (zopen.c) uses to layer its LZW decompressor on a
 * stdio stream driven by read/write/seek/close callbacks.  This wrapper
 * pulls in the real musl <stdio.h>, declares funopen(3), and
 * compat/funopen.c implements it in terms of fopencookie(3) so the port's
 * <-style callbacks compile and work unchanged.
 */
#ifndef _FREELINX_COMPAT_STDIO_H_
#define _FREELINX_COMPAT_STDIO_H_

#include_next <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
FILE	*funopen(const void *,
	    int (*)(void *, char *, int),
	    int (*)(void *, const char *, int),
	    off_t (*)(void *, off_t, int),
	    int (*)(void *));
#ifdef __cplusplus
}
#endif

#endif /* !_FREELINX_COMPAT_STDIO_H_ */