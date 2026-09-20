/* FreeLinX/ports - base/compat/pwcache.c : BSD libutil pwcache(3) stubs.
 *
 * pwcache_userdb(3)/pwcache_groupdb(3) register open/close callbacks and
 * alternate-database lookup functions for libutil's cached user/group
 * lookups.  FreeLinX resolves users/groups with direct getpwnam(3)/
 * getgrnam(3) (musl has no persistent DB handle to manage), so the
 * register calls are successful no-ops.  Used by usr.sbin/mtree
 * getid.c (linked into pax) with the NetBSD 10 4-argument signature.
 */
#include <pwd.h>
#include <grp.h>

int
pwcache_userdb(int (*fn)(int), void (*endfn)(void),
    struct passwd *(*pwfn)(const char *), struct passwd *(*uidfn)(uid_t))
{
	(void)fn;
	(void)endfn;
	(void)pwfn;
	(void)uidfn;
	return 0;
}

int
pwcache_groupdb(int (*fn)(int), void (*endfn)(void),
    struct group *(*grfn)(const char *), struct group *(*gidfn)(gid_t))
{
	(void)fn;
	(void)endfn;
	(void)grfn;
	(void)gidfn;
	return 0;
}