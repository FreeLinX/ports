/* FreeLinX/ports - base/compat/stdlib.h : musl <stdlib.h> extension wrapper.
 *
 * Found first because the FreeLinX compat dir leads the include path.
 * Pulls in the real musl <stdlib.h> via #include_next, then declares the
 * BSD-only additions the base ports use and musl omits:
 *   - strtoi(3)/strtou(3): intentional-cast variants of strtoimax/strtoumax
 *     with bounds checking.  NetBSD declares them in <inttypes.h>
 *     (guarded by _NETBSD_SOURCE), but netcat.c calls them after including
 *     only <stdlib.h>; a compat <stdlib.h> is the one place every caller
 *     reaches.  Implemented in compat/strtoi.c.
 *   - arc4random_uniform(3): unbiased upper-bounded arc4random; musl ships
 *     no arc4random* at all (FreeLinX compat provides arc4random() in
 *     compat/arc4random.c).  Implemented in the same file.
 */
#ifndef _FREELINX_COMPAT_STDLIB_H_
#define _FREELINX_COMPAT_STDLIB_H_

#include_next <stdlib.h>

#include <sys/cdefs.h>
#include <stdint.h>

__BEGIN_DECLS
intmax_t	strtoi(const char * __restrict, char ** __restrict, int,
	    intmax_t, intmax_t, int *);
uintmax_t	strtou(const char * __restrict, char ** __restrict, int,
	    uintmax_t, uintmax_t, int *);
uint32_t	arc4random_uniform(uint32_t);
__END_DECLS

#endif /* !_FREELINX_COMPAT_STDLIB_H_ */