/* FreeLinX/ports - base/compat/utmpentry.c : getutentries(3) for musl.
 *
 * FreeLinX implementation of the NetBSD who(1) utmpentry library (the
 * netbsd usr.bin/who version) on musl's <utmpx.h>.  Exposes
 * getutentries()/endutentries() with the same linked-list contract used by
 * users/wall/write.
 */
#include <sys/types.h>
#include <sys/time.h>
#include <utmpx.h>
#include <stdlib.h>
#include <string.h>

#include "utmpentry.h"

size_t maxname = WHO_NAME_LEN;
size_t maxline = WHO_LINE_LEN;
size_t maxhost = WHO_HOST_LEN;
int etype;

size_t
getutentries(const char *path, struct utmpentry **rethead)
{
	struct utmpentry *head = NULL, *tail = NULL;
	struct utmpx *u;
	size_t n = 0;

	if (path != NULL)
		utmpxname(path);

	setutxent();
	while ((u = getutxent()) != NULL) {
		struct utmpentry *e;

		if (u->ut_type != USER_PROCESS &&
		    u->ut_type != LOGIN_PROCESS &&
		    u->ut_type != INIT_PROCESS)
			continue;

		e = calloc(1, sizeof(*e));
		if (e == NULL)
			break;

		(void)strlcpy(e->name, u->ut_user, sizeof(e->name));
		(void)strlcpy(e->line, u->ut_line, sizeof(e->line));
		(void)strlcpy(e->host, u->ut_host, sizeof(e->host));
		e->tv = u->ut_tv;
		e->pid = (pid_t)u->ut_pid;
		e->term = (uint16_t)u->ut_exit.e_termination;
		e->exit = (uint16_t)u->ut_exit.e_exit;
		e->sess = (uint16_t)u->ut_session;
		e->type = (uint16_t)u->ut_type;
		e->next = NULL;

		if (tail != NULL)
			tail->next = e;
		else
			head = e;
		tail = e;
		n++;
	}
	endutxent();

	*rethead = head;
	return n;
}

void
endutentries(void)
{
	/* The list is intentionally not cached/freed (see users(1)). */
}