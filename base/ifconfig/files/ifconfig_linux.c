/* FreeLinX BSD-style ifconfig command, backed by Linux interface ioctls. */
#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

static void usage(void)
{
	fprintf(stderr, "usage: ifconfig [interface]\n"
	    "       ifconfig interface up|down\n"
	    "       ifconfig interface mtu VALUE\n"
	    "       ifconfig interface inet ADDRESS [netmask MASK]\n");
	exit(2);
}

static void set_name(struct ifreq *ifr, const char *name)
{
	memset(ifr, 0, sizeof(*ifr));
	if (strlen(name) >= IFNAMSIZ) {
		fprintf(stderr, "ifconfig: interface name too long: %s\n", name);
		exit(2);
	}
	strcpy(ifr->ifr_name, name);
}

static const char *addr_text(const struct sockaddr *sa, char out[INET_ADDRSTRLEN])
{
	const struct sockaddr_in *sin = (const struct sockaddr_in *)sa;
	if (sa->sa_family != AF_INET)
		return "-";
	if (inet_ntop(AF_INET, &sin->sin_addr, out, INET_ADDRSTRLEN) == NULL)
		return "-";
	return out;
}

static void print_flags(short flags)
{
	int first = 1;
	struct { short bit; const char *name; } names[] = {
		{ IFF_UP, "UP" }, { IFF_BROADCAST, "BROADCAST" },
		{ IFF_LOOPBACK, "LOOPBACK" }, { IFF_RUNNING, "RUNNING" },
		{ IFF_PROMISC, "PROMISC" }, { IFF_MULTICAST, "MULTICAST" }
	};
	size_t i;
	printf("flags=%#x<", (unsigned short)flags);
	for (i = 0; i < sizeof(names) / sizeof(names[0]); i++)
		if (flags & names[i].bit) {
			printf("%s%s", first ? "" : ",", names[i].name);
			first = 0;
		}
	puts(">");
}

static void show_one(int fd, const char *name)
{
	struct ifreq ifr;
	char address[INET_ADDRSTRLEN], mask[INET_ADDRSTRLEN];
	set_name(&ifr, name);
	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
		fprintf(stderr, "ifconfig: %s: %s\n", name, strerror(errno));
		return;
	}
	printf("%s: ", name); print_flags(ifr.ifr_flags);
	set_name(&ifr, name);
	if (ioctl(fd, SIOCGIFMTU, &ifr) == 0)
		printf("\tmtu %d\n", ifr.ifr_mtu);
	set_name(&ifr, name);
	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
		const char *a = addr_text(&ifr.ifr_addr, address);
		set_name(&ifr, name);
		if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
			printf("\tinet %s netmask %s\n", a, addr_text(&ifr.ifr_addr, mask));
		else
			printf("\tinet %s\n", a);
	}
}

static void show_all(int fd)
{
	char buf[16384];
	struct ifconf ifc;
	struct ifreq *ifr;
	int count, i;
	memset(&ifc, 0, sizeof(ifc));
	ifc.ifc_len = sizeof(buf); ifc.ifc_buf = buf;
	if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) { perror("ifconfig: SIOCGIFCONF"); exit(1); }
	ifr = ifc.ifc_req; count = ifc.ifc_len / (int)sizeof(*ifr);
	for (i = 0; i < count; i++) {
		int duplicate = 0, j;
		for (j = 0; j < i; j++)
			if (strcmp(ifr[j].ifr_name, ifr[i].ifr_name) == 0) duplicate = 1;
		if (!duplicate) show_one(fd, ifr[i].ifr_name);
	}
}

static int set_flags(int fd, const char *name, int up)
{
	struct ifreq ifr;
	set_name(&ifr, name);
	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) return -1;
	if (up) ifr.ifr_flags |= IFF_UP; else ifr.ifr_flags &= (short)~IFF_UP;
	return ioctl(fd, SIOCSIFFLAGS, &ifr);
}

static int set_addr(int fd, const char *name, int request, const char *text)
{
	struct ifreq ifr;
	struct sockaddr_in *sin;
	set_name(&ifr, name);
	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	sin->sin_family = AF_INET;
	if (inet_pton(AF_INET, text, &sin->sin_addr) != 1) { errno = EINVAL; return -1; }
	return ioctl(fd, request, &ifr);
}

int main(int argc, char **argv)
{
	int fd, rc = 0;
	struct ifreq ifr;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("ifconfig: socket"); return 1; }
	if (argc == 1) { show_all(fd); close(fd); return 0; }
	if (argc == 2) { show_one(fd, argv[1]); close(fd); return 0; }
	if (strcmp(argv[2], "up") == 0 || strcmp(argv[2], "down") == 0) {
		rc = set_flags(fd, argv[1], argv[2][0] == 'u');
	} else if (strcmp(argv[2], "mtu") == 0 && argc == 4) {
		char *end; long mtu = strtol(argv[3], &end, 10);
		if (*argv[3] == '\0' || *end != '\0' || mtu < 68 || mtu > 65535) usage();
		set_name(&ifr, argv[1]); ifr.ifr_mtu = (int)mtu; rc = ioctl(fd, SIOCSIFMTU, &ifr);
	} else if (strcmp(argv[2], "inet") == 0 && (argc == 4 || argc == 6)) {
		rc = set_addr(fd, argv[1], SIOCSIFADDR, argv[3]);
		if (rc == 0 && argc == 6 && strcmp(argv[4], "netmask") == 0)
			rc = set_addr(fd, argv[1], SIOCSIFNETMASK, argv[5]);
		else if (argc == 6 && strcmp(argv[4], "netmask") != 0) usage();
	} else usage();
	if (rc < 0) fprintf(stderr, "ifconfig: %s: %s\n", argv[1], strerror(errno));
	close(fd);
	return rc < 0;
}
