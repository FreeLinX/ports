/*
 * FreeLinX compatibility header for NetBSD-derived network utilities.
 *
 * NetBSD's ping includes <netinet/ip_var.h>, but it does not use any of the
 * kernel-private declarations from that header.  Linux/musl deliberately has
 * no equivalent public header, so an empty guarded shim is sufficient.
 */
#ifndef _FREELINX_COMPAT_NETINET_IP_VAR_H_
#define _FREELINX_COMPAT_NETINET_IP_VAR_H_

#endif /* _FREELINX_COMPAT_NETINET_IP_VAR_H_ */
