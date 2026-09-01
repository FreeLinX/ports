/* FreeLinX/ports - base/compat : NetBSD <sys/mount.h> extension wrapper.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * This header is found first because the FreeLinX compat dir leads the
 * include path (-I base/compat).  It pulls in the real musl <sys/mount.h>
 * via #include_next and then adds what the NetBSD df.c expects and musl does
 * not provide.
 *
 * musl's <sys/mount.h> is the Linux kernel mount(2) interface; its
 * mount-flags are the MS_* values plus MNT_FORCE/MNT_DETACH/MNT_EXPIRE (the
 * umount2(2) flags).  NetBSD df.c is written against the BSD VFS model:
 * MNAMELEN (name buffer length, 90) and the MNT_* exported flags from
 * NetBSD <sys/sys/fstypes.h>.  musl omits both.  The MNT_* values below are
 * the NetBSD definitions (fstypes.h), so df's MNT_WAIT/MNT_NOWAIT/MNT_LOCAL/
 * MNT_IGNORE tests compile verbatim.  They describe a Linux mount table
 * through compat/statvfs.c + compat/getmntinfo.c.
 *
 * Only the flags the base sources actually use are added.
 */
#ifndef _FREELINX_COMPAT_SYS_MOUNT_H_
#define _FREELINX_COMPAT_SYS_MOUNT_H_

#include_next <sys/mount.h>
#include <sys/statvfs.h>

#define	MNAMELEN	90	/* length of buffer for returned name */

/*
 * Mount flags.  Values are NetBSD's (sys/sys/fstypes.h); df.c tests the
 * bits in getmntinfo()/statvfs() results that compat fills with MNT_LOCAL
 * (via the fs-type table) and the readable option bits.
 */
#define	MNT_RDONLY	0x00000001	/* read only filesystem */
#define	MNT_SYNCHRONOUS	0x00000002	/* file system written synchronously */
#define	MNT_NOEXEC	0x00000004	/* can't exec from filesystem */
#define	MNT_NOSUID	0x00000008	/* don't honor setuid bits on fs */
#define	MNT_NODEV	0x00000010	/* don't interpret special files */
#define	MNT_UNION	0x00000020	/* union with underlying filesystem */
#define	MNT_ASYNC	0x00000040	/* file system written asynchronously */
#define	MNT_NOCOREDUMP	0x00008000	/* don't write core dumps to this FS */
#define	MNT_RELATIME	0x00020000	/* only update access time if mod/ch */
#define	MNT_IGNORE	0x00100000	/* don't show entry in df */
#define	MNT_EXTATTR	0x01000000	/* enable extended attributes */
#define	MNT_LOG		0x02000000	/* Use logging */
#define	MNT_NOATIME	0x04000000	/* Never update access times in fs */
#define	MNT_SYMPERM	0x20000000	/* recognize symlink permission */
#define	MNT_NODEVMTIME	0x40000000	/* Never update mod times for devs */
#define	MNT_SOFTDEP	0x80000000	/* Use soft dependencies */

#define	MNT_LOCAL	0x00001000	/* filesystem is stored locally */
#define	MNT_QUOTA	0x00002000	/* quotas are enabled on filesystem */
#define	MNT_ROOTFS	0x00004000	/* identifies the root filesystem */

#define	MNT_WAIT	1	/* synchronously wait for I/O to complete */
#define	MNT_NOWAIT	2	/* start all I/O, but do not wait for it */

#endif /* !_FREELINX_COMPAT_SYS_MOUNT_H_ */