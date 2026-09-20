/* FreeLinX/ports - base/compat/paxstub.c : libutil helpers for base/pax.
 *
 * pax links usr.sbin/mtree/getid.c which provides its own setup_getid(),
 * so the full sigstub.c cannot be linked (duplicate symbol).  These are the
 * remaining libutil functions pax references via getid.c/spec.c, implemented
 * as real musl backends or honest stubs:
 *   - uid_from_user/gid_from_group : getpwnam/getgrnam lookups
 *   - raise_default_signal         : kill with default disposition
 *   - string_to_flags/flags_to_string : FreeLinX stores no chflags(2)
 *     metadata; empty flag set (mirrors sigstub.c)
 */
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>

int
uid_from_user(const char *name, uid_t *uidp)
{
	struct passwd *pw;

	if (name == NULL)
		return -1;
	pw = getpwnam(name);
	if (pw == NULL)
		return -1;
	*uidp = pw->pw_uid;
	return 0;
}

int
gid_from_group(const char *name, gid_t *gidp)
{
	struct group *gr;

	if (name == NULL)
		return -1;
	gr = getgrnam(name);
	if (gr == NULL)
		return -1;
	*gidp = gr->gr_gid;
	return 0;
}

int
raise_default_signal(int sig)
{
	struct sigaction sa, osa;
	sigset_t nmask, omask;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	if (sigaction(sig, &sa, &osa) == -1)
		return -1;
	sigemptyset(&nmask);
	sigaddset(&nmask, sig);
	sigprocmask(SIG_UNBLOCK, &nmask, &omask);
	(void)kill(getpid(), sig);
	sigprocmask(SIG_SETMASK, &omask, NULL);
	(void)sigaction(sig, &osa, NULL);
	return 0;
}

int
string_to_flags(char **stringp, unsigned long *setp, unsigned long *clrp)
{
	if (stringp != NULL)
		*stringp = NULL;
	if (setp != NULL)
		*setp = 0;
	if (clrp != NULL)
		*clrp = 0;
	return 0;
}

char *
flags_to_string(unsigned long flags, const char *def)
{
	return (flags == 0 && def != NULL) ? (char *)def : "";
}