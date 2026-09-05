/*
 * FreeLinX/ports - base/compat/sysctl.h : minimal <sys/sysctl.h>.
 *
 * Only the entries usr.bin/whereis needs (the "user.cs_path" query that
 * nets the standard command search path) are provided.  FreeLinX has no
 * kernel sysctl(8) MIB, so everything else returns ENOTSUP.
 */

#ifndef _FREELINX_COMPAT_SYS_SYSCTL_H_
#define _FREELINX_COMPAT_SYS_SYSCTL_H_

#include <sys/types.h>

#define	CTL_USER	6
#define	USER_CS_PATH	100

int	sysctl(const int *, u_int, void *, size_t *,
	    const void *, size_t);

#endif /* !_FREELINX_COMPAT_SYS_SYSCTL_H_ */