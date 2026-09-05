/*
 * FreeLinX/ports - base/compat : netgroup() stub implementations.
 *
 * There is no NIS in FreeLinX; every netgroup lookup returns "not
 * found" exactly as a system without netgroup data would.  This lets
 * usr.bin/getent and usr.bin/innetgr link and behave usefully for the
 * other databases.
 */

#include <netgroup.h>

int
setnetgrent(const char *group)
{
	(void)group;
	return 0;
}

void
endnetgrent(void)
{
}

int
getnetgrent(const char **host, const char **user, const char **domain)
{
	*host = (const char *)0;
	*user = (const char *)0;
	*domain = (const char *)0;
	return 0;
}

int
innetgr(const char *netgroup, const char *host, const char *user,
    const char *domain)
{
	(void)netgroup;
	(void)host;
	(void)user;
	(void)domain;
	return 0;
}