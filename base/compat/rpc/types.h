/* FreeLinX musl-compat - minimal <rpc/types.h> stub.
 *
 * NetBSD usr.sbin/inetd pulls <rpc/types.h> for the historical sunrpc
 * typedefs.  musl has no RPC library; inetd on FreeLinX only needs the type
 * names to compile (no RPC protocols are actually used).  Only truly
 * missing names are defined here (the u_int, u_long and u_char family
 * already comes from musl <sys/types.h>).
 *
 * NOTE: never spell the u_int family here as u_int followed by a comment
 * terminator.  That terminator closes this block comment mid-sentence and the
 * rest of the line gets parsed as code, which is how this header used to fail
 * with "expected identifier or '('" in ten RPC/NIS ports (rpcinfo, rusers,
 * rwall, showmount, rup, ypcat, ...).
 */
#ifndef _RPC_TYPES_H
#define _RPC_TYPES_H

typedef int bool_t;
typedef int enum_t;
typedef long rpcprog_t;
typedef long rpcvers_t;
typedef long rpcproc_t;
typedef long rpcport_t;
typedef unsigned long rpcblk_t;

#endif /* _RPC_TYPES_H */