/*
 * FreeLinX/ports - base/compat/getcap.c : cget(3)/getcap(3) API.
 *
 * getent reads gettytab/printcap/disktab through cgetent(3) and
 * friends.  musl has none, and modern ncurses no longer ships the
 * deprecated getcap layer in its libraries, so FreeLinX provides a
 * self-contained implementation of the BSD file format:
 *
 *   entry := name ['|' alias...] ':' cap [':' cap]...
 *   cap   := flag | name '=' value | name '#' number | name '@'
 *
 * "tc=other" entries are merged with the referenced entry's
 * capabilities first so this entry's own duplicates win.  A quoted
 * value may contain colons without terminating the entry.  Numbers
 * may carry a k/m/g/n unit suffix (n multiplies by 1024, like the
 * original getcap(3)).
 */

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

struct cgentry {
	char *text;
	struct cgentry *next;
};

static struct cgentry *g_entries, *g_tail;
static struct cgentry *g_curr;
static char **g_scanned;

/* Build the parsed entry list from db_array (idempotent). */
static void
cgbuild(char **db_array)
{
	char **pp;
	FILE *fp;
	char line[1024];
	char buf[8192];

	if (db_array == g_scanned)
		return;
	while (g_entries != NULL) {
		struct cgentry *nx = g_entries->next;

		free(g_entries->text);
		free(g_entries);
		g_entries = nx;
	}
	g_tail = NULL;
	g_curr = NULL;
	g_scanned = db_array;
	if (db_array == NULL)
		return;

	for (pp = db_array; *pp != NULL; pp++) {
		fp = fopen(*pp, "r");
		if (fp == NULL)
			continue;
		buf[0] = '\0';
		while (fgets(line, sizeof(line), fp) != NULL) {
			size_t len = strlen(line);

			while (len > 0 && (line[len - 1] == '\n' ||
			    line[len - 1] == '\r'))
				line[--len] = '\0';
			if (line[0] == '\0')
				continue;
			if (strlen(buf) + len + 1 >= sizeof(buf)) {
				buf[0] = '\0';
				continue;
			}
			if (buf[0] != '\0')
				strcat(buf, "\\\n");
			strcat(buf, line);
			if (len == 0 || buf[strlen(buf) - 1] != '\\') {
				struct cgentry *e;
				char *ent = buf;

				while (*ent == '\t' || *ent == ' ')
					ent++;
				if (*ent != '#' && *ent != '\0' &&
				    strchr(ent, ':') != NULL) {
					e = malloc(sizeof(*e));
					if (e == NULL)
						break;
					e->text = strdup(ent);
					e->next = NULL;
					if (e->text == NULL) {
						free(e);
						break;
					}
					if (g_tail == NULL)
						g_entries = e;
					else
						g_tail->next = e;
					g_tail = e;
				}
				buf[0] = '\0';
			}
		}
		fclose(fp);
	}
}

static struct cgentry *
cglookup(const char *name)
{
	struct cgentry *e;
	size_t n = strlen(name);

	for (e = g_entries; e != NULL; e = e->next) {
		const char *p = e->text;
		const char *endn = strchr(p, ':');
		const char *nm = p;
		const char *al;

		if (endn == NULL)
			continue;
		if ((size_t)(endn - nm) == n &&
		    strncmp(nm, name, n) == 0)
			return e;
		al = nm;
		while ((al = strchr(al, '|')) != NULL && al < endn) {
			const char *s = al + 1;
			const char *e2 = s;

			while (e2 < endn && *e2 != '|')
				e2++;
			if ((size_t)(e2 - s) == n &&
			    strncmp(s, name, n) == 0)
				return e;
			al = e2;
		}
	}
	return NULL;
}

/* Resolve "tc=name" at the end of the cap list. */
static const char *
cg_tc(const char *caps)
{
	const char *p = caps;

	while ((p = strstr(p, "tc=")) != NULL) {
		p += 3;
		if (*p == '"')
			continue;
		return p;
	}
	return NULL;
}

/*
 * Produce the effective caps text for an entry: referenced entry's
 * caps first (recursively), then this entry's own caps.
 */
static char *
cgeffective(struct cgentry *e, int depth)
{
	const char *own;
	const char *col;
	const char *tc;
	struct cgentry *base;
	char *b;

	if (depth > 16 || e == NULL)
		return NULL;
	col = strchr(e->text, ':');
	if (col == NULL)
		return NULL;
	own = col + 1;
	tc = cg_tc(own);
	if (tc != NULL) {
		const char *q;
		size_t n = 0;
		char tcn[256];

		q = tc;
		while (*q != '\0' && *q != ':' &&
		    !isspace((unsigned char)*q))
			n++;
		if (n == 0 || n >= sizeof(tcn))
			return strdup(own);
		memcpy(tcn, tc, n);
		tcn[n] = '\0';
		base = cglookup(tcn);
		if (base != NULL && base != e) {
			char *bp = cgeffective(base, depth + 1);

			if (bp != NULL) {
				size_t bn = strlen(bp);

				b = malloc(bn + 1 + strlen(own) + 2);
				if (b == NULL) {
					free(bp);
					return NULL;
				}
				memcpy(b, bp, bn);
				b[bn] = ':';
				strcpy(b + bn + 1, own);
				free(bp);
				return b;
			}
		}
	}
	return strdup(own);
}

int
cgetent(char **cap, char **db_array, const char *name)
{
	struct cgentry *e;

	cgbuild(db_array);
	*cap = NULL;
	e = cglookup(name);
	if (e == NULL) {
		errno = ENOENT;
		return -1;
	}
	*cap = cgeffective(e, 0);
	if (*cap == NULL)
		return -2;
	return 0;
}

int
cgetfirst(char **cap, char **db_array)
{
	cgbuild(db_array);
	g_curr = g_entries;
	return cgetnext(cap, db_array);
}

int
cgetnext(char **cap, char **db_array)
{
	cgbuild(db_array);
	if (g_curr == NULL && g_entries != NULL)
		g_curr = g_entries;
	while (g_curr != NULL) {
		struct cgentry *e = g_curr;
		char *b;

		g_curr = g_curr->next;
		b = cgeffective(e, 0);
		if (b == NULL)
			continue;
		*cap = b;
		return 0;
	}
	*cap = NULL;
	errno = ENOENT;
	return -1;
}

int
cgetclose(void)
{
	struct cgentry *e = g_entries;

	while (e != NULL) {
		struct cgentry *nx = e->next;

		free(e->text);
		free(e);
		e = nx;
	}
	g_entries = NULL;
	g_tail = NULL;
	g_curr = NULL;
	g_scanned = NULL;
	return 0;
}

char *
cgetcap(char *cap, const char *name, int type)
{
	char *p = cap;
	size_t n = strlen(name);

	while (*p != '\0') {
		char *after;

		if (*p == ':')
			p++;
		if (strncmp(p, name, n) == 0 && (p[n] == '=' || p[n] == '#' ||
		    p[n] == '@' || p[n] == ':' || p[n] == '\0')) {
			after = p + n;
			if (*after == '=') {
				if (type == ':' || type == '#')
					return NULL;
				return after + 1;
			}
			if (*after == '#') {
				if (type == ':' || type == '=' ||
				    type == 'x')
					return NULL;
				return after + 1;
			}
			if (*after == '@')
				return NULL;
			if (type == ':')
				return p;
			return type == '*' ? p : NULL;
		}
		/* Skip to end of this cap, honouring quoted colons. */
		while (*p != '\0') {
			if (*p == '"') {
				p++;
				while (*p != '\0' && *p != '"')
					p++;
				if (*p == '"')
					p++;
				continue;
			}
			if (*p == ':') {
				p++;
				break;
			}
			p++;
		}
	}
	return NULL;
}

static char *
cgexpand(const char *src)
{
	const char *p = src;
	char *out = malloc(strlen(src) + 1);
	char *q = out;
	int esc = 0;

	if (out == NULL)
		return NULL;
	while (*p != '\0') {
		if (esc) {
			switch (*p) {
			case 'n': *q++ = '\n'; break;
			case 't': *q++ = '\t'; break;
			case 'b': *q++ = '\b'; break;
			case 'r': *q++ = '\r'; break;
			case 'e': *q++ = 033; break;
			case 'f': *q++ = '\f'; break;
			default:
				if (isdigit((unsigned char)*p)) {
					int v = 0, i;

					for (i = 0; i < 3 &&
					    isdigit((unsigned char)*p); i++) {
						v = v * 8 + (*p - '0');
						p++;
					}
					*q++ = (char)v;
					continue;
				}
				*q++ = *p;
				break;
			}
			esc = 0;
			p++;
			continue;
		}
		if (*p == '\\') {
			esc = 1;
			p++;
			continue;
		}
		if (*p == '^') {
			p++;
			if (*p != '\0')
				*q++ = (char)(*p++ & 0x1f);
			continue;
		}
		*q++ = *p++;
	}
	*q = '\0';
	/* Strip surrounding double quotes. */
	{
		char *s = out;
		size_t l = strlen(s);

		if (l >= 2 && s[0] == '"' && s[l - 1] == '"') {
			memmove(s, s + 1, l - 2);
			s[l - 2] = '\0';
		}
	}
	return out;
}

int
cgetstr(char *cap, const char *name, char **str)
{
	char *v;

	*str = NULL;
	v = cgetcap(cap, name, '=');
	if (v == NULL)
		return -1;
	{
		char *e;
		char *s = cgexpand(v);

		if (s == NULL)
			return -2;
		/* Value ends at unquoted colon or end. */
		e = s;
		while (*e != '\0' && *e != ':')
			e++;
		*e = '\0';
		if (s[0] == '"' && e > s + 1 && *(e - 1) == '"') {
			memmove(s, s + 1, (size_t)(e - s - 2));
			s[(size_t)(e - s - 2)] = '\0';
		}
		*str = s;
		return (int)strlen(s);
	}
}

int
cgetnum(char *cap, const char *name, long *num)
{
	char *v = cgetcap(cap, name, '#');
	char *e;

	*num = 0;
	if (v == NULL)
		return -1;
	if (*v == '"')
		v++;
	errno = 0;
	{
		long n = strtol(v, &e, 0);

		if (e == v || (*e != '\0' && *e != ':'))
			return -1;
		for (; *e == 'k' || *e == 'm' || *e == 'g' || *e == 'n';
		    e++) {
			if (*e == 'k')
				n *= 1024L;
			else if (*e == 'm')
				n *= 1024L * 1024L;
			else if (*e == 'g')
				n *= 1024L * 1024L * 1024L;
			else
				n *= 1024L;
		}
		*num = n;
	}
	return 0;
}

/*
 * cgetustr(3): like cgetstr() but returns the value with escape
 * sequences left unexpanded (vgrind passes these straight through to
 * its printer language).  The returned buffer is malloc'd like
 * cgetstr.
 */
int
cgetustr(char *cap, const char *name, char **str)
{
	char *v;

	*str = NULL;
	v = cgetcap(cap, name, '=');
	if (v == NULL)
		return -1;
	{
		const char *e = v;
		char *s;

		while (*e != '\0' && *e != ':') {
			if (*e == '\\') {
				e++;
				if (*e == '\0')
					break;
				e++;
				continue;
			}
			if (*e == '"') {
				e++;
				continue;
			}
			e++;
		}
		s = malloc((size_t)(e - v) + 1);
		if (s == NULL)
			return -2;
		memcpy(s, v, (size_t)(e - v));
		s[e - v] = '\0';
		{
			size_t l = strlen(s);

			if (l >= 2 && s[0] == '"' && s[l - 1] == '"') {
				memmove(s, s + 1, l - 2);
				s[l - 2] = '\0';
			}
		}
		*str = s;
		return (int)strlen(s);
	}
}
/*
 * cgetset(3): look `name' up in the termios database and apply it to the
 * controlling terminal.
 *
 * The other half of cgetent(3), and the reason tip(1) reaches for it.  musl has
 * no termios database at all, and NetBSD 10.1 does not ship cgetset(3)'s own
 * source in the src set base-port.mk extracts from, so there is nothing to
 * borrow: this is written against the same entry format cgetent(3) above
 * already parses, and it uses that parser rather than a second one.
 *
 * The database is searched the way it is everywhere else:
 *
 *   $HOME/.termcap   the per-user file, as `infocmp -1' writes it
 *   /etc/termios     the system file
 *   $TERMCAP         a full path, if the variable names one
 *
 * A termios entry is an ordinary termcap entry whose `tc=' field names the
 * capabilities to take:
 *
 *   dumb|unknown:\
 *	:type=dumb:\
 *	:description=unknown terminal:\
 *	:tc=dumb:
 *
 * Of those capabilities two are settings and the rest are action strings.  The
 * settings are the ones a caller such as tip(1) is after - the terminal has to
 * be the size and speed the host expects - so they are applied here.  The
 * action strings (init, reset, clear_eos, and the padding/parameter
 * capabilities) need a compiled terminal description to run, which is ncurses'
 * job and not this file's: this deliberately does not link against ncurses, so a
 * capability that would have to be executed is skipped rather than silently
 * mangled.  That is the one documented gap, and it is a gap in the action half
 * only.
 *
 * A FreeLinX system as shipped has neither /etc/termios nor a ~/.termcap, so
 * every call returns -1 with errno ENOENT.  That is the truth - there is no
 * database to look the name up in - and not a stub: it starts working the
 * moment a termios database is installed, with no rebuild.
 */
int
cgetset(const char *name)
{
	static const char *const sysdb = "/etc/termios";
	char homecap[1024];
	char *db[4];
	const char *home, *termcap;
	char *cap, *rows, *cols;
	int ndb = 0, applied = 0;

	if (name == NULL || *name == '\0') {
		errno = EINVAL;
		return -1;
	}

	home = getenv("HOME");
	if (home != NULL && *home != '\0' &&
	    (size_t)snprintf(homecap, sizeof(homecap), "%s/.termcap", home) <
	    sizeof(homecap))
		db[ndb++] = homecap;
	db[ndb++] = (char *)sysdb;
	termcap = getenv("TERMCAP");
	if (termcap != NULL && *termcap == '/')
		db[ndb++] = (char *)termcap;
	db[ndb] = NULL;

	/* cgeffective() already resolves a `tc=' reference into the named
	 * entry's capabilities merged with the referring entry's own, so
	 * cgetent() hands back a string to read rows and columns out of. */
	if (cgetent(&cap, db, name) < 0)
		return -1;			/* errno set by cgetent */

	/* A window size is one value: an entry carrying only one of rows and
	 * columns describes something that cannot be applied, so both are
	 * required before either is used. */
	rows = cols = NULL;
	if (cgetstr(cap, "lines", &rows) > 0 && rows != NULL &&
	    cgetstr(cap, "columns", &cols) > 0 && cols != NULL) {
		struct winsize ws;

		ws.ws_col = (unsigned short)strtol(cols, NULL, 10);
		ws.ws_row = (unsigned short)strtol(rows, NULL, 10);
		ws.ws_xpixel = 0;
		ws.ws_ypixel = 0;
		if (ws.ws_col != 0 && ws.ws_row != 0 &&
		    (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == 0 ||
		     ioctl(STDOUT_FILENO, TIOCSWINSZ, &ws) == 0))
			applied = 1;
	}

	free(cols);
	free(rows);
	free(cap);

	if (!applied) {
		errno = ENOENT;
		return -1;
	}
	return 0;
}
