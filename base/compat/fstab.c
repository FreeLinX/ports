/* FreeLinX/ports - base/compat/fstab.c : BSD getfsent(3) family on Linux.
 *
 * NetBSD's quota(1) resolves each device to its mount point via getfsspec(3)
 * / getfsfile(3).  Linux keeps the live mount table at /etc/mtab; FreeLinX
 * parses its "spec file vfstype opts freq passno" six-column format - the
 * same layout as BSD fstab(5), so the BSD struct maps 1:1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstab.h"

static struct fstab fs_static;
static FILE *fs_have;

static int
readfields(void)
{
	char buf[2048];
	char *cp, *p, *tok[5];
	int ntok;
	int isro = 0, isrw = 0;
	char *opt;

	if (fs_have == NULL && !setfsent())
		return 0;

	while (fgets(buf, (int)sizeof(buf), fs_have) != NULL) {
		cp = strchr(buf, '\n');
		if (cp == NULL) {
			int c;
			do {
				c = getc(fs_have);
			} while (c != EOF && c != '\n');
			continue;
		}
		*cp = '\0';

		/* split into whitespace-separated up-to-6 fields */
		ntok = 0;
		p = buf;
		while (ntok < 5) {
			while (*p == ' ' || *p == '\t')
				p++;
			if (*p == '\0' || *p == '#')
				break;
			tok[ntok++] = p;
			while (*p != '\0' && *p != ' ' && *p != '\t')
				p++;
			if (*p != '\0')
				*p++ = '\0';
		}

		if (ntok < 3)
			continue;
		if (tok[0][0] != '/' && strcmp(tok[0], "none") != 0 &&
		    strstr(tok[0], "://") == NULL)
			continue;

		fs_static.fs_spec = tok[0];
		fs_static.fs_file = tok[1];
		fs_static.fs_vfstype = tok[2];
		if (ntok > 3) {
			fs_static.fs_mntops = tok[3];
			isro = isrw = 0;
			for (opt = strtok(tok[3], ","); opt != NULL;
			    opt = strtok(NULL, ",")) {
				if (strcmp(opt, "ro") == 0)
					isro = 1;
				else if (strcmp(opt, "rw") == 0)
					isrw = 1;
			}
		} else {
			fs_static.fs_mntops = "";
			isro = isrw = 0;
		}
		fs_static.fs_type = (isro && !isrw) ? "ro" : "rw";
		fs_static.fs_freq = 0;
		fs_static.fs_passno = 0;
		if (ntok > 4 && tok[4][0] != '\0')
			fs_static.fs_freq = (int)strtol(tok[4], NULL, 10);
		return 1;
	}
	return 0;
}

int
setfsent(void)
{
	if (fs_have != NULL) {
		rewind(fs_have);
		return 1;
	}
	fs_have = fopen("/etc/mtab", "r");
	return fs_have != NULL;
}

void
endfsent(void)
{
	if (fs_have != NULL) {
		(void)fclose(fs_have);
		fs_have = NULL;
	}
}

struct fstab *
getfsent(void)
{
	if (readfields())
		return &fs_static;
	return NULL;
}

struct fstab *
getfsspec(const char *name)
{
	struct fstab *fs;

	while ((fs = getfsent()) != NULL)
		if (fs->fs_spec != NULL && strcmp(fs->fs_spec, name) == 0)
			return fs;
	return NULL;
}

struct fstab *
getfsfile(const char *name)
{
	struct fstab *fs;

	while ((fs = getfsent()) != NULL)
		if (fs->fs_file != NULL && strcmp(fs->fs_file, name) == 0)
			return fs;
	return NULL;
}