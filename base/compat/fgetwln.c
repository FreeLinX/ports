/*
 * FreeLinX/ports - base/compat : BSD fgetwln(3) for musl.
 *
 * usr.bin/fmt reads input as wide lines (fgetwln).  musl has no wide
 * line reader; implement it with fgetwc() with BSD semantics: return a
 * pointer to a static, caller-owned buffer holding the next wide line
 * INCLUDING the trailing newline, and store the number of wide chars
 * (newline included) in *len.  Returns NULL at EOF with no characters.
 * The static buffer is overwritten on each call (matching BSD fgetln()).
 */

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t *
fgetwln(FILE *stream, size_t *len)
{
	static wchar_t *buf;
	static size_t buflen;
	size_t used = 0;
	wint_t c;

	for (;;) {
		if (used + 2 > buflen) {
			size_t n = buflen ? buflen * 2 : 128;
			wchar_t *nb = realloc(buf, n * sizeof(*buf));
			if (nb == NULL) {
				*len = 0;
				return NULL;
			}
			buf = nb;
			buflen = n;
		}
		c = fgetwc(stream);
		if (c == WEOF) {
			if (used == 0) {
				*len = 0;
				return NULL;
			}
			break;
		}
		buf[used++] = (wchar_t)c;
		if (c == L'\n')
			break;
	}
	*len = used;
	buf[used] = L'\0';
	return buf;
}

/*
 * RFC-822-style header-line detector in wide form, the definition
 * NetBSD-libc supplies for usr.bin/fmt's `int ishead(const wchar_t *)`.
 * Returns 1 when the line begins with an email/mail head field name
 * (a run of letters/digits/hyphens followed by a colon and a space or
 * end-of-line, e.g. "To: joe", "Subject:"), else 0.  In NetBSD this is
 * the wide-char sibling of usr.bin/mail/head.c's byte ishead().
 */
int
ishead(const wchar_t *s)
{
	const wchar_t *p = s;

	if (!iswalpha(*p))
		return 0;
	while (iswalnum(*p) || *p == L'-')
		p++;
	if (*p != L':')
		return 0;
	p++;
	if (*p == L'\0' || iswspace(*p))
		return 1;
	return *p == L'\t';
}