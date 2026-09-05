/*
 * FreeLinX/ports - base/compat : musl <rpc/rpcent.h> stand-in.
 *
 * The rpc(3) database API (getrpcent et al.) reads /etc/rpc.  musl has
 * no such API, so FreeLinX provides the header plus no-data wrappers
 * (rpc.c); the rpc database reports empty, matching a system without
 * /etc/rpc entries.
 */

#ifndef _FREELINX_COMPAT_RPC_RPCENT_H_
#define _FREELINX_COMPAT_RPC_RPCENT_H_

struct rpcent {
	char	*r_name;
	char	**r_aliases;
	int	r_number;
};

struct rpcent	*getrpcent(void);
struct rpcent	*getrpcbyname(const char *);
struct rpcent	*getrpcbynumber(int);
void		setrpcent(int);
void		endrpcent(void);

#endif /* !_FREELINX_COMPAT_RPC_RPCENT_H_ */