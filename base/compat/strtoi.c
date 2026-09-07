/* FreeLinX/ports - base/compat/strtoi.c : NetBSD strtoi(3)/strtou(3).
 *
 * musl lacks the NetBSD strtoi/strtou integer-parsing family.  These wrap
 * strtoimax/strtoumax and clamp to lo..hi, returning ECANCELED via rstatus
 * on range error (matching NetBSD semantics: if rstatus is non-NULL it
 * receives ERANGE on range failure, ECANCELED if the result had to be
 * clamped).
 */
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

intmax_t
strtoi(const char *nptr, char **endptr, int base, intmax_t lo, intmax_t hi, int *rstatus)
{
	intmax_t val;
	int status = 0;

	if (endptr != NULL)
		*endptr = (char *)nptr;

	val = strtoimax(nptr, endptr, base);
	if (errno == EINVAL) {
		status = EINVAL;
	} else {
		if (errno == ERANGE || val < lo || val > hi) {
			status = ECANCELED;
			val = val < lo ? lo : hi;
		}
	}
	if (rstatus != NULL)
		*rstatus = status;
	return val;
}

uintmax_t
strtou(const char *nptr, char **endptr, int base, uintmax_t lo, uintmax_t hi, int *rstatus)
{
	uintmax_t val;
	int status = 0;

	if (endptr != NULL)
		*endptr = (char *)nptr;

	val = strtoumax(nptr, endptr, base);
	if (errno == EINVAL) {
		status = EINVAL;
	} else {
		if (errno == ERANGE || val < lo || val > hi) {
			status = ECANCELED;
			val = val < lo ? lo : hi;
		}
	}
	if (rstatus != NULL)
		*rstatus = status;
	return val;
}