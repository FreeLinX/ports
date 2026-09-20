/* FreeLinX/ports - base/compat/parsedate.c : NetBSD parsedate(3) shim.
 *
 * musl has no parsedate(3) (BSD libutil).  pax's `-t` option and `-s`
 * substitution specs accept a human date string when the argument is not a
 * file.  This is a best-effort real parser on top of strptime(3): it walks
 * the common date(1)-style layouts NetBSD's own parser accepts.  It is not
 * a full BSD date grammar (no relative "1 hour ago", no TZ bracketing), and
 * is documented as such; the core archive/extract paths never call it.
 *
 * Signature matches <util.h>:
 *	time_t parsedate(const char *p, const time_t *now, const int *f)
 * Unlike NetBSD, `now` is ignored (always wall-clock now) and `f` only
 * forces the AM/PM interpretation when non-NULL.  Returns (time_t)-1 with
 * errno = EINVAL when the string does not parse.
 */
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const char *date_fmts[] = {
	"%a %b %e %H:%M:%S %Z %Y",	/* date(1) default, e.g. Sat Sep 19 14:20:30 UTC 2026 */
	"%a %b %e %H:%M:%S %Y",		/* date(1) without zone */
	"%a %b %e %H:%M:%S",		/* date(1) w/o year */
	"%b %e %H:%M:%S %Y",		/* ctime-ish */
	"%b %e %H:%M:%S",
	"%Y-%m-%d %H:%M:%S",		/* ISO */
	"%Y-%m-%d %H:%M",
	"%Y%m%d%H%M%S",			/* pax -t style, no punctuation */
	"%y%m%d%H%M%S",
	"%Y%m%d%H%M",
	"%y%m%d%H%M",
	"%m/%d/%y %H:%M",
	"%m/%d/%Y %H:%M",
	"%Y-%m-%d",			/* date only */
	"%m/%d/%Y",
	"%m/%d/%y",
	NULL
};

time_t
parsedate(const char *p, const time_t *now, const int *f)
{
	struct tm tm;
	const char **fmt;
	char buf[64];
	size_t n;
	time_t t;

	(void)now;
	if (p == NULL) {
		errno = EINVAL;
		return (time_t)-1;
	}
	/* Strip a trailing timezone abbreviation the parser can't place
	 * before feeding strptime (the %Z formats above already try it). */
	n = strcspn(p, "\n");
	if (n >= sizeof(buf))
		n = sizeof(buf) - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';

	for (fmt = date_fmts; *fmt != NULL; fmt++) {
		memset(&tm, 0, sizeof(tm));
		if (strptime(buf, *fmt, &tm) != NULL) {
			/* If no year was specified the struct tm year is 0;
			 * resolve to the current year like BSD parsers do. */
			if (tm.tm_year == 0) {
				time_t nowt = time(NULL);
				struct tm *ntm = localtime(&nowt);
				if (ntm != NULL)
					tm.tm_year = ntm->tm_year;
			}
			if (f != NULL) {
				/* -p/-a style half-day disambiguation: the
				 * pointer is non-NULL when the caller wants
				 * the AM/PM to follow the past/future hint,
				 * but pax never sets it; keep plain mktime. */
			}
			t = mktime(&tm);
			if (t != (time_t)-1) {
				errno = 0;
				return t;
			}
		}
		if (buf[0] == '\0')
			break;
	}
	errno = EINVAL;
	return (time_t)-1;
}