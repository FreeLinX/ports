/* FreeLinX/ports - base/compat/errno.h : musl <errno.h> extension wrapper.
 *
 * musl's <errno.h> is generated from the Linux errno table and omits the
 * BSD error EFTYPE ("Inappropriate file type or format", value 79 on
 * NetBSD).  NetBSD's compress (zopen.c) sets errno = EFTYPE when the magic
 * header is wrong.  musl does not use value 79, so defining it restores the
 * BSD contract without shadowing a Linux error.
 */
#ifndef _FREELINX_COMPAT_ERRNO_H_
#define _FREELINX_COMPAT_ERRNO_H_

#include_next <errno.h>

#ifndef EFTYPE
#define	EFTYPE	79	/* Inappropriate file type or format */
#endif

#endif /* !_FREELINX_COMPAT_ERRNO_H_ */