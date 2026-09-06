/* FreeLinX shim: musl has no <sys/bitops.h>.  The fast_divide32(3) family
 * is provided by compat/fast_divide.c; the fls(3)/homebrew helpers NetBSD
 * tools use rarely are not needed by the vendored cdb/terminfo sources. */
#ifndef _FLX_SYS_BITOPS_H_
#define _FLX_SYS_BITOPS_H_
#include <stdint.h>

void fast_divide32_prepare(uint32_t, uint32_t *, uint8_t *, uint8_t *);
uint32_t fast_divide32(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
uint32_t fast_remainder32(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
#endif
