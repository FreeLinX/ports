/* FreeLinX/ports - base/compat/fparseln.c : fparseln(3) for musl.
 *
 * Port of the NetBSD libc fparseln(3) implementation to musl stdio.
 * The NetBSD source relies on fgetln(3) and <reentrant.h>; this build
 * substitutes getline(3) and drops the libc flocking.  The parsing
 * algorithm (comment / continuation / escape semantics, FPARSELN_* flag
 * handling) is untouched.  The FPARSELN_* macros live in flx_bsd.h.
 */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
isescaped(const char *sp, const char *p, int esc)
{
	const char *cp;
	size_t ne;

	if (esc == '\0')
		return 0;

	for (ne = 0, cp = p; --cp >= sp && *cp == esc; ne++)
		continue;

	return (ne & 1) != 0;
}

char *
fparseln(FILE *fp, size_t *size, size_t *lineno, const char str[3], int flags)
{
	static const char dstr[3] = { '\\', '\\', '#' };

	size_t s, len;
	char   *buf;
	char   *ptr, *cp;
	int	cnt;
	char	esc, con, nl, com;
	char   *line = NULL;
	size_t linesize = 0;
	ssize_t lr;

	len = 0;
	buf = NULL;
	cnt = 1;

	if (str == NULL)
		str = dstr;

	esc = str[0];
	con = str[1];
	com = str[2];
	nl  = '\n';

	while (cnt) {
		cnt = 0;

		if (lineno)
			(*lineno)++;

		if ((lr = getline(&line, &linesize, fp)) == -1)
			break;

		s = (size_t)lr;

		if (s && com) {		/* Check and eliminate comments */
			for (cp = line; cp < line + s; cp++)
				if (*cp == com && !isescaped(line, cp, esc)) {
					s = (size_t)(cp - line);
					cnt = s == 0 && buf == NULL;
					break;
				}
		}

		if (s && nl) {		/* Check and eliminate trailing newline */
			cp = &line[s - 1];

			if (*cp == nl)
				s--;
		}

		if (s && con) {		/* Check and eliminate continuations */
			cp = &line[s - 1];

			if (*cp == con && !isescaped(line, cp, esc)) {
				s--;
				cnt = 1;
			}
		}

		if (s == 0) {
			if (cnt || buf != NULL)
				continue;
		}

		if ((cp = realloc(buf, len + s + 1)) == NULL) {
			free(buf);
			buf = NULL;
			break;
		}
		buf = cp;

		(void) memcpy(buf + len, line, s);
		len += s;
		buf[len] = '\0';
	}

	free(line);

	if ((flags & FPARSELN_UNESCALL) != 0 && esc && buf != NULL &&
	    strchr(buf, esc) != NULL) {
		ptr = cp = buf;
		while (cp[0] != '\0') {
			int skipesc;

			while (cp[0] != '\0' && cp[0] != esc)
				*ptr++ = *cp++;
			if (cp[0] == '\0' || cp[1] == '\0') {
				if (cp[0] != '\0')
					*ptr++ = *cp++;
				break;
			}

			skipesc = 0;
			if (cp[1] == com)
				skipesc += (flags & FPARSELN_UNESCCOMM);
			if (cp[1] == con)
				skipesc += (flags & FPARSELN_UNESCCONT);
			if (cp[1] == esc)
				skipesc += (flags & FPARSELN_UNESCESC);
			if (cp[1] != com && cp[1] != con && cp[1] != esc)
				skipesc = (flags & FPARSELN_UNESCREST);

			if (skipesc)
				cp++;
			else
				*ptr++ = *cp++;
			*ptr++ = *cp++;
		}
		*ptr = '\0';
		len = strlen(buf);
	}

	if (size)
		*size = len;
	return buf;
}