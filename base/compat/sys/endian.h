/* FreeLinX shim: musl has no <sys/endian.h>; le/be{16,32,64}dec/enc come
 * from flx_bsd.h (force-included in every base compile). */
#ifndef _FLX_SYS_ENDIAN_H_
#define _FLX_SYS_ENDIAN_H_
#include <stdint.h>
#include <strings.h>
#endif
