/* FreeLinX BSD-style route command, backed by Linux route ioctls. */
#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

static void usage(void)
{
	fprintf(stderr, "usage: route [-n]\n"
	    "       route add|delete default [gw GATEWAY] [dev IFACE]\n"
	    "       route add|delete -net DEST[/PREFIX] [gw GATEWAY] [dev IFACE]\n");
	exit(2);
}

static int parse_ipv4(const char *text, struct in_addr *out)
{
	return inet_pton(AF_INET, text, out) == 1 ? 0 : -1;
}

static int prefix_mask(const char *text, struct in_addr *mask)
{
	char *end;
	long n = strtol(text, &end, 10);
	uint32_t value;
	if (*text == '\0' || *end != '\0' || n < 0 || n > 32)
		return -1;
	value = n == 0 ? 0 : 0xffffffffU << (32 - n);
	mask->s_addr = htonl(value);
	return 0;
}

static void sockaddr4(struct sockaddr *dst, const struct in_addr *addr)
{
	struct sockaddr_in *sin = (struct sockaddr_in *)dst;
	memset(sin, 0, sizeof(*sin));
	sin->sin_family = AF_INET;
	sin->sin_addr = *addr;
}

static void show_routes(int numeric)
{
	FILE *f = fopen("/proc/net/route", "r");
	char line[512];
	(void)numeric;
	if (f == NULL) {
		perror("route: /proc/net/route");
		exit(1);
	}
	puts("Kernel IP routing table");
	puts("Destination     Gateway         Genmask         Flags Metric Ref    Use Iface");
	(void)fgets(line, sizeof(line), f);
	while (fgets(line, sizeof(line), f) != NULL) {
		char iface[IFNAMSIZ];
		unsigned long dst, gateway, flags, mask;
		int metric, ref, use;
		struct in_addr a;
		char d[INET_ADDRSTRLEN], g[INET_ADDRSTRLEN], m[INET_ADDRSTRLEN];
		if (sscanf(line, "%15s %lx %lx %lx %d %d %d %lx", iface,
		    &dst, &gateway, &flags, &metric, &ref, &use, &mask) != 8)
			continue;
		a.s_addr = htonl((uint32_t)dst); inet_ntop(AF_INET, &a, d, sizeof(d));
		a.s_addr = htonl((uint32_t)gateway); inet_ntop(AF_INET, &a, g, sizeof(g));
		a.s_addr = htonl((uint32_t)mask); inet_ntop(AF_INET, &a, m, sizeof(m));
		printf("%-15s %-15s %-15s %04lX  %-6d %-6d %-6d %s\n",
		    d, g, m, flags, metric, ref, use, iface);
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	struct rtentry route;
	struct in_addr dst = { .s_addr = INADDR_ANY }, mask = { .s_addr = INADDR_ANY };
	struct in_addr gateway = { .s_addr = INADDR_ANY };
	const char *dev = NULL;
	int add, fd, i;

	if (argc == 1) { show_routes(0); return 0; }
	if (argc == 2 && strcmp(argv[1], "-n") == 0) { show_routes(1); return 0; }
	if (argc < 3) usage();
	if (strcmp(argv[1], "add") == 0) add = 1;
	else if (strcmp(argv[1], "delete") == 0 || strcmp(argv[1], "del") == 0) add = 0;
	else usage();

	if (strcmp(argv[2], "default") == 0) {
		dst.s_addr = INADDR_ANY; mask.s_addr = INADDR_ANY;
	} else if (strcmp(argv[2], "-net") == 0 && argc >= 4) {
		char spec[INET_ADDRSTRLEN + 4], *slash;
		if (strlen(argv[3]) >= sizeof(spec)) usage();
		strcpy(spec, argv[3]); slash = strchr(spec, '/');
		if (slash != NULL) { *slash++ = '\0'; if (prefix_mask(slash, &mask) != 0) usage(); }
		else mask.s_addr = htonl(0xffffffffU);
		if (parse_ipv4(spec, &dst) != 0) usage();
		i = 4;
	} else usage();
	if (strcmp(argv[2], "default") == 0) i = 3;
	while (i < argc) {
		if (strcmp(argv[i], "gw") == 0 && i + 1 < argc) {
			if (parse_ipv4(argv[++i], &gateway) != 0) usage();
		} else if (strcmp(argv[i], "dev") == 0 && i + 1 < argc) dev = argv[++i];
		else usage();
		i++;
	}
	memset(&route, 0, sizeof(route));
	sockaddr4(&route.rt_dst, &dst); sockaddr4(&route.rt_genmask, &mask);
	route.rt_flags = RTF_UP;
	if (gateway.s_addr != INADDR_ANY) { sockaddr4(&route.rt_gateway, &gateway); route.rt_flags |= RTF_GATEWAY; }
	route.rt_dev = (char *)dev;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) { perror("route: socket"); return 1; }
	if (ioctl(fd, add ? SIOCADDRT : SIOCDELRT, &route) < 0) {
		fprintf(stderr, "route: %s route: %s\n", add ? "add" : "delete", strerror(errno));
		close(fd); return 1;
	}
	close(fd);
	return 0;
}
