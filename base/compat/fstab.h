/*
 * FreeLinX/ports - base/compat : musl <fstab.h> stand-in.
 *
 * NetBSD's quota user-land reads the mounted-filesystem table via the
 * BSD getfsent(3)/getfsspec(3) family to resolve a device to its mount
 * point.  musl has no fstab.h at all; FreeLinX provides the BSD struct
 * and a Linux-native backend (compat/fstab.c) that parses /etc/mtab so
 * quota shows real mounted filesystems.
 */

#ifndef _FREELINX_COMPAT_FSTAB_H_
#define _FREELINX_COMPAT_FSTAB_H_

struct fstab {
	char	*fs_spec;	/* block special device name */
	char	*fs_file;	/* filesystem path prefix */
	char	*fs_vfstype;	/* file system type */
	char	*fs_mntops;	/* mount options */
	const char *fs_type;	/* rw, ro, sw, or xx */
	int	fs_freq;	/* dump frequency, in days */
	int	fs_passno;	/* pass number on parallel dump */
};

struct fstab	*getfsent(void);
struct fstab	*getfsspec(const char *);
struct fstab	*getfsfile(const char *);
int		setfsent(void);
void		endfsent(void);

#endif /* !_FREELINX_COMPAT_FSTAB_H_ */