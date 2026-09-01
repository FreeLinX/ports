/* FreeLinX/ports - base/compat : NetBSD <sys/statvfs.h> stand-in.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's `/usr/bin/df` is written against the BSD VFS statvfs model: the
 * `struct statvfs` with BSD fields (f_fstypename, f_mntonname, f_mntfromname,
 * f_mntfromlabel, f_fsidx, f_owner, the f_fresvd/f_favail quotas and the
 * 64-bit sync/async counters), plus getmntinfo(3).  musl's `struct statvfs`
 * is the POSIX-mandated slim shape with a different member set.  Rather than
 * patching the pristine NetBSD df.c, this header shadows the system one (the
 * FreeLinX compat dir leads the -I path) and defines the NetBSD struct the
 * source actually uses.
 *
 * The layout is the NetBSD 10.1 definition (sys/sys/statvfs.h).  getmntinfo
 * and statvfs are re-declared here (no __RENAME, since there is no kernel
 * ABI to be compatible with in a static build); the implementations are
 * compat/statvfs.c and compat/getmntinfo.c, which translate the Linux
 * statfs(2)/fstatfs(2) and /proc/self/mounts data into this struct.
 *
 * Only the user-space subset df needs is provided.  fsblkcnt_t/fsfilcnt_t
 * and uid_t come from musl via <sys/types.h>.  fsid_t is not defined here;
 * df.c never uses it (it reads f_fsid, which is unsigned long).
 */
#ifndef _FREELINX_COMPAT_SYS_STATVFS_H_
#define _FREELINX_COMPAT_SYS_STATVFS_H_

#include <sys/types.h>
#include <stdint.h>

#define	_VFS_NAMELEN	32
#define	_VFS_MNAMELEN	1024

struct statvfs {
	unsigned long	f_flag;		/* copy of mount exported flags */
	unsigned long	f_bsize;	/* file system block size */
	unsigned long	f_frsize;	/* fundamental file system block size */
	unsigned long	f_iosize;	/* optimal file system block size */

	/* The following are in units of f_frsize */
	fsblkcnt_t	f_blocks;	/* number of blocks in file system */
	fsblkcnt_t	f_bfree;	/* free blocks avail in file system */
	fsblkcnt_t	f_bavail;	/* free blocks avail to non-root */
	fsblkcnt_t	f_bresvd;	/* blocks reserved for root */

	fsfilcnt_t	f_files;	/* total file nodes in file system */
	fsfilcnt_t	f_ffree;	/* free file nodes in file system */
	fsfilcnt_t	f_favail;	/* free file nodes avail to non-root */
	fsfilcnt_t	f_fresvd;	/* file nodes reserved for root */

	uint64_t	f_syncreads;	/* count of sync reads since mount */
	uint64_t	f_syncwrites;	/* count of sync writes since mount */
	uint64_t	f_asyncreads;	/* count of async reads since mount */
	uint64_t	f_asyncwrites;	/* count of async writes since mount */

	unsigned long	f_fsid;		/* Posix compatible fsid */
	unsigned long	f_namemax;	/* maximum filename length */
	uid_t		f_owner;	/* user that mounted the file system */

	uint64_t	f_spare[4];	/* spare space */

	char	f_fstypename[_VFS_NAMELEN];	/* fs type name */
	char	f_mntonname[_VFS_MNAMELEN];	/* directory on which mounted */
	char	f_mntfromname[_VFS_MNAMELEN];	/* mounted file system */
	char	f_mntfromlabel[_VFS_MNAMELEN];	/* disk label name if avail */
};

int	getmntinfo(struct statvfs **, int);
int	statvfs(const char *__restrict, struct statvfs *__restrict);
#endif /* !_FREELINX_COMPAT_SYS_STATVFS_H_ */