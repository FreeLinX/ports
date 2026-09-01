/* FreeLinX/ports - base/compat : musl statvfs(2) stand-in for NetBSD df.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD `/usr/bin/df` calls statvfs(2) and expects it to fill the BSD
 * `struct statvfs` (see compat/sys/statvfs.h), including the char-array
 * fields f_mntfromname/f_mntonname/f_fstypename that describe the mount.
 * Linux statvfs(2) (musl's) fills a different, POSIX-only struct and cannot
 * produce those names.  This compat object re-declares the `statvfs` symbol:
 * it is linked before -lc (as a member of the port's own OBJ_DIR set), so
 * musl's libc.a statvfs.o is never pulled -- there is no duplicate-symbol
 * error.  The implementation translates the Linux statfs(2) numbers and the
 * /proc/self/mounts entry for the path's containing mount into the NetBSD
 * struct.
 */
#include <sys/cdefs.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * FreeLinX: musl ships statfs(2) and statvfs(2) in the SAME object file
 * (libc.a statvfs.lo), so linking any reference to statfs() drags in musl's
 * statvfs() too -- a duplicate-symbol clash with the NetBSD-faithful
 * flx_statvfs() we provide here.  To keep musl's statvfs.lo out of the link,
 * the Linux statfs(2) numbers are obtained with a direct syscall(SYS_statfs)
 * rather than by calling libc's statfs().
 */
#define FLX_STATFS(_p, _s)	(syscall(SYS_statfs, (_p), (_s)) == 0)

#define	_NFSPATH	"/proc/self/mounts"

static const char *flx_netfstypes[] = {
	"nfs", "nfs4", "nfsd", "cifs", "smbfs", "sshfs", "fuse.sshfs",
	"davfs", "davs", "webdav", "curlftpfs", "ftpfs", "p9", "panfs",
	"githubfs", "gdfs", "glusterfs", "lustre", "gvfsd-fuse", NULL
};

/* FreeLinX: 1 if `type` is a network filesystem (not MNT_LOCAL). */
int
flx_netfs(const char *type)
{
	int i;

	for (i = 0; flx_netfstypes[i] != NULL; i++)
		if (strcmp(type, flx_netfstypes[i]) == 0)
			return 1;
	return 0;
}

/*
 * FreeLinX: map a Linux mount option string + fs type to the NetBSD MNT_*
 * flags (values from compat/sys/mount.h).  MNT_LOCAL comes from the fs type.
 * This is shared by compat/statvfs.c and compat/getmntinfo.c.
 */
int
flx_statvfs_flags(const char *opts, const char *type)
{
	const char *o, *p;
	size_t n;
	char opt[64];
	unsigned long flags;

	flags = flx_netfs(type) ? 0 : MNT_LOCAL;
	for (o = opts; o != NULL; o = p) {
		p = strchr(o, ',');
		n = p != NULL ? (size_t)(p - o) : strlen(o);
		if (n >= sizeof(opt))
			n = sizeof(opt) - 1;
		memcpy(opt, o, n);
		opt[n] = '\0';
		if (p != NULL)
			p++;
		if (strcmp(opt, "ro") == 0)
			flags |= MNT_RDONLY;
		else if (strcmp(opt, "nosuid") == 0)
			flags |= MNT_NOSUID;
		else if (strcmp(opt, "nodev") == 0)
			flags |= MNT_NODEV;
		else if (strcmp(opt, "noexec") == 0)
			flags |= MNT_NOEXEC;
		else if (strcmp(opt, "sync") == 0)
			flags |= MNT_SYNCHRONOUS;
		else if (strcmp(opt, "noatime") == 0)
			flags |= MNT_NOATIME;
		else if (strcmp(opt, "relatime") == 0)
			flags |= MNT_RELATIME;
	}
	return flags;
}

/*
 * Fill the char-array fields (source, mountpoint, type) of `sf` from the
 * /proc/self/mounts entry whose mount point is the longest prefix of `path`.
 * Returns 0 on success, -1 if `path` matches no mount.  \040 (space) escapes
 * in the source/mountpoint fields are left as-is; they name the same paths
 * that appear escaped in `path` when called from df with a mount-point arg.
 */
static int
flx_fill_mount_names(const char *path, struct statvfs *sf)
{
	char *line, *src, *mnt, *type, *opts, *tok;
	FILE *fp;
	int fd, matched;
	size_t len, best;

	fd = open(_NFSPATH, O_RDONLY);
	if (fd == -1)
		return -1;
	fp = fdopen(fd, "r");
	if (fp == NULL) {
		close(fd);
		return -1;
	}

	best = 0;
	matched = 0;
	line = NULL;
	len = 0;
	while (getline(&line, &len, fp) != -1) {
		/* fs_spec fs_file fs_vfstype fs_mntopts ... (space separated) */
		tok = strtok(line, " \t\n");
		if (tok == NULL)
			continue;
		src = tok;
		tok = strtok(NULL, " \t\n");
		if (tok == NULL)
			continue;
		mnt = tok;
		tok = strtok(NULL, " \t\n");
		if (tok == NULL)
			continue;
		type = tok;
		tok = strtok(NULL, " \t\n");
		opts = tok != NULL ? tok : "";

		if (strncmp(path, mnt, strlen(mnt)) == 0 &&
		    strlen(mnt) > best) {
			best = strlen(mnt);
			(void)strncpy(sf->f_mntfromname, src,
			    sizeof(sf->f_mntfromname) - 1);
			(void)strncpy(sf->f_mntonname, mnt,
			    sizeof(sf->f_mntonname) - 1);
			(void)strncpy(sf->f_fstypename, type,
			    sizeof(sf->f_fstypename) - 1);
			sf->f_mntfromname[sizeof(sf->f_mntfromname) - 1] = '\0';
			sf->f_mntonname[sizeof(sf->f_mntonname) - 1] = '\0';
			sf->f_fstypename[sizeof(sf->f_fstypename) - 1] = '\0';
			sf->f_mntfromlabel[0] = '\0';
			sf->f_flag = flx_statvfs_flags(opts, type);
			matched = 1;
		}
	}
	if (line != NULL)
		free(line);
	fclose(fp);
	return matched ? 0 : -1;
}

int
statvfs(const char *path, struct statvfs *sf)
{
	char mntpath[1024];
	struct statfs ss;

	/* Fill numeric + name fields from /proc/self/mounts; if `path` is not
	 * under any mount (unlikely), statfs it directly and leave names
	 * empty. */
	if (flx_fill_mount_names(path, sf) < 0)
		(void)strncpy(mntpath, path, sizeof(mntpath) - 1);
	else
		(void)strncpy(mntpath, sf->f_mntonname, sizeof(mntpath) - 1);
	mntpath[sizeof(mntpath) - 1] = '\0';

	if (!FLX_STATFS(mntpath, &ss))
		return -1;

	sf->f_bsize = ss.f_bsize;
	sf->f_frsize = ss.f_frsize;
	sf->f_iosize = ss.f_bsize;
	sf->f_blocks = ss.f_blocks;
	sf->f_bfree = ss.f_bfree;
	sf->f_bavail = ss.f_bavail;
	sf->f_bresvd = 0;
	sf->f_files = ss.f_files;
	sf->f_ffree = ss.f_ffree;
	sf->f_favail = ss.f_ffree;
	sf->f_fresvd = 0;
	sf->f_syncreads = 0;
	sf->f_syncwrites = 0;
	sf->f_asyncreads = 0;
	sf->f_asyncwrites = 0;
	sf->f_fsid = (unsigned long)ss.f_fsid.__val[0];
	sf->f_namemax = ss.f_namelen;
	sf->f_owner = 0;
	sf->f_spare[0] = sf->f_spare[1] = sf->f_spare[2] = sf->f_spare[3] = 0;

	return 0;
}