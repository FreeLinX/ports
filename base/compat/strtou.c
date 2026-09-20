/* FreeLinX/ports - base/compat/strtou.c : NetBSD portable strtou(3).
 *
 * Same template as strtoi.c, configured for unsigned parsing; musl has no
 * strtou.  Used by usr.sbin/inetd parse code.
 */
#include <sys/cdefs.h>
__RCSID("$NetBSD: strtou.c FreeLinX $");

#include <stddef.h>
#include <errno.h>
#include <inttypes.h>

#define	_FUNCNAME	strtou
#define	__TYPE		uintmax_t
#define	__WRAPPED	strtoumax

#include "_strtoi.h"
