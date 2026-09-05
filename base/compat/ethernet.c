/*
 * FreeLinX/ports - base/compat : ethernet address helpers (musl lacks
 * the <net/ethernet.h>/<net/if_ether.h> ether_* API entirely).
 *
 * ether_aton/ether_ntoa are real parsers/formatters; the /etc/ethers
 * database functions read the conventional "<ether-addr> <hostname>"
 * file just like the BSD libc implementations.
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <net/if_ether.h>

#define _PATH_ETHERS	"/etc/ethers"

static struct ether_addr *
parse_line(char *line, char **name)
{
	static struct ether_addr addr;
	char *p = line;
	int i;

	for (i = 0; i < 6; i++) {
		char *e;
		unsigned long v = strtoul(p, &e, 16);

		if (e == p || v > 0xff)
			return NULL;
		addr.ether_addr_octet[i] = (uint8_t)v;
		p = e;
		if (i < 5) {
			if (*p != ':')
				return NULL;
			p++;
		}
	}
	while (*p != '\0' && !isspace((unsigned char)*p))
		p++;
	while (*p != '\0' && isspace((unsigned char)*p))
		p++;
	*name = p;
	return &addr;
}

struct ether_addr *
ether_aton(const char *asc)
{
	char line[64];
	char *name;
	struct ether_addr *ep;

	snprintf(line, sizeof(line), "%s", asc);
	ep = parse_line(line, &name);
	if (ep == NULL)
		return NULL;
	if (name[0] != '\0' && *name != '\0')
		return NULL;
	{
		static struct ether_addr addr;
		addr = *ep;
		return &addr;
	}
}

char *
ether_ntoa(const struct ether_addr *addr)
{
	static char buf[18];

	snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
	    addr->ether_addr_octet[0], addr->ether_addr_octet[1],
	    addr->ether_addr_octet[2], addr->ether_addr_octet[3],
	    addr->ether_addr_octet[4], addr->ether_addr_octet[5]);
	return buf;
}

int
ether_hostton(const char *hostname, struct ether_addr *e)
{
	FILE *fp;
	char line[256];
	char *name;
	struct ether_addr *ep;
	size_t hlen = strlen(hostname);

	fp = fopen(_PATH_ETHERS, "r");
	if (fp == NULL)
		return -1;
	while (fgets(line, sizeof(line), fp) != NULL) {
		ep = parse_line(line, &name);
		if (ep == NULL)
			continue;
		if (strncmp(name, hostname, hlen) == 0 &&
		    (name[hlen] == '\0' || isspace((unsigned char)
		    name[hlen]))) {
			*e = *ep;
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	errno = ENOENT;
	return -1;
}

int
ether_ntohost(char *hostname, const struct ether_addr *e)
{
	FILE *fp;
	char line[256];
	char *name;
	struct ether_addr *ep;
	char *nl;

	fp = fopen(_PATH_ETHERS, "r");
	if (fp == NULL)
		return -1;
	while (fgets(line, sizeof(line), fp) != NULL) {
		ep = parse_line(line, &name);
		if (ep == NULL)
			continue;
		if (memcmp(ep->ether_addr_octet, e->ether_addr_octet,
		    ETHER_ADDR_LEN) != 0)
			continue;
		nl = name;
		while (*nl != '\0' && !isspace((unsigned char)*nl) &&
		    *nl != '\n')
			nl++;
		*nl = '\0';
		(void)strlcpy(hostname, name, 256);
		fclose(fp);
		return 0;
	}
	fclose(fp);
	errno = ENOENT;
	return -1;
}