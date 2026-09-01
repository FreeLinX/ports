/* FreeLinX/ports - base/compat/netinet/ip.h : musl <netinet/ip.h> extension.
 *
 * musl's <netinet/ip.h> defines IPTOS_DSCP_MASK, IPTOS_DSCP(x), the AF*
 * DiffServ codepoints and EF, but NOT the CS0..CS7 class-selector values.
 * NetBSD's nc (map_tos) tables "cs0".."cs7" against IPTOS_DSCP_CS0..CS7.
 * This wrapper is found first (compat dir leads the include path), pulls in
 * the real musl header, then adds the six missing class-selector constants
 * (RFC 2474), same values NetBSD gives them.
 */
#ifndef _FREELINX_COMPAT_NETINET_IP_H_
#define _FREELINX_COMPAT_NETINET_IP_H_

#include_next <netinet/ip.h>

#ifndef IPTOS_DSCP_CS0
#define	IPTOS_DSCP_CS0		0x00	/* class selector 0 */
#define	IPTOS_DSCP_CS1		0x08	/* class selector 1 */
#define	IPTOS_DSCP_CS2		0x10	/* class selector 2 */
#define	IPTOS_DSCP_CS3		0x18	/* class selector 3 */
#define	IPTOS_DSCP_CS4		0x20	/* class selector 4 */
#define	IPTOS_DSCP_CS5		0x28	/* class selector 5 */
#define	IPTOS_DSCP_CS6		0x30	/* class selector 6 */
#define	IPTOS_DSCP_CS7		0x38	/* class selector 7 */
#endif

#endif /* !_FREELINX_COMPAT_NETINET_IP_H_ */