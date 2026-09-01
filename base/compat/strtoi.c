/* FreeLinX/ports - base/compat/strtoi.c : BSD strtoi(3)/strtou(3).
 *
 * NetBSD's nc and friends parse option values with strtoi(3)/strtou(3):
 * bounded conversions that clamp to [lo,hi] and report ECANCELED (no
 * digits), ENOTSUP (trailing junk) or ERANGE (out of range) through the
 * optional status pointer, without disturbing errno on success.  musl has
 * only the unbounded strtoimax/strtoumax, so FreeLinX provides the BSD
 * pair with exactly the NetBSD contract (mirrors NetBSD's _strtoi.h
 * implementation from src.lib/libc/stdlib).
 */
#include <errno.h>
#include <inttypes.h>
#include <stddef.h>

#define strtoi_impl_FUNCNAME strtoi
#define strtoi_impl_TYPE intmax_t
#define strtoi_impl_WRAPPED strtoimax

intmax_t
strtoi_impl_FUNCNAME(const char * __restrict nptr, char ** __restrict endptr,
    int base, intmax_t lo, intmax_t hi, int *rstatus)
{
	int serrno;
	intmax_t im;
	char *ep;
	int rep;

	if (endptr == NULL)
		endptr = &ep;
	if (rstatus == NULL)
		rstatus = &rep;

	serrno = errno;
	errno = 0;

	im = strtoi_impl_WRAPPED(nptr, endptr, base);

	*rstatus = errno;
	errno = serrno;

	if (*rstatus == 0) {
		/* No digits were found */
		if (nptr == *endptr)
			*rstatus = ECANCELED;
		/* There are further characters after number */
		else if (**endptr != '\0')
			*rstatus = ENOTSUP;
	}

	if (im < lo) {
		if (*rstatus == 0)
			*rstatus = ERANGE;
		return lo;
	}
	if (im > hi) {
		if (*rstatus == 0)
			*rstatus = ERANGE;
		return hi;
	}

	return im;
}

#undef strtoi_impl_FUNCNAME
#undef strtoi_impl_TYPE
#undef strtoi_impl_WRAPPED

uintmax_t
strtou(const char * __restrict nptr, char ** __restrict endptr, int base,
    uintmax_t lo, uintmax_t hi, int *rstatus)
{
	int serrno;
	uintmax_t um;
	char *ep;
	int rep;

	if (endptr == NULL)
		endptr = &ep;
	if (rstatus == NULL)
		rstatus = &rep;

	serrno = errno;
	errno = 0;

	um = strtoumax(nptr, endptr, base);

	*rstatus = errno;
	errno = serrno;

	if (*rstatus == 0) {
		/* No digits were found */
		if (nptr == *endptr)
			*rstatus = ECANCELED;
		/* There are further characters after number */
		else if (**endptr != '\0')
			*rstatus = ENOTSUP;
	}

	if (um < lo) {
		if (*rstatus == 0)
			*rstatus = ERANGE;
		return lo;
	}
	if (um > hi) {
		if (*rstatus == 0)
			*rstatus = ERANGE;
		return hi;
	}

	return um;
}