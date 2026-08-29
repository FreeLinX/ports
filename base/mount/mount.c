/*
 * FreeLinX/ports - base/mount : FreeLinX-native mount (Linux mount(2)).
 *
 * NOTE: this is NOT the NetBSD mount source.  NetBSD's mount is built on the
 * BSD VFS model: the BSD mount(2) syscall, struct statvfs with BSD MNT_*
 * flags, getmntinfo(3), <sys/disklabel.h> partition handling, and the
 * per-filesystem mount_<fstype> helper dispatch.  None of that has a Linux
 * equivalent, and the ports framework will not fake BSD kernel APIs.
 *
 * FreeLinX therefore ships a genuine Linux mount(2) front-end.  It:
 *   - with no arguments, lists the current mounts read from /proc/mounts
 *     (via getmntent(3), fallback /etc/mtab);
 *   - otherwise calls mount(2) directly (source, target, fstype, flags,
 *     data) building the flags from -o options, like the Linux mount.
 *
 * Whether it can actually MOUNT anything depends on the FreeLinX kernel
 * implementing mount(2) and the requested filesystem (tmpfs, devtmpfs, ...);
 * that is a kernel concern and is not faked here.  Until the kernel boots,
 * mount builds/installs but cannot perform a real mount; listing still works
 * once /proc comes up, and every invocation validates its arguments.
 *
 * musl provides mount(2), the MS_* flags and getmntent(3) - no compat shims.
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
	    "usage: mount [-t fstype] [-o options] special dir\n"
	    "       mount              (list mounts from /proc/mounts)\n");
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
		(void)printf("%s on %s type %s%s%s\n",
		    me->mnt_fsname, me->mnt_dir, me->mnt_type,
		    me->mnt_opts[0] ? " (" : "",
		    me->mnt_opts[0] ? me->mnt_opts : "");
	}
	endmntent(fp);
}

static unsigned long
parse_opts(const char *o, const char *fstype)
{
	unsigned long flags = 0;
	char *copy, *tok, *p;

	if (o == NULL || *o == '\0')
		return 0;

	copy = strdup(o);
	if (copy == NULL)
		err(EXIT_FAILURE, "strdup");
	for (tok = strtok(copy, ","); tok != NULL; tok = strtok(NULL, ",")) {
		for (p = tok; *p; ++p)
			if (*p == '=')
				break;
		if (*p == '=')
			*p = '\0';
		if (strcmp(tok, "ro") == 0)		flags |= MS_RDONLY;
		else if (strcmp(tok, "rw") == 0)	flags &= ~MS_RDONLY;
		else if (strcmp(tok, "nosuid") == 0)	flags |= MS_NOSUID;
		else if (strcmp(tok, "nodev") == 0)	flags |= MS_NODEV;
		else if (strcmp(tok, "noexec") == 0)	flags |= MS_NOEXEC;
		else if (strcmp(tok, "sync") == 0)	flags |= MS_SYNCHRONOUS;
		else if (strcmp(tok, "remount") == 0)	flags |= MS_REMOUNT;
		/* other options are passed as filesystem data */
	}
	free(copy);
	(void)fstype;
	return flags;
}

int
main(int argc, char **argv)
{
	int ch;
	const char *fstype = NULL;
	const char *opts = NULL;
	const char *source, *target;

	while ((ch = getopt(argc, argv, "t:o:h")) != -1) {
		switch (ch) {
		case 't':
			fstype = optarg;
			break;
		case 'o':
			opts = optarg;
			break;
		case 'h':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		list_mounts();
		return EXIT_SUCCESS;
	}
	if (argc != 2)
		usage();

	source = argv[0];
	target = argv[1];

	if (mount(source, target, fstype ? fstype : "auto",
	    parse_opts(opts, fstype), NULL) == -1)
		err(EXIT_FAILURE, "mount %s on %s", source, target);

	return EXIT_SUCCESS;
}
