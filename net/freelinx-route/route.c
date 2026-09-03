/*
 * FreeLinX - net/freelinx-route
 *
 * A BSD-styled route for the Linux kernel, written from scratch and built only
 * with the FreeLinX clang + LLD + musl toolchain against libnl-3 (which
 * FreeLinX builds itself from source; LGPL-2.1).  It talks to the kernel over
 * AF_NETLINK (NETLINK_ROUTE) with the netlink route cache libnl wraps -- the
 * API the Linux kernel actually provides (there is no BSD AF_ROUTE on Linux,
 * so the NetBSD netbsd-route source cannot link without a netlink rewrite).
 *
 * Deliberately minimal and BSD-flavoured, mirroring the small NetBSD `route`
 * surface: no GNU long-options, no color.  Commands: show/add/delete.
 *
 * Usage (NetBSD-ish):
 *   flx-route [show]                    print the routing table
 *   flx-route add default <gateway>
 *   flx-route add -net A.B.C.D/NN <gateway>
 *   flx-route add -net A.B.C.D/NN -interface <if>
 *   flx-route add -host A.B.C.D <gateway>
 *   flx-route delete -net A.B.C.D/NN <gateway>
 *   flx-route delete default <gateway>
 *
 * License: BSD-2-Clause (FreeLinX original code).
 *
 * NOTE on libnl's opaque nexthop: struct rtnl_nexthop is private to libnl.
 * FreeLinX pins libnl to 3.9.0 (net/libnl), so we reproduce its exact layout
 * here, purely to walk the rtnh_list; we still read values through the public
 * accessors (rtnl_route_nh_get_*), never by touching fields directly.  If the
 * pinned libnl version changes, the layout below must be revisited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/addr.h>
#include <netlink/route/route.h>
#include <netlink/route/nexthop.h>
#include <netlink/list.h>

static const char *flx_progname = "flx-route";

/* --- minimal reproduction of libnl 3.9.0 struct rtnl_nexthop (for list walk) */
struct rtnl_nexthop_priv {
	uint8_t  rtnh_flags;
	uint8_t  rtnh_flag_mask;
	uint8_t  rtnh_weight;
	uint8_t  spare;
	uint32_t rtnh_ifindex;
	struct nl_addr *rtnh_gateway;
	uint32_t ce_mask;
	struct nl_list_head rtnh_list;
	uint32_t rtnh_realms;
	struct nl_addr *rtnh_newdst;
	struct nl_addr *rtnh_via;
	void    *rtnh_encap;
};

static void
usage(void)
{
	fprintf(stderr,
	    "usage: %s [show]\n"
	    "       %s add default <gateway>\n"
	    "       %s add -net A.B.C.D/NN <gateway>\n"
	    "       %s add -net A.B.C.D/NN -interface <ifname>\n"
	    "       %s add -host A.B.C.D <gateway>\n"
	    "       %s delete -net A.B.C.D/NN <gateway>\n"
	    "       %s delete default <gateway>\n",
	    flx_progname, flx_progname, flx_progname,
	    flx_progname, flx_progname, flx_progname,
	    flx_progname);
	exit(1);
}

static const char *
route_type_str(unsigned int t)
{
	switch (t) {
	case 1:  return "unicast";
	case 2:  return "local";
	case 3:  return "broadcast";
	case 4:  return "anycast";
	case 5:  return "multicast";
	case 6:  return "blackhole";
	case 7:  return "unreachable";
	case 8:  return "prohibit";
	case 9:  return "throw";
	case 10: return "nat";
	case 11: return "xresolve";
	default: return "?";
	}
}

static const char *
scope_str(unsigned int s)
{
	switch (s) {
	case RT_SCOPE_UNIVERSE: return "UNIVERSE";
	case RT_SCOPE_SITE: return "SITE";
	case RT_SCOPE_LINK: return "LINK";
	case RT_SCOPE_HOST: return "HOST";
	case RT_SCOPE_NOWHERE: return "NOWHERE";
	default: return "?";
	}
}

static const char *
table_str(unsigned int t)
{
	/* Linux routing tables (linux/rtnetlink.h RT_TABLE_*). */
	switch (t) {
	case 254: return "main";
	case 255: return "local";
	case 253: return "default";
	default: return "?";
	}
}

/* Linux kernel route scopes (linux/rtnetlink.h). */
#define RT_SCOPE_UNIVERSE	0
#define RT_SCOPE_SITE		200
#define RT_SCOPE_LINK		253
#define RT_SCOPE_HOST		254
#define RT_SCOPE_NOWHERE	255

/* --- print one route, BSD style ------------------------------------------- */
static void
route_cb(struct nl_object *obj, void *arg)
{
	struct rtnl_route *r = (struct rtnl_route *) obj;
	struct nl_addr *dst = rtnl_route_get_dst(r);
	const char *dststr = "-";
	struct nl_list_head *nh_list;
	struct rtnl_nexthop_priv *nh;
	char dstbuf[INET6_ADDRSTRLEN];
	char nbuf[INET6_ADDRSTRLEN];
	char ifbuf[16];
	int flags;
	int i = 0;

	(void)arg;
	if (dst != NULL) {
		(void)nl_addr2str(dst, dstbuf, sizeof(dstbuf));
		dststr = dstbuf;
	}

	printf("%-18s ", dststr);

	nh_list = rtnl_route_get_nexthops(r);
	if (nh_list != NULL) {
		nl_list_for_each_entry(nh, nh_list, rtnh_list) {
			struct rtnl_nexthop *pub = (struct rtnl_nexthop *) nh;
			struct nl_addr *gw;
			if (i > 0)
				printf("            "); /* continuation */
			gw = rtnl_route_nh_get_gateway(pub);
			if (gw != NULL)
				printf("%-18s ", nl_addr2str(gw, nbuf, sizeof(nbuf)));
			else
				printf("%-18s ", "-");
			i++;
		}
	}
	/* default / simple entries have a single empty nexthop list; print gateway */
	if (nh_list == NULL || i == 0) {
		printf("%-18s ", "-");
	}

	flags = rtnl_route_get_flags(r);
	printf("%s ", (flags & 0x1) ? "U" : "-");   /* RTM_F_CLONED approximate */

	printf(" %3s ", table_str(rtnl_route_get_table(r)));
	printf("%9s ", scope_str(rtnl_route_get_scope(r)));
	printf("%-9s ", route_type_str(rtnl_route_get_type(r)));

	/* interface (first nexthop ifindex) */
	if (nh_list != NULL) {
		nl_list_for_each_entry(nh, nh_list, rtnh_list) {
			struct rtnl_nexthop *pub = (struct rtnl_nexthop *) nh;
			int idx = rtnl_route_nh_get_ifindex(pub);
			if (idx > 0) {
				if (if_indextoname((unsigned)idx, ifbuf) != NULL) {
					printf("%s\n", ifbuf);
					return;
				}
			}
		}
	}
	printf("-\n");
	(void)ifbuf;
}

static int
route_show(struct nl_sock *sock)
{
	struct nl_cache *ct = NULL;
	int err;

	err = rtnl_route_alloc_cache(sock, AF_UNSPEC, 0, &ct);
	if (err < 0 || ct == NULL) {
		fprintf(stderr, "%s: unable to read routing table: %s\n",
		    flx_progname, nl_geterror(err));
		return 1;
	}
	printf("Destination       Gateway           Flags tble scope     type      if\n");
	nl_cache_foreach(ct, route_cb, NULL);
	nl_cache_put(ct);
	return 0;
}

/* --- build a route object from parsed args --------------------------------- */
struct flx_dst {
	int is_default;
	int is_host;
	char addr[INET6_ADDRSTRLEN];
	int plen;
};

static void
parse_dst(const char *tok, struct flx_dst *d)
{
	char *slash;

	d->is_default = 0;
	d->is_host = 0;
	d->plen = -1;
	d->addr[0] = '\0';

	if (strcmp(tok, "default") == 0) {
		d->is_default = 1;
		return;
	}
	slash = strchr(tok, '/');
	if (slash != NULL) {
		*slash = '\0';
		d->plen = atoi(slash + 1);
	}
	strncpy(d->addr, tok, INET6_ADDRSTRLEN - 1);
	d->addr[INET6_ADDRSTRLEN - 1] = '\0';
	if (slash != NULL)
		*slash = '/';
}

static struct rtnl_route *
make_route(const struct flx_dst *d)
{
	struct rtnl_route *r = rtnl_route_alloc();
	int family = AF_INET;

	if (r == NULL)
		return NULL;

	if (d->is_default) {
		rtnl_route_set_dst(r, nl_addr_build(AF_INET, NULL, 0));
		rtnl_route_set_family(r, AF_INET);
		return r;
	}
	if (strchr(d->addr, ':') != NULL)
		family = AF_INET6;

	rtnl_route_set_family(r, family);
	{
		struct nl_addr *dst = NULL;
		if (nl_addr_parse(d->addr, family, &dst) == 0 && dst != NULL) {
			int plen = d->plen;
			if (plen < 0) {
				if (d->is_host)
					plen = family == AF_INET6 ? 128 : 32;
				else if (strchr(d->addr, ':') != NULL)
					plen = 64;
				else
					plen = 24;
			}
			nl_addr_set_prefixlen(dst, plen);
			rtnl_route_set_dst(r, dst);
			nl_addr_put(dst);
		}
	}
	return r;
}

static struct rtnl_nexthop *
make_nh(const char *gw, int ifindex, int family)
{
	struct rtnl_nexthop *nh = rtnl_route_nh_alloc();

	if (nh == NULL)
		return NULL;
	if (ifindex > 0)
		rtnl_route_nh_set_ifindex(nh, ifindex);
	if (gw != NULL) {
		struct nl_addr *g = NULL;
		if (nl_addr_parse(gw, family, &g) == 0 && g != NULL) {
			rtnl_route_nh_set_gateway(nh, g);
			nl_addr_put(g);
		}
	}
	return nh;
}

static int
route_add_del(struct nl_sock *sock, int do_add, int is_host,
              const char *dsttok, const char *gw,
              const char *ifname)
{
	struct flx_dst d;
	struct rtnl_route *r;
	struct rtnl_nexthop *nh;
	struct nl_addr *tmp;
	int family = AF_INET;
	int ifindex = 0;
	int err;

	parse_dst(dsttok, &d);
	d.is_host = is_host;
	if (strchr(d.addr, ':') != NULL)
		family = AF_INET6;
	if (d.is_default)
		family = AF_INET; /* default => 0.0.0.0/0 */

	r = make_route(&d);
	if (r == NULL) {
		fprintf(stderr, "%s: out of memory\n", flx_progname);
		return 1;
	}

	/* parse gateway into tmp to learn family consistency (borrow nl_addr) */
	tmp = NULL;
	if (gw != NULL && nl_addr_parse(gw, AF_UNSPEC, &tmp) == 0 && tmp != NULL) {
		if (nl_addr_get_family(tmp) == AF_INET6)
			family = AF_INET6;
		nl_addr_put(tmp);
	}
	if (ifname != NULL)
		ifindex = (int)if_nametoindex(ifname);

	nh = make_nh(gw, ifindex, family);
	if (nh == NULL) {
		fprintf(stderr, "%s: out of memory\n", flx_progname);
		nl_object_put((struct nl_object *) r);
		return 1;
	}
	rtnl_route_add_nexthop(r, nh);

	/* A directly-connected route (interface only, no gateway) is LINK scope. */
	if (gw == NULL && ifindex > 0)
		rtnl_route_set_scope(r, RT_SCOPE_LINK);

	if (do_add)
		err = rtnl_route_add(sock, r, 0);
	else
		err = rtnl_route_delete(sock, r, 0);

	nl_object_put((struct nl_object *) r);
	if (err < 0) {
		fprintf(stderr, "%s: cannot %s route: %s\n", flx_progname,
		    do_add ? "add" : "delete", nl_geterror(err));
		return 1;
	}
	return 0;
}

int
main(int argc, char **argv)
{
	struct nl_sock *sock;
	int a = 1;
	int rc = 0;

	if (argc > 1 && (strcmp(argv[1], "-h") == 0 ||
	    strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0))
		usage();

	sock = nl_socket_alloc();
	if (!sock) { perror("route: nl_socket_alloc"); return 1; }
	if (nl_connect(sock, NETLINK_ROUTE) < 0) {
		perror("route: nl_connect");
		nl_socket_free(sock);
		return 1;
	}

	/* default: show */
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "show") == 0)) {
		rc = route_show(sock);
	} else if (strcmp(argv[a], "add") == 0 || strcmp(argv[a], "delete") == 0) {
		int do_add = strcmp(argv[a], "add") == 0;
		const char *dsttok = NULL;
		const char *gw = NULL;
		const char *ifname = NULL;
		int is_net = 0, is_host = 0;
		int i;

		a++;
		/* collect destination + gateway (+ optional -net/-host/-interface) */
		for (i = a; i < argc; i++) {
			if (strcmp(argv[i], "-net") == 0)   { is_net = 1; continue; }
			if (strcmp(argv[i], "-host") == 0)  { is_host = 1; continue; }
			if (strcmp(argv[i], "-interface") == 0) {
				if (i + 1 < argc) ifname = argv[++i];
				continue;
			}
			if (dsttok == NULL)
				dsttok = argv[i];
			else if (gw == NULL)
				gw = argv[i];
		}
		if (dsttok == NULL) usage();
		if (!is_net && !is_host && strcmp(dsttok, "default") != 0) {
			/* treat a bare address as a host route (NetBSD default) */
			is_host = 1;
		}
		rc = route_add_del(sock, do_add, is_host, dsttok, gw, ifname);
	} else {
		usage();
	}

	nl_socket_free(sock);
	return rc;
}
