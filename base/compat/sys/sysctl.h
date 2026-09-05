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
#include <sys/sysvipc.h>

#define	CTL_KERN	1
#define	KERN_SYSVIPC	24

#define	KERN_SYSVIPC_MSG		1
#define	KERN_SYSVIPC_SEM		2
#define	KERN_SYSVIPC_SHM		3
#define	KERN_SYSVIPC_INFO		4
#define	KERN_SYSVIPC_MSG_INFO		1
#define	KERN_SYSVIPC_SEM_INFO		2
#define	KERN_SYSVIPC_SHM_INFO		3

#define	CTL_USER	6
#define	USER_CS_PATH	100

int	sysctl(const int *, u_int, void *, size_t *,
	    const void *, size_t);

#endif /* !_FREELINX_COMPAT_SYS_SYSCTL_H_ */