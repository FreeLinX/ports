/*
 * FreeLinX/ports - base/compat/hesiod.c : hesiod(3) client stub.
 *
 * mir://hesiod does not exist on FreeLinX hosts; the API keeps the
 * exact NetBSD contract so hesinfo(1) builds and runs.  As on NetBSD,
 * hesinfo_init(3) refuses to initialise when /etc/hesiod.conf is
 * missing; if it is present the resolver behaves as a configured zone
 * that has no answers for anything (hesinfo prints "Hesiod name not
 * found"), and hesiod_to_bind() composes the textual name/type string
 * (deallocated by hesiod_free_list()).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <hesiod.h>

#define	_PATH_HESIODCONF	"/etc/hesiod.conf"

struct hesiod_ctx {
	char	rhs[256];
};

int
hesiod_init(void **context)
{
	struct hesiod_ctx *c;

	if (context == NULL)
		return -1;
	c = malloc(sizeof(*c));
	if (c == NULL)
		return -1;
	c->rhs[0] = '\0';
	/* NetBSD semantics: no config file means no Hesiod service. */
	if (access(_PATH_HESIODCONF, R_OK) != 0) {
		free(c);
		return -1;
	}
	*context = c;
	return 0;
}

char *
hesiod_to_bind(void *context, const char *name, const char *type)
{
	char *s;

	(void)context;
	if (name == NULL || type == NULL)
		return NULL;
	s = malloc(strlen(name) + 1 + strlen(type) + 1);
	if (s == NULL)
		return NULL;
	sprintf(s, "%s.%s", name, type);
	return s;
}

char **
hesiod_resolve(void *context, const char *name, const char *type)
{
	(void)context;
	(void)name;
	(void)type;
	return NULL;
}

void
hesiod_free_list(void *context, char **list)
{
	(void)context;
	if (list == NULL)
		return;
	free(list);
}

void
hesiod_end(void *context)
{
	free(context);
}