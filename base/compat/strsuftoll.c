/* FreeLinX/ports - base/compat/strsuftoll.c : NetBSD strsuftoll(3).
 *
 * NetBSD's dd parses its size/skip/count arguments through strsuftoll(3),
 * which accepts an optional trailing multiplier suffix (b/k/m/g/t/p/e,
 * case-insensitive, 512/1024^1..1024^6).  musl has no such libc function;
 * FreeLinX provides this implementation.  The BSD original returns a char *
 * (pointer past the parse); every FreeLinX caller (bin/dd/args.c among
 * others) assigns the result directly to a numeric field, so this shim
 * returns the parsed value.  Failure behaviour matches dd's expectation:
 * strsuftoll() prints "<hint>: <arg>: ..." via errx(3) and exits;
 * strsuftollx() records the same message in the caller's buffer and
 * returns 0.
 */
#include <err.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long
strsuftoll_int(const char *hint, const char *arg, long long min, long long max,
    char *errbuf, size_t errbufsz)
{
	unsigned long long val, mult;
	long long sval;
	const char *p, *dig;
	char *end;
	int sign = 1;
	int have_suffix = 0;
	int bad = 0;

	p = arg;
	while (isspace((unsigned char)*p))
		p++;
	if (*p == '+' || *p == '-') {
		if (*p == '-')
			sign = -1;
		p++;
	}

	dig = p;
	val = strtoull(dig, &end, 0);
	if (end == dig)
		bad = 1;
	else if (val == ULLONG_MAX && sign > 0)
		bad = 1;
	else {
		switch (tolower((unsigned char)*end)) {
		case 'b':	mult = 512;		have_suffix = 1; break;
		case 'k':	mult = 1024ULL;		have_suffix = 1; break;
		case 'm':	mult = 1024ULL*1024;	have_suffix = 1; break;
		case 'g':	mult = 1024ULL*1024*1024; have_suffix = 1; break;
		case 't':	mult = 1024ULL*1024*1024*1024;	have_suffix = 1; break;
		case 'p':	mult = 1024ULL*1024*1024*1024*1024; have_suffix = 1; break;
		case 'e':	mult = 1024ULL*1024*1024*1024*1024*1024; have_suffix = 1; break;
		default:	mult = 1; break;
		}
		if (have_suffix) {
			const char *tail = end + 1;
			while (isspace((unsigned char)*tail))
				tail++;
			if (*tail != '\0')
				bad = 1;
			if (!bad) {
				if (val > ULLONG_MAX / mult)
					bad = 1;
				else
					val *= mult;
			}
		} else {
			const char *tail = end;
			while (isspace((unsigned char)*tail))
				tail++;
			if (*tail != '\0')
				bad = 1;
		}
	}

	if (bad) {
		if (errbuf != NULL) {
			snprintf(errbuf, errbufsz, "%s: %s: invalid number",
			    hint, arg);
			return 0;
		}
		errx(1, "%s: %s: invalid number", hint, arg);
	}

	if (sign < 0) {
		if (val > (unsigned long long)LLONG_MAX + 1) {
			goto toobig;
		}
		sval = (val == (unsigned long long)LLONG_MAX + 1) ?
		    LLONG_MIN : -(long long)val;
	} else {
		if (val > (unsigned long long)LLONG_MAX)
			goto toobig;
		sval = (long long)val;
	}
	if (sval < min || sval > max)
		goto outofrange;

	return sval;

toobig:
	if (errbuf != NULL) {
		snprintf(errbuf, errbufsz, "%s: %s: value too large", hint, arg);
		return 0;
	}
	errx(1, "%s: %s: value too large", hint, arg);

outofrange:
	if (errbuf != NULL) {
		snprintf(errbuf, errbufsz, "%s: %s: value out of range", hint, arg);
		return 0;
	}
	errx(1, "%s: %s: value out of range", hint, arg);
}

long long
strsuftoll(const char *hint, const char *arg, long long min, long long max)
{
	return strsuftoll_int(hint, arg, min, max, NULL, 0);
}

long long
strsuftollx(const char *hint, const char *arg, long long min, long long max,
    char *errbuf, size_t errbufsz)
{
	long long rv;

	rv = strsuftoll_int(hint, arg, min, max, errbuf, errbufsz);
	if (rv == 0 && errbuf != NULL && errbuf[0] == '\0')
		(void)snprintf(errbuf, errbufsz, "%s: %s: invalid number",
		    hint, arg);
	return rv;
}