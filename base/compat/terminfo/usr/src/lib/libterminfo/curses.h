/* FreeLinX shim: libterminfo/setupterm.c pulls <curses.h> solely for the
 * use_env() prototype required by POSIX; these termtools link no curses, so
 * provide the declaration here. */
#ifndef _FLX_TERMINFO_CURSES_H_
#define _FLX_TERMINFO_CURSES_H_
#include <stdbool.h>
void use_env(bool value);
#endif
