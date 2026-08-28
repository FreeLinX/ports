/* FreeLinX/ports - shells/netbsd-sh : musl getprogname(3)/setprogname(3).
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's libc provides getprogname(3)/setprogname(3).  musl exposes the
 * equivalent data through the GNU-ism program_invocation_short_name (declared
 * in <errno.h>).  The sh's error.c calls getprogname() to prefix error
 * messages with the shell's own name, so we provide it here; the bltin
 * sources get the same symbol via the macros in bltin.h.
 *
 * Compiled into the sh link set (see the netbsd-sh Makefile do-build recipe).
 */

#include <errno.h>
#include <string.h>

/*
 * musl declares these GNU-isms only under _GNU_SOURCE; the sh uses neither
 * flag.  The symbols themselves are provided by musl's crt/crt1.o, so a
 * plain extern here is enough to use them.
 */
extern char *program_invocation_short_name;
extern char *program_invocation_name;

const char *
getprogname(void)
{
	if (program_invocation_short_name != NULL
	    && *program_invocation_short_name != '\0')
		return program_invocation_short_name;
	return "sh";
}

void
setprogname(const char *progname)
{
	/* musl tracks this itself (from argv[0]); keep the interface. */
	(void)progname;
}

/*
 * NetBSD's setproctitle(3) lets jobs' `kill %Z`/shell `setproctitle` update
 * the process title.  musl has no equivalent; a no-op is safe (the shell's
 * observable behaviour -- outfmt, exit status -- is unchanged).
 */
void
setproctitle(const char *fmt, ...)
{
	(void)fmt;
}