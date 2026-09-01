/* FreeLinX/ports - base/compat : musl getmntinfo(3) stand-in for NetBSD df.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD `/usr/bin/df` calls getmntinfo(3) (and getvfsstat(2) underneath it)
 * to enumerate every mounted filesystem.  musl/Linux has neither; the Linux
 * mount table is /proc/self/mounts (the real kernel export, same data
 * mount(8)/findmnt(8) read).  This compat object parses that file and fills
 * an array of the NetBSD `struct statvfs` (see compat/sys/statvfs.h): the
 * source/mountpoint/type names and the MNT_* flags from the mount line, the
 * block/inode counts from the Linux statfs(2) of each mount point.  It is
 * the direct analogue of NetBSD's lib/libc/gen/getmntinfo.c.
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

#define	_NFSPATH	"/proc/self/mounts"

/*
 * musl defines statfs(2) and statvfs(2) in the same libc.a member
 * (statvfs.lo); to keep musl's statvfs() out of the df link set (we provide
 * a NetBSD-faithful flx_statvfs), the block counts come from a raw
 * syscall(SYS_statfs) -- see compat/statvfs.c.
 */
#define FLX_STATFS(_p, _s)	(syscall(SYS_statfs, (_p), (_s)) == 0)

/*
 * flx_statvfs_flags/flx_netfs live in compat/statvfs.c and are always linked
 * (df) beside this object.
 */
int	flx_netfs(const char *);
int	flx_statvfs_flags(const char *, const char *);

/*
 * Decode the \040 (and \011) octal escapes /proc uses for spaces/tabs in the
 * source and mount point fields.  `s` is a token from the mount line; the
 * result replaces it in place (the token is no shorter than its decoded
 * form).
 */
static const char *
flx_unescape(const char *s, char *out, size_t outlen)
{
	size_t i, o;

	for (i = 0, o = 0; s[i] != '\0' && o + 1 < outlen; i++) {
		if (s[i] == '\\' && i + 4 < strlen(s) && s[i + 1] == '0') {
			unsigned v = 0;
			int j;
			for (j = 1; j <= 3; j++) {
				if (s[i + j] < '0' || s[i + j] > '7')
					break;
				v = v * 8 + (unsigned)(s[i + j] - '0');
			}
			if (j > 1) {
				out[o++] = (char)v;
				i += j;
				continue;
			}
		}
		out[o++] = s[i];
	}
	out[o] = '\0';
	return out;
}

int
getmntinfo(struct statvfs **mntbufp, int flags)
{
	char *line, *tok, *type, *opts;
	const char *src, *mnt;
	char srcbuf[_VFS_MNAMELEN], mntbuf2[_VFS_MNAMELEN];
	struct statvfs *mb, *newmb;
	struct statfs ss;
	FILE *fp;
	int fd, count;
	size_t len, cap;

	(void)flags;	/* MNT_WAIT/MNT_NOWAIT: /proc/self/mounts has no cache */

	fd = open(_NFSPATH, O_RDONLY);
	if (fd == -1)
		return 0;
	fp = fdopen(fd, "r");
	if (fp == NULL) {
		close(fd);
		return 0;
	}

	mb = NULL;
	cap = 0;
	count = 0;
	line = NULL;
	len = 0;
	while (getline(&line, &len, fp) != -1) {
		struct statvfs sv;

		memset(&sv, 0, sizeof(sv));
		tok = strtok(line, " \t\n");
		if (tok == NULL)
			continue;
		src = flx_unescape(tok, srcbuf, sizeof(srcbuf));
		tok = strtok(NULL, " \t\n");
		if (tok == NULL)
			continue;
		mnt = flx_unescape(tok, mntbuf2, sizeof(mntbuf2));
		tok = strtok(NULL, " \t\n");
		if (tok == NULL)
			continue;
		type = tok;
		tok = strtok(NULL, " \t\n");
		opts = tok != NULL ? tok : "";

		(void)strncpy(sv.f_mntfromname, src,
		    sizeof(sv.f_mntfromname) - 1);
		(void)strncpy(sv.f_mntonname, mnt, sizeof(sv.f_mntonname) - 1);
		(void)strncpy(sv.f_fstypename, type,
		    sizeof(sv.f_fstypename) - 1);
		sv.f_mntfromlabel[0] = '\0';
		sv.f_flag = flx_statvfs_flags(opts, type);

		if (FLX_STATFS(mnt, &ss)) {
			sv.f_bsize = ss.f_bsize;
			sv.f_frsize = ss.f_frsize;
			sv.f_iosize = ss.f_bsize;
			sv.f_blocks = ss.f_blocks;
			sv.f_bfree = ss.f_bfree;
			sv.f_bavail = ss.f_bavail;
			sv.f_bresvd = 0;
			sv.f_files = ss.f_files;
			sv.f_ffree = ss.f_ffree;
			sv.f_favail = ss.f_ffree;
			sv.f_fresvd = 0;
			sv.f_fsid = (unsigned long)ss.f_fsid.__val[0];
			sv.f_namemax = ss.f_namelen;
		}

		if ((size_t)count >= cap) {
			cap = cap == 0 ? 32 : cap * 2;
			newmb = realloc(mb, cap * sizeof(*mb));
			if (newmb == NULL)
				break;
			mb = newmb;
		}
		(void)memcpy(&mb[count], &sv, sizeof(*mb));
		count++;
	}
	if (line != NULL)
		free(line);
	fclose(fp);
	*mntbufp = mb;
	return count;
}