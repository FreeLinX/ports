/* FreeLinX/ports - base/compat : <sys/queue.h> shim.
 *
 * musl does not ship <sys/queue.h>; NetBSD userland sources (grep's STAILQ
 * ring buffer, mpool's TAILQ buckets, manconf.h, kvm getloadavg) need the
 * full BSD intrusive-list macro family.  The FreeLinX NetBSD overlay carries
 * the untouched NetBSD 10.1 sys/queue.h (nbsys/sys/sys/queue.h); this file
 * simply routes to it so -I compat wins the name and everything downstream
 * gets the complete, coherent macro set instead of a partial clone.
 */
#ifndef _FREELINX_COMPAT_SYS_QUEUE_H_
#define _FREELINX_COMPAT_SYS_QUEUE_H_

#include <sys/cdefs.h>
#include "../nbsys/sys/sys/queue.h"

#endif /* !_FREELINX_COMPAT_SYS_QUEUE_H_ */
