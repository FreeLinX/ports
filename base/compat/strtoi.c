/* FreeLinX/ports - base/compat/strtoi.c : NetBSD portable strtoi(3).
 *
 * Taken verbatim (minus the HAVE_STRTOI configure guard) from NetBSD's
 * external/bsd/blocklist/port/strtoi.c (DragonFly/Citrus Project, BSD
 * licensed); musl has no strtoi.  Uses the _strtoi.h template.
 */
#include <sys/cdefs.h>
__RCSID("$NetBSD: strtoi.c FreeLinX $");

#include <stddef.h>
#include <errno.h>
#include <inttypes.h>

#define	_FUNCNAME	strtoi
#define	__TYPE		intmax_t
#define	__WRAPPED	strtoimax

#include "_strtoi.h"
