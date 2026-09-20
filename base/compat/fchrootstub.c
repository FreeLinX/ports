/* FreeLinX musl-compat - fchroot(2) shim.
 *
 * BSD pax (and other BSD tools) use fchroot() to securely pivot into an
 * extracted archive tree.  Linux has no fchroot syscall; the FreeLinX model
 * never relies on it, so this shim lets the code build/run while failing
 * cleanly if a tool path actually attempts it.
 */
#include <unistd.h>
#include <errno.h>

int fchroot(int fd)
{
	(void)fd;
	errno = EPERM;
	return -1;
}