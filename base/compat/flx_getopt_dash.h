/* FreeLinX: BSD getopt(3) semantics for a leading '-' in optstring.
 *
 * NetBSD tools such as su, newgrp, calendar, tset and xstr list '-' in their
 * optstring ("-dflm") so that a lone "-" argument (as in "su -") is returned
 * as option '-'.  musl gives a leading '-' the GNU meaning instead: every
 * non-option argument is returned as option 1, so "su root" fell through to
 * usage().  This wrapper keeps the BSD meaning: it strips the leading '-'
 * and returns '-' for a lone "-" argument itself.
 *
 * Included with -include by the ports that need it (not via flx_bsd.h).
 */
#ifndef _FREELINX_GETOPT_DASH_H
#define _FREELINX_GETOPT_DASH_H
#include <string.h>
#include <unistd.h>

static inline int
flx_getopt_dash(int argc, char *const argv[], const char *optstring)
{
	if (optstring[0] == '-') {
		optstring++;
		if (optind > 0 && optind < argc && argv[optind] != NULL &&
		    strcmp(argv[optind], "-") == 0) {
			optind++;
			return '-';
		}
	}
	return getopt(argc, argv, optstring);
}
#define getopt(c, v, s) flx_getopt_dash((c), (v), (s))

#endif
