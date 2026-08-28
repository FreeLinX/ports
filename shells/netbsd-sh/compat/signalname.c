/* FreeLinX/ports - shells/netbsd-sh : signalname(3)/signalnumber(3) for musl.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's libc provides signalname(3)/signalnumber(3)/signalnext(3) (used
 * by the sh's `trap` builtin and by `kill -l`).  musl's <signal.h> has none
 * of them.  A verbatim NetBSD table would be WRONG here, because the BSD and
 * Linux signal NUMBERING differ (e.g. SIGCONT is 19 on BSD but 18 on Linux,
 * SIGBUS is 10 on BSD but 7 on Linux).  This file therefore derives every
 * mapping from musl's own <signal.h> macros at compile time, so the shell
 * traps/names exactly the signals FreeLinX delivers.
 *
 * Compiled into the sh link set (see the netbsd-sh Makefile do-build recipe).
 */

#include <signal.h>
#include <stdio.h>
#include <strings.h>

static const char *
sigshort(int sig)
{
	switch (sig) {
	case SIGHUP:	return "HUP";
	case SIGINT:	return "INT";
	case SIGQUIT:	return "QUIT";
	case SIGILL:	return "ILL";
	case SIGTRAP:	return "TRAP";
	case SIGABRT:	return "ABRT";
	case SIGBUS:	return "BUS";
	case SIGFPE:	return "FPE";
	case SIGKILL:	return "KILL";
	case SIGUSR1:	return "USR1";
	case SIGSEGV:	return "SEGV";
	case SIGUSR2:	return "USR2";
	case SIGPIPE:	return "PIPE";
	case SIGALRM:	return "ALRM";
	case SIGTERM:	return "TERM";
	case SIGSTKFLT:	return "STKFLT";
	case SIGCHLD:	return "CHLD";
	case SIGCONT:	return "CONT";
	case SIGSTOP:	return "STOP";
	case SIGTSTP:	return "TSTP";
	case SIGTTIN:	return "TTIN";
	case SIGTTOU:	return "TTOU";
	case SIGURG:	return "URG";
	case SIGXCPU:	return "XCPU";
	case SIGXFSZ:	return "XFSZ";
	case SIGVTALRM:	return "VTALRM";
	case SIGPROF:	return "PROF";
	case SIGWINCH:	return "WINCH";
	case SIGIO:	return "IO";
	case SIGPWR:	return "PWR";
	case SIGSYS:	return "SYS";
	default:
		return NULL;
	}
}

const char *
signalname(int sig)
{
	static char rtb[16];
	const char *p;

	if (sig <= 0 || sig >= NSIG)
		return NULL;

	p = sigshort(sig);
	if (p != NULL)
		return p;

#ifdef SIGRTMIN
	if (sig >= SIGRTMIN && sig <= SIGRTMAX) {
		(void)snprintf(rtb, sizeof rtb, "RT%d", sig - SIGRTMIN);
		return rtb;
	}
#endif
	return NULL;
}

int
signalnumber(const char *name)
{
	int n;

	if (strncasecmp(name, "sig", 3) == 0)
		name += 3;

	for (n = 1; n < NSIG; ++n)
		/* NetBSD's SMALL signalnumber() also ignores real-time
		 * "RTn" names, so sigshort() suffices here. */
		if (sigshort(n) != NULL &&
		    strcasecmp(name, sigshort(n)) == 0)
			return n;

	return 0;
}

int
signalnext(int sig)
{
	int n;

	if (sig < 0 || sig >= NSIG)
		return -1;
	if (sig > 0 && signalname(sig) == NULL)
		return -1;

	for (n = sig + 1; n < NSIG; ++n)
		if (signalname(n) != NULL)
			return n;

	return 0;
}