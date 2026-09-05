/* FreeLinX/ports - base/compat/termcap_stub.c : no-op termcap entries.
 *
 * usr.bin/msgs sizes its question/and-prompt display through tgetent(3)/
 * tgetnum(3) (the output-field count), falling back to a default when the
 * termcap lookup fails.  FreeLinX has no terminfo/termcap database, and
 * msgs is not worth linking the ncurses libtinfo archive for.  These stubs
 * make the lookup "not found" and msgs uses its defaults.  Powers-ports that
 * want REAL termcap link libtinfo and must NOT link this file.
 */
#include <termcap.h>

int
tgetent(char *buf, const char *name)
{
	(void)buf;
	(void)name;
	return 0;
}

int
tgetnum(const char *id)
{
	(void)id;
	return 0;
}

int
tgetflag(const char *id)
{
	(void)id;
	return 0;
}

char *
tgetstr(const char *id, char **area)
{
	(void)id;
	(void)area;
	return NULL;
}

int
tputs(const char *str, int affcnt, int (*outc)(int))
{
	(void)str;
	(void)affcnt;
	(void)outc;
	return 1;
}