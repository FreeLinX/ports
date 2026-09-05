/*
 * FreeLinX/ports - base/compat/devname.c : devname(3).
 *
 * musl has no devname(3).  It resolves a dev_t to a /dev entry name by
 * scanning /dev for a matching major:minor.  Used by lastcomm.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define	DEVNAMELEN	((sizeof("maj:min") - 1) * 256)

char *
devname(dev_t device, mode_t type)
{
	static char buf[32];
	DIR *dp;
	struct dirent *de;
	struct stat st;

	(void)type;
	dp = opendir("/dev");
	if (dp == NULL) {
		snprintf(buf, sizeof(buf), "%.8x:%.8x",
		    (unsigned)(device >> 8), (unsigned)(device & 0xff));
		return buf;
	}
	while ((de = readdir(dp)) != NULL) {
		char path[512];

		if (de->d_name[0] == '.')
			continue;
		snprintf(path, sizeof(path), "/dev/%s", de->d_name);
		if (stat(path, &st) != 0)
			continue;
		if (S_ISCHR(st.st_mode) && st.st_rdev == device) {
			closedir(dp);
			return strdup(de->d_name);
		}
		if (S_ISBLK(st.st_mode) && st.st_rdev == device) {
			closedir(dp);
			return strdup(de->d_name);
		}
	}
	closedir(dp);
	snprintf(buf, sizeof(buf), "%.8x:%.8x",
	    (unsigned)(device >> 8), (unsigned)(device & 0xff));
	return buf;
}