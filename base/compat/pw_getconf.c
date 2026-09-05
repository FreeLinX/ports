/*
 * FreeLinX/ports - base/compat/pw_getconf.c : pw_getconf(3).
 *
 * Answers "class name" queries from /etc/passwd.conf in the NetBSD
 * file format:
 *
 *   default:localcipher=md5,subfile
 *   user:localcipher=sha1
 *
 * Searches the requested class, then "default", then "*" (NetBSD
 * order).  Returns 1 on a match (value copied into buf), 0 when the
 * key is absent, -1 on read errors.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <login_cap.h>

#define	_PATH_PASSWDCONF	"/etc/passwd.conf"

static int
class_hits_line(const char *line, const char *class)
{
	const char *p = line;

	if (class[0] == '*')
		return 1;
	if (p[0] == ' ')
		p++;
	for (;;) {
		size_t clen = strlen(class);

		if (strncmp(p, class, clen) == 0 &&
		    (p[clen] == ':' || p[clen] == '|'))
			return 1;
		p = strchr(p, '|');
		if (p == NULL)
			return 0;
		p++;
	}
}

static int
lookup_cap(const char *line, const char *name, size_t klen,
    char *buf, size_t buflen)
{
	const char *cap = line;
	const char *eq;

	while ((cap = strstr(cap, name)) != NULL) {
		if ((cap == line || *(cap - 1) == ':') &&
		    cap[klen] == '=') {
			eq = cap + klen + 1;
			while (*eq != '\0' && *eq != ',') {
				if (buflen == 0)
					break;
				*buf++ = *eq++;
				buflen--;
			}
			*buf = '\0';
			return 1;
		}
		cap += klen;
	}
	return 0;
}

int
pw_getconf(char *buf, size_t buflen, const char *class, const char *name)
{
	FILE *fp;
	char line[2048];
	const char *classes[3];
	size_t klen = strlen(name);
	int i;
	int got = 0;

	classes[0] = class;
	classes[1] = "default";
	classes[2] = "*";

	if (buflen == 0)
		return -1;
	buf[0] = '\0';

	fp = fopen(_PATH_PASSWDCONF, "r");
	if (fp == NULL)
		return 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		char *p;

		p = strchr(line, '\n');
		if (p != NULL)
			*p = '\0';
		if (line[0] == '#' || line[0] == '\0')
			continue;
		for (i = 0; i < 3; i++) {
			if (class_hits_line(line, classes[i])) {
				if (lookup_cap(line, name, klen, buf,
				    buflen)) {
					got = 1;
					goto out;
				}
				break;
			}
		}
	}
out:
	fclose(fp);
	return got ? 1 : 0;
}