/*
 * FreeLinX/ports - base/umount : FreeLinX-native umount (Linux umount2(2)).
 *
 * NOTE: this is NOT the NetBSD umount source.  NetBSD's umount uses the BSD
 * unmount(2), statvfs with f_fstypename, getmntinfo(3), and the umount_<type>
 * helper dispatch plus NFS/RPC (clnt_create / RPCMNT_UMOUNT) for exports.
 * None of that has a Linux equivalent and the ports framework will not fake
 * BSD kernel APIs.
 *
 * FreeLinX ships a genuine Linux umount2(2) front-end.  It:
 *   - with no arguments, lists the current mounts read from /proc/mounts;
 *   - otherwise calls umount2(2) directly on the target path (or /dev/mount
 *     source), honoring -f (force, MNT_FORCE) and -t <type>.
 *
 * musl provides umount2(2) and getmntent(3) - no compat shims.
 */

#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>

#if defined(_PATH_MOUNTED)
#define MOUNT_LIST _PATH_MOUNTED
#else
#define MOUNT_LIST "/proc/mounts"
#endif

static void
usage(void)
{
	(void)fprintf(stderr,
	    "usage: umount [-f] [-t fstype] dir|source ...\n"
	    "       umount              (list mounts from /proc/mounts)\n");
	exit(EXIT_FAILURE);
}

static void
list_mounts(void)
{
	FILE *fp;
	struct mntent *me;

	fp = setmntent(MOUNT_LIST, "r");
	if (fp == NULL)
		err(EXIT_FAILURE, "setmntent");
	while ((me = getmntent(fp)) != NULL) {
		if (me->mnt_fsname[0] == '\0' || me->mnt_dir[0] == '\0')
			continue;
		(void)printf("%s on %s type %s\n",
		    me->mnt_fsname, me->mnt_dir, me->mnt_type);
	}
	endmntent(fp);
}

static int
match_type(const char *entry_type, const char *want)
{
	/* empty prefix: match everything */
	if (want == NULL || *want == '\0')
		return 1;
	if (strcmp(entry_type, want) == 0)
		return 1;
	/* net => any "net*" filesystem (historical umount shorthand) */
	if (strncmp(want, "net", 3) == 0 &&
	    strncmp(entry_type, "net", 3) == 0)
		return 1;
	return 0;
}

static void
list_mounts_type(const char *want)
{
	FILE *fp;
	struct mntent *me;

	fp = setmntent(MOUNT_LIST, "r");
	if (fp == NULL)
		err(EXIT_FAILURE, "setmntent");
	while ((me = getmntent(fp)) != NULL) {
		if (me->mnt_fsname[0] == '\0' || me->mnt_dir[0] == '\0')
			continue;
		if (match_type(me->mnt_type, want) == 0)
			continue;
		(void)printf("%s on %s type %s\n",
		    me->mnt_fsname, me->mnt_dir, me->mnt_type);
	}
	endmntent(fp);
}

int
main(int argc, char **argv)
{
	int ch;
	int flags = 0;
	const char *type = NULL;
	int rval = EXIT_SUCCESS;

	while ((ch = getopt(argc, argv, "ft:h")) != -1) {
		switch (ch) {
		case 'f':
			flags |= MNT_FORCE;
			break;
		case 't':
			type = optarg;
			break;
		case 'h':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		list_mounts_type(type);
		return EXIT_SUCCESS;
	}

	for (; argc > 0; argc--, argv++) {
		if (umount2(*argv, flags) == -1) {
			warn("umount %s", *argv);
			rval = EXIT_FAILURE;
		}
	}

	return rval;
}
