/* FreeLinX/ports - base/compat/sys/endian.h : NetBSD byte-order shim.
 *
 * NetBSD base sources include <sys/endian.h> to obtain the
 * htole/letoh/htobe/betoh conversion family and the BYTE_ORDER macros.
 * musl provides all of those through <endian.h>; this wrapper (found via
 * the compat include path) forwards to it.
 */
#ifndef _FREELINX_SYS_ENDIAN_H_
#define _FREELINX_SYS_ENDIAN_H_

#include <endian.h>

#endif /* !_FREELINX_SYS_ENDIAN_H_ */