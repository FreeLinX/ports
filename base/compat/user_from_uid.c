/* FreeLinX/ports - base/compat : user_from_uid(3)/group_from_gid(3) for musl.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's cp, mv, rm and ls display owner/group through the pointer-cache
 * lookup functions user_from_uid(3)/group_from_gid(3) (lib/libc/gen/
 * pwcache.c).  musl has neither the functions nor a BSD-style password/group
 * cache, so none of the NetBSD source can be reused verbatim; this is a
 * FreeLinX stand-in with the same observable contract as NetBSD's pwcache:
 *
 *   user_from_uid(uid, noname):
 *     known uid  -> the user name
 *     unknown    -> NULL if noname, else the numeric uid as a string
 *
 * and the mirror for group_from_gid().  The pwcache contract says the
 * returned string stays valid until the next call; these tools use the
 * result immediately (strcpy into ls's NAMES, a single fprintf in mv/rm),
 * so a small static buffer is sufficient.  Implemented on musl's
 * getpwuid(3)/getgrgid(3).
 *
 * Compiled into the cp/mv/rm/ls link sets.
 */
#include <sys/types.h>

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

const char *
user_from_uid(uid_t uid, int noname)
{
	struct passwd *pw;
	static char namebuf[32];

	if ((pw = getpwuid(uid)) != NULL && pw->pw_name[0] != '\0')
		return pw->pw_name;
	if (noname)
		return NULL;
	(void)snprintf(namebuf, sizeof(namebuf), "%lu", (unsigned long)uid);
	return namebuf;
}

const char *
group_from_gid(gid_t gid, int noname)
{
	struct group *gr;
	static char namebuf[32];

	if ((gr = getgrgid(gid)) != NULL && gr->gr_name[0] != '\0')
		return gr->gr_name;
	if (noname)
		return NULL;
	(void)snprintf(namebuf, sizeof(namebuf), "%lu", (unsigned long)gid);
	return namebuf;
}