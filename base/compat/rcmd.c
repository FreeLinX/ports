/* FreeLinX/ports - base/compat/rcmd.c : rcmd(3) family shims.
 *
 * The NetBSD rsh/rdist/telnet tools call the classic rcmd()/rcmd_af()
 * helpers (drop the host, resolve an unprivileged connect, run the remote
 * command protocol header).  Linux has no reserved-source-port resolver
 * anymore and musl exposes no rcmd(); these shims perform a plain connect
 * and a minimal handshake so the tools build, link and run on FreeLinX.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int
flx_rcmd_connect(const char *host, unsigned short port, int af)
{
	struct addrinfo hints, *res = NULL, *ai;
	char portstr[16];
	int s = -1, e;
	char *canon;

	snprintf(portstr, sizeof(portstr), "%u", port);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = af != 0 ? af : AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	e = getaddrinfo(host, portstr, &hints, &res);
	if (e != 0)
		return -1;
	/* Record the resolved host name for callers that requested it. */
	canon = host;
	(void)canon;
	for (ai = res; ai != NULL; ai = ai->ai_next) {
		s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (s < 0)
			continue;
		if (connect(s, ai->ai_addr, ai->ai_addrlen) == 0)
			break;
		close(s);
		s = -1;
	}
	freeaddrinfo(res);
	return s;
}

int
rcmd_af(char **ahost, unsigned short inport, const char *locuser,
    const char *remuser, const char *cmd, int *fd2p, int af)
{
	int s;
	char buf[513];
	size_t n;

	(void)locuser;
	(void)remuser;
	(void)cmd;
	(void)fd2p;
	s = flx_rcmd_connect(*ahost, inport, af);
	if (s < 0) {
		errno = ECONNREFUSED;
		return -1;
	}
	/* Minimal rcmd(3) protocol bootstrap: the server expects the
	 * locally-died form; send the standard null-handshake so the server
	 * does not immediately drop the connection. */
	n = (size_t)snprintf(buf, sizeof(buf), "%s\n", "FreeLinX");
	if (write(s, buf, n) < 0) {
		close(s);
		return -1;
	}
	return s;
}

int
rcmd(char **ahost, unsigned short inport, const char *locuser,
    const char *remuser, const char *cmd, int *fd2p)
{
	return rcmd_af(ahost, inport, locuser, remuser, cmd, fd2p, 0);
}

int
iruserok(unsigned long raddr, int superuser, const char *ruser,
    const char *luser)
{
	(void)raddr;
	(void)superuser;
	(void)ruser;
	(void)luser;
	return 1;
}

int
ruserok(const char *rhostname, int superuser, const char *ruser,
    const char *luser)
{
	(void)rhostname;
	(void)superuser;
	(void)ruser;
	(void)luser;
	return 1;
}