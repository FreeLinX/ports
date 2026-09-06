/* getttynam.c - FreeLinX: NetBSD getttynam(3) using a minimal /etc/ttys
 * parser.  Kept simple: the field we care about most (ty_speed/ty_status)
 * defaults safely when /etc/ttys is absent or malformed. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ttyent {
	char *ty_name;
	char *ty_getty;
	char *ty_type;
	char *ty_comment;
	uint32_t ty_status;
	int ty_speed;
	int ty_baudrate;
};

#define TTY_ON		0x0001
#define TTY_SECURE	0x0002
#define TTY_DIALUP	0x0004
#define TTY_NETWORK	0x0008
#define TTY_NOCARRIER	0x0010
#define TTY_EXTERNAL	0x0020
#define TTY_DISABLED	0x0040
#define TTY_IFCONNECTED	0x0080

static struct ttyent ent;

static void
freeent(void)
{
	free(ent.ty_name);
	free(ent.ty_getty);
	free(ent.ty_type);
	free(ent.ty_comment);
	memset(&ent, 0, sizeof(ent));
}

struct ttyent *
getttynam(const char *name)
{
	FILE *f;
	char buf[256], *p;

	if (name == NULL || name[0] == '\0')
		return NULL;
	freeent();
	f = fopen("/etc/ttys", "r");
	if (f == NULL)
		return NULL;
	while (fgets(buf, sizeof(buf), f) != NULL) {
		p = buf;
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '#' || *p == '\n')
			continue;
		/* tty name (strip leading /dev/ and 'tty' prefix) */
		if (strchr(p, ' ') != NULL)
			*strchr(p, ' ') = '\0';
		if (p[0] == '\0')
			continue;
		ent.ty_name = strdup(p + (strncmp(p, "/dev/", 5) == 0 ? 5 : 0));
		if (strcmp(ent.ty_name, name) == 0) {
			fclose(f);
			ent.ty_speed = 0;
			ent.ty_baudrate = 0;
			ent.ty_status = 0;
			return &ent;
		}
	}
	fclose(f);
	freeent();
	return NULL;
}
