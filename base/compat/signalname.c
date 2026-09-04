/* FreeLinX/ports - base/compat : signalname(3)/signalnumber(3)/signalnext(3)
 * for musl.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's libc provides signalname(3)/signalnumber(3)/signalnext(3) (used
 * by the sh's `trap` builtin, by `kill -l` and by `kill -s`, and by the
 * printsignals() helper in bin/kill/kill.c).  musl's <signal.h> has none of
 * them.  A verbatim NetBSD table would be WRONG here, because the BSD and
 * Linux signal NUMBERING differ (e.g. SIGCONT is 19 on BSD but 18 on Linux,
 * SIGBUS is 10 on BSD but 7 on Linux).  This file therefore derives every
 * mapping from musl's own <signal.h> macros at compile time, so the shell
 * traps/names exactly the signals FreeLinX delivers.
 *
 * Shared by the netbsd-sh and base/kill link sets (see their Makefiles).
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
/* newsyslog's parse_sig() loops `for (n = 1; n < NSIG; n++)` and
 * strcasecmp()s sys_signame[n]; every slot up to NSIG must be non-NULL.
 * Linux realtime slots have no BSD short name, so paint 'RTnn'.
 * (musl's <signal.h> defines NSIG 64.)
 */
const char * const sys_signame[NSIG + 1] = {
	[0] = "Name",
	[1] = "Hup",
	[2] = "Int",
	[3] = "Quit",
	[4] = "Ill",
	[5] = "Trap",
	[6] = "Abrt",
	[7] = "Bus",
	[8] = "Fpe",
	[9] = "Kill",
	[10] = "Usr1",
	[11] = "Segv",
	[12] = "Usr2",
	[13] = "Pipe",
	[14] = "Alrm",
	[15] = "Term",
	[16] = "Stkflt",
	[17] = "Chld",
	[18] = "Cont",
	[19] = "Stop",
	[20] = "Tstp",
	[21] = "Ttin",
	[22] = "Ttou",
	[23] = "Urg",
	[24] = "Xcpu",
	[25] = "Xfsz",
	[26] = "Vtalrm",
	[27] = "Prof",
	[28] = "Winch",
	[29] = "Io",
	[30] = "Pwr",
	[31] = "Sys",
	[32] = "RT0",
	[33] = "RT1",
	[34] = "RT2",
	[35] = "RT3",
	[36] = "RT4",
	[37] = "RT5",
	[38] = "RT6",
	[39] = "RT7",
	[40] = "RT8",
	[41] = "RT9",
	[42] = "RT10",
	[43] = "RT11",
	[44] = "RT12",
	[45] = "RT13",
	[46] = "RT14",
	[47] = "RT15",
	[48] = "RT16",
	[49] = "RT17",
	[50] = "RT18",
	[51] = "RT19",
	[52] = "RT20",
	[53] = "RT21",
	[54] = "RT22",
	[55] = "RT23",
	[56] = "RT24",
	[57] = "RT25",
	[58] = "RT26",
	[59] = "RT27",
	[60] = "RT28",
	[61] = "RT29",
	[62] = "RT30",
	[63] = "RT31",
	[64] = "RT32",
};
