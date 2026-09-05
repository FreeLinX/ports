/*
 * FreeLinX/ports - base/compat/sysctl.c : minimal sysctl(3).
 *
 * Backs only the CTL_USER/USER_CS_PATH query used by usr.bin/whereis,
 * answered from the compiler's _CS_PATH.  All other MIBs fail ENOTSUP.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sysctl.h>

int
sysctl(const int *name, u_int namelen, void *oldp, size_t *oldlenp,
    const void *newp, size_t newlen)
{
	size_t req;
	size_t got;
	char cs[1024];

	(void)newp;
	(void)newlen;

	if (name == NULL || namelen < 2 || name[0] != CTL_USER)
		goto unsup;
	if (name[1] != USER_CS_PATH)
		goto unsup;

	got = confstr(_CS_PATH, cs, sizeof(cs));
	if (got == 0 || got >= sizeof(cs))
		goto unsup;
	req = strlen(cs) + 1;

	if (oldp == NULL) {
		*oldlenp = req;
		return 0;
	}
	if (*oldlenp < req) {
		errno = ENOMEM;
		return -1;
	}
	memcpy(oldp, cs, req);
	*oldlenp = req;
	return 0;

 unsup:
	errno = ENOTSUP;
	return -1;
}