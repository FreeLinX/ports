/*
 * FreeLinX/ports - base/compat : musl <net/if_ether.h> stand-in.
 *
 * musl provides no <net/if_ether.h>; usr.bin/getent includes it for
 * the ethers database.  Minimum definitions matching the BSD header
 * contract for what getent uses.
 */

#ifndef _FREELINX_COMPAT_NET_IF_ETHER_H_
#define _FREELINX_COMPAT_NET_IF_ETHER_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef ETHER_ADDR_LEN
#define	ETHER_ADDR_LEN	6
#endif

struct ether_addr {
	uint8_t	ether_addr_octet[ETHER_ADDR_LEN];
};

struct sockaddr_ether {
	uint8_t	se_len;
	uint8_t	se_family;
	struct ether_addr se_ether;
};

#define	ETHERTYPE_PUP		0x0200
#define	ETHERTYPE_IP		0x0800
#define	ETHERTYPE_ARP		0x0806
#define	ETHERTYPE_REVARP	0x8035
#define	ETHERTYPE_VLAN		0x8100
#define	ETHERTYPE_IPV6		0x86dd

/* ethernet address API (musl lacks these) (ethernet.c). */
struct ether_addr	*ether_aton(const char *);
char			*ether_ntoa(const struct ether_addr *);
int			ether_hostton(const char *, struct ether_addr *);
int			ether_ntohost(char *, const struct ether_addr *);

#endif /* !_FREELINX_COMPAT_NET_IF_ETHER_H_ */