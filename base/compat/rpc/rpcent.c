/*
 * FreeLinX/ports - base/compat : rpc(3) database wrappers.
 *
 * With no /etc/rpc in FreeLinX the rpc database is empty; every
 * lookup returns not-found (NULL), which is honest and keeps the
 * utilities linkable.
 */

#include <rpc/rpcent.h>

struct rpcent *
getrpcent(void)
{
	return NULL;
}

struct rpcent *
getrpcbyname(const char *name)
{
	(void)name;
	return NULL;
}

struct rpcent *
getrpcbynumber(int number)
{
	(void)number;
	return NULL;
}

void
setrpcent(int stayopen)
{
	(void)stayopen;
}

void
endrpcent(void)
{
}