/*-
 * Copyright (c) 2026 The FreeLinX Project
 *
 * FreeLinX musl-compat stand-in for libterminfo pieces that NetBSD's
 * telnet(1) links from the base terminfo library:
 *
 *	- setupterm(3): FreeLinX ships no terminfo database, and telnet only
 *	  uses it to resolve the terminal-type NAME list for TELOPT_TTYPE.
 *	  Returning failure makes gettermname() take its documented fallback:
 *	  send the plain, upper-cased $TERM value.
 *	- ttytype[]: referenced from telnet.c's `#else` branch of the
 *	  TERMCAP conditional (termbuf==ttytype).  Only accessed when
 *	  setupterm() succeeds, which never happens here, but the symbol is
 *	  needed at link time.
 */

#include <stddef.h>

int
setupterm(const char *term, int fd, int *errret)
{
	/* No terminal information database: report failure so the caller
	 * falls back to its default terminal-type handling. */
	if (errret != NULL)
		*errret = 0;
	return -1;
}

char ttytype[32] = "vt220";	/* default terminal type (unused) */