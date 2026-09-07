/* FreeLinX/ports - base/compat : musl <sys/msg.h> wrapper.
 *
 * WHY THIS FILE EXISTS: same reason as base/compat/sys/stat.h.  musl's
 * <bits/msg.h> struct msqid_ds has a pad member literally named `__unused`
 * (unsigned long __unused[2]), which the global BSD `__unused` attribute
 * macro from flx_bsd.h would textually corrupt.  Take the macro down for the
 * musl struct parse and restore it immediately afterwards.  SysV IPC tools
 * (ipcrm(1)/ipcs(1), whereis via base/compat/sys/sysvipc.h) include
 * <sys/msg.h>; this wrapper is found first because base/compat leads the
 * include path.
 */
#ifndef _FREELINX_COMPAT_SYS_MSG_H_
#define _FREELINX_COMPAT_SYS_MSG_H_

#ifdef __unused
#undef __unused
#endif
#include_next <sys/msg.h>
#ifndef __unused
#define	__unused	__attribute__((__unused__))
#endif

#endif /* !_FREELINX_COMPAT_SYS_MSG_H_ */