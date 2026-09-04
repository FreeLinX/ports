/* FreeLinX/ports - base/compat/utmpentry.h : NetBSD utmpentry(3) shim.
 *
 * FreeLinX stand-in for NetBSD's <utmpentry.h> (usr.bin/who), using
 * musl's <utmpx.h> instead of the SUPPORT_UTMPX/#error config dance.
 * Provides the struct + getutentries/endutentries used by users/wall/
 * write; the implementation is compat/utmpentry.c.
 */
#ifndef _FREELINX_UTMPENTRY_H_
#define _FREELINX_UTMPENTRY_H_

#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>
#include <utmpx.h>
#include <utmp.h>

#define	WHO_NAME_LEN	UT_NAMESIZE
#define	WHO_LINE_LEN	UT_LINESIZE
#define	WHO_HOST_LEN	UT_HOSTSIZE

struct utmpentry {
	char name[WHO_NAME_LEN + 1];
	char line[WHO_LINE_LEN + 1];
	char host[WHO_HOST_LEN + 1];
	struct timeval tv;
	pid_t pid;
	uint16_t term;
	uint16_t exit;
	uint16_t sess;
	uint16_t type;
	struct utmpentry *next;
};

extern size_t maxname, maxline, maxhost;
extern int etype;

size_t getutentries(const char *, struct utmpentry **);
void endutentries(void);

#endif /* !_FREELINX_UTMPENTRY_H_ */