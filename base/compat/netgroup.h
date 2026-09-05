/*
 * FreeLinX/ports - base/compat : musl <netgroup.h> stand-in.
 *
 * musl implements no netgroup(NIS) lookup at all, and FreeLinX has no
 * NIS.  usr.bin/getent and usr.bin/innetgr include this header; the
 * companion netgroup.c provides empty, always-no-match implementations
 * so the utilities build and report "no netgroup data" cleanly.
 */

#ifndef _FREELINX_COMPAT_NETGROUP_H_
#define _FREELINX_COMPAT_NETGROUP_H_

int	setnetgrent(const char *);
void	endnetgrent(void);
int	getnetgrent(const char **, const char **, const char **);
int	innetgr(const char *, const char *, const char *, const char *);

#endif /* !_FREELINX_COMPAT_NETGROUP_H_ */