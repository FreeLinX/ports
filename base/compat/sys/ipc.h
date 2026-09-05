/*
 * FreeLinX/ports - base/compat/sys/ipc.h : NetBSD-flavoured <sys/ipc.h>.
 *
 * ipcrm(1)/ipcs(1) talk to the SysV IPC table through sysctl(3) and to
 * the kernel through msgctl/semctl/shmctl.  On FreeLinX the former is
 * answered from /proc/sysvipc and the latter from Linux SysV IPC, but
 * the tool sources reference ipc_perm members NetBSD names -- "._key"
 * -- that musl spells "__ipc_perm_key".  This header (picked up before
 * the sysroot copy via -I) provides the NetBSD names.
 */

#ifndef _FREELINX_COMPAT_SYS_IPC_H_
#define _FREELINX_COMPAT_SYS_IPC_H_

#include <sys/types.h>

typedef int key_t;

#define	IPC_PRIVATE	0
#define	IPC_CREAT	00001000	/* create if key is nonexistent */
#define	IPC_EXCL	00002000	/* fail if key exists */
#define	IPC_NOWAIT	00004000	/* return error on wait */

#define	IPC_RMID	0		/* remove resource */
#define	IPC_SET		1		/* set options */
#define	IPC_STAT	2		/* get options */
#define	IPC_INFO	3		/* see sys/sysvipc.h */

struct ipc_perm {
	key_t	_key;		/* key given to the xxxget(2) call */
	uid_t	uid;		/* effective UID of owner */
	gid_t	gid;		/* effective GID of owner */
	uid_t	cuid;		/* effective UID of creator */
	gid_t	cgid;		/* effective GID of creator */
	mode_t	mode;		/* permission bits; the FreeLinX sysctl
				   shim packs the real ipcid into the top
				   16 bits (masked by ipcs' fmt_perm). */
};

#endif /* !_FREELINX_COMPAT_SYS_IPC_H_ */