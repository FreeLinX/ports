/*
 * FreeLinX - net/freelinx-ifconfig
 *
 * A BSD-styled ifconfig for the Linux kernel, written from scratch and built
 * only with the FreeLinX clang + LLD + musl toolchain against libnl-3 (which
 * FreeLinX builds itself from source; LGPL-2.1).  It talks to the kernel over
 * AF_NETLINK (NETLINK_ROUTE) with the netlink route/link/addr caches that
 * libnl wraps -- the API the Linux kernel actually provides (there is no BSD
 * AF_ROUTE on Linux).
 *
 * Deliberately minimal and BSD-flavoured: no GNU long-options, no color.
 * Just interface enumeration and the handful of mutating verbs a bring-up
 * needs (up/down, set address, set mtu).
 *
 * Usage (NetBSD-ish):
 *   flx-ifconfig             list all interfaces with addresses
 *   flx-ifconfig <if>        show one interface
 *   flx-ifconfig <if> up     bring an interface up
 *   flx-ifconfig <if> down   bring an interface down
 *   flx-ifconfig <if> inet ADDR/NN
 *   flx-ifconfig <if> inet6 ADDR/NN
 *   flx-ifconfig <if> mtu N
 *
 * License: BSD-2-Clause (FreeLinX original code).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/route/link.h>
#include <netlink/route/addr.h>

static const char *flx_progname = "flx-ifconfig";

static const char *flx_wanted;      /* NULL => show all */

static void
usage(void)
{
	fprintf(stderr,
	    "usage: %s\n"
	    "       %s <interface>\n"
	    "       %s <interface> up|down\n"
	    "       %s <interface> inet ADDR/NN\n"
	    "       %s <interface> inet6 ADDR/NN\n"
	    "       %s <interface> mtu <n>\n",
	    flx_progname, flx_progname, flx_progname,
	    flx_progname, flx_progname, flx_progname);
	exit(1);
}

/* --- link flags -> BSD-ish string --------------------------------------- */
static void
flx_flags_str(unsigned int f, char *buf, size_t buflen)
{
	char *p = buf;
	size_t left = buflen;

	*buf = '\0';
#define FLX_FLAG(mask, name) do { \
		if ((f & (mask)) && left > 3) { \
			int n = snprintf(p, left, "%s,", (name)); \
			p += n; left -= (size_t)n; \
		} \
	} while (0)
	FLX_FLAG(IFF_UP, "UP");
	FLX_FLAG(IFF_BROADCAST, "BROADCAST");
	FLX_FLAG(IFF_DEBUG, "DEBUG");
	FLX_FLAG(IFF_LOOPBACK, "LOOPBACK");
	FLX_FLAG(IFF_POINTOPOINT, "POINTOPOINT");
	FLX_FLAG(IFF_RUNNING, "RUNNING");
	FLX_FLAG(IFF_PROMISC, "PROMISC");
	FLX_FLAG(IFF_MULTICAST, "MULTICAST");
	FLX_FLAG(IFF_NOARP, "NOARP");
#undef FLX_FLAG
	if (p > buf) p[-1] = '\0';
}

/* --- callback: print the addresses attached to one interface -------------- */
struct flx_addr_ctx {
	struct nl_cache *addrs;
	int ifindex;
};

static void
flx_addr_cb(struct nl_object *obj, void *arg)
{
	struct flx_addr_ctx *ctx = arg;
	struct rtnl_addr *a = (struct rtnl_addr *) obj;
	struct nl_addr *l;
	char buf[INET6_ADDRSTRLEN];

	if (rtnl_addr_get_ifindex(a) != ctx->ifindex)
		return;
	l = rtnl_addr_get_local(a);
	if (l == NULL)
		return;
	(void)nl_addr2str(l, buf, sizeof(buf));
	printf("\t%s %s\n",
	    nl_addr_get_family(l) == AF_INET ? "inet" : "inet6",
	    buf);
}

/* --- print one link with all its addresses -------------------------------- */
struct flx_link_ctx {
	struct nl_cache *addrs;
	int matched;
};

static void
flx_link_cb(struct nl_object *obj, void *arg)
{
	struct flx_link_ctx *ctx = arg;
	struct rtnl_link *link = (struct rtnl_link *) obj;
	struct flx_addr_ctx actx;
	char flags[128];
	const char *name;

	name = rtnl_link_get_name(link);
	if (flx_wanted != NULL && strcmp(name, flx_wanted) != 0)
		return;
	if (ctx) ctx->matched = 1;

	flx_flags_str(rtnl_link_get_flags(link), flags, sizeof(flags));
	printf("%s: flags=%u<%s> mtu %u\n",
	    name, rtnl_link_get_flags(link), flags,
	    rtnl_link_get_mtu(link));

	if (ctx->addrs != NULL) {
		actx.addrs = ctx->addrs;
		actx.ifindex = rtnl_link_get_ifindex(link);
		nl_cache_foreach(ctx->addrs, flx_addr_cb, &actx);
	}
	printf("\n");
}

static int
flx_list(struct nl_sock *sock, const char *wanted)
{
	struct nl_cache *links = NULL, *addrs = NULL;
	struct flx_link_ctx ctx;
	int rc = 0;
	int err;

	err = rtnl_link_alloc_cache(sock, AF_UNSPEC, &links);
	if (err < 0 || links == NULL) {
		fprintf(stderr, "%s: unable to read interface list: %s\n",
		    flx_progname, nl_geterror(err));
		return 1;
	}
	if (rtnl_addr_alloc_cache(sock, &addrs) < 0)
		addrs = NULL;

	flx_wanted = wanted;
	ctx.addrs = addrs;
	ctx.matched = 0;

	nl_cache_foreach(links, flx_link_cb, &ctx);

	if (wanted != NULL && ctx.matched == 0) {
		fprintf(stderr, "%s: interface %s does not exist\n",
		    flx_progname, wanted);
		rc = 1;
	}

	if (links) nl_cache_put(links);
	if (addrs) nl_cache_put(addrs);
	return rc;
}

/* --- find a link in the cache by name ------------------------------------ */
static struct rtnl_link *
flx_get_link(struct nl_sock *sock, const char *name, struct nl_cache **out)
{
	*out = NULL;
	if (rtnl_link_alloc_cache(sock, AF_UNSPEC, out) < 0 || *out == NULL) {
		fprintf(stderr, "%s: unable to get link cache: %s\n",
		    flx_progname, strerror(errno));
		return NULL;
	}
	return rtnl_link_get_by_name(*out, name);
}

/* --- bring an interface up/down via rtnl_link_change(4-arg) -------------- */
static int
flx_set_state(struct nl_sock *sock, const char *name, int up)
{
	struct nl_cache *cache = NULL;
	struct rtnl_link *link, *changes;
	int rc;

	link = flx_get_link(sock, name, &cache);
	if (link == NULL) {
		fprintf(stderr, "%s: interface %s does not exist\n",
		    flx_progname, name);
		if (cache) nl_cache_put(cache);
		return 1;
	}
	changes = rtnl_link_alloc();
	if (changes == NULL) { nl_cache_put(cache); return 1; }
	if (up)
		rtnl_link_set_flags(changes, IFF_UP);
	else
		rtnl_link_unset_flags(changes, IFF_UP);

	rc = rtnl_link_change(sock, link, changes, 0);
	nl_object_put((struct nl_object *) changes);
	nl_cache_put(cache);
	if (rc < 0) {
		fprintf(stderr, "%s: cannot set %s %s: %s\n",
		    flx_progname, name, up ? "up" : "down", nl_geterror(rc));
		return 1;
	}
	return 0;
}

/* --- set mtu -------------------------------------------------------------- */
static int
flx_set_mtu(struct nl_sock *sock, const char *name, unsigned int mtu)
{
	struct nl_cache *cache = NULL;
	struct rtnl_link *link, *changes;
	int rc;

	link = flx_get_link(sock, name, &cache);
	if (link == NULL) {
		fprintf(stderr, "%s: interface %s does not exist\n",
		    flx_progname, name);
		if (cache) nl_cache_put(cache);
		return 1;
	}
	changes = rtnl_link_alloc();
	if (changes == NULL) { nl_cache_put(cache); return 1; }
	rtnl_link_set_ifindex(changes, rtnl_link_get_ifindex(link));
	rtnl_link_set_mtu(changes, mtu);

	rc = rtnl_link_change(sock, link, changes, 0);
	nl_object_put((struct nl_object *) changes);
	nl_cache_put(cache);
	if (rc < 0) {
		fprintf(stderr, "%s: cannot set mtu %u on %s: %s\n",
		    flx_progname, mtu, name, nl_geterror(rc));
		return 1;
	}
	return 0;
}

/* --- add an IPv4/IPv6 address -------------------------------------------- */
static int
flx_set_addr(struct nl_sock *sock, const char *name, int family,
             const char *addrstr, int plen)
{
	struct nl_addr *nl = NULL;
	struct rtnl_addr *a;
	int rc;

	if (nl_addr_parse(addrstr, family, &nl) < 0 || nl == NULL) {
		fprintf(stderr, "%s: bad address %s\n", flx_progname, addrstr);
		return 1;
	}
	a = rtnl_addr_alloc();
	if (a == NULL) { nl_addr_put(nl); return 1; }
	/* Set the prefix on the parsed addr BEFORE set_local(): rtnl_addr_set_local()
	 * overwrites the prefixlen with the parsed addr's own (0), which would make
	 * the kernel add a /32 instead of the requested prefix. */
	nl_addr_set_prefixlen(nl, plen);
	rtnl_addr_set_ifindex(a, (int)if_nametoindex(name));
	rtnl_addr_set_family(a, family);
	rtnl_addr_set_local(a, nl);
	rtnl_addr_set_prefixlen(a, plen);
	rc = rtnl_addr_add(sock, a, 0);
	nl_addr_put(nl);
	nl_object_put((struct nl_object *) a);
	if (rc < 0) {
		fprintf(stderr, "%s: cannot add %s address to %s: %s\n",
		    flx_progname, family == AF_INET ? "inet" : "inet6",
		    name, nl_geterror(rc));
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
	if (!sock) { perror("ifconfig: nl_socket_alloc"); return 1; }
	if (nl_connect(sock, NETLINK_ROUTE) < 0) {
		perror("ifconfig: nl_connect");
		nl_socket_free(sock);
		return 1;
	}

	if (argc == 1) {
		rc = flx_list(sock, NULL);
	} else if (argc == 2) {
		rc = flx_list(sock, argv[a]);
	} else {
		const char *iface = argv[a++];
		if (strcmp(argv[a], "up") == 0) {
			rc = flx_set_state(sock, iface, 1);
		} else if (strcmp(argv[a], "down") == 0) {
			rc = flx_set_state(sock, iface, 0);
		} else if (strcmp(argv[a], "mtu") == 0 && a + 1 < argc) {
			rc = flx_set_mtu(sock, iface, (unsigned)strtoul(argv[a + 1], NULL, 10));
		} else if ((strcmp(argv[a], "inet") == 0 ||
		            strcmp(argv[a], "inet6") == 0) && a + 1 < argc) {
			int fam = strcmp(argv[a], "inet6") == 0 ? AF_INET6 : AF_INET;
			char *slash = strchr(argv[a + 1], '/');
			int plen;
			if (slash != NULL) {
				*slash = '\0';
				plen = atoi(slash + 1);
				rc = flx_set_addr(sock, iface, fam, argv[a + 1], plen);
				*slash = '/';
			} else {
				plen = fam == AF_INET ? 24 : 64;
				rc = flx_set_addr(sock, iface, fam, argv[a + 1], plen);
			}
		} else {
			usage();
		}
	}

	nl_socket_free(sock);
	return rc;
}
