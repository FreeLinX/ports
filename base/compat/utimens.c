/* FreeLinX/ports - base/compat : utimens(3)/lutimens(3) for musl.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's usr.bin/touch uses utimens(2)/lutimens(2) to set a file's access
 * and modification times (using the newer per-timespec API).  musl exposes
 * only the POSIX.1-2008 utimensat(2)/futimens(2); it has neither the unadorned
 * utimens() nor the BSD lutimens() name.  These two thin shims map onto
 * utimensat(AT_FDCWD, ...) so the NetBSD touch source compiles and behaves
 * correctly on FreeLinX/Linux (where lutimens follows the BSD contract of
 * acting on the symlink itself and not its target via AT_SYMLINK_NOFOLLOW).
 *
 * Compiled into the touch link set (see the base/touch Makefile).
 */
#include <fcntl.h>
#include <sys/stat.h>

int
utimens(const char *path, const struct timespec times[2])
{
	return utimensat(AT_FDCWD, path, times, 0);
}

int
lutimens(const char *path, const struct timespec times[2])
{
	return utimensat(AT_FDCWD, path, times, AT_SYMLINK_NOFOLLOW);
}
