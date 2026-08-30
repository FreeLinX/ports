/* FreeLinX/ports - base/timeout : sys_nsig/sys_signame shim for musl.
 *
 * WHY THIS FILE EXISTS (see README / OS porting notes):
 *
 * NetBSD <signal.h> exposes the globals `sys_signame[]` (short signal names
 * indexed by signal number) and `sys_nsig` (the number of signals), which
 * usr.bin/timeout/timeout.c reads in parse_signal().  musl has neither.
 * This file derives both from musl's own <signal.h> macros at compile time,
 * so timeout names exactly the signals FreeLinX delivers (Linux numbering,
 * not BSD numbering).
 */

#include <signal.h>
#include <stddef.h>

static const char *const sigabbrev[NSIG] = {
	[SIGHUP]	= "HUP",
	[SIGINT]	= "INT",
	[SIGQUIT]	= "QUIT",
	[SIGILL]	= "ILL",
	[SIGTRAP]	= "TRAP",
	[SIGABRT]	= "ABRT",
	[SIGBUS]	= "BUS",
	[SIGFPE]	= "FPE",
	[SIGKILL]	= "KILL",
	[SIGUSR1]	= "USR1",
	[SIGSEGV]	= "SEGV",
	[SIGUSR2]	= "USR2",
	[SIGPIPE]	= "PIPE",
	[SIGALRM]	= "ALRM",
	[SIGTERM]	= "TERM",
#ifdef SIGSTKFLT
	[SIGSTKFLT]	= "STKFLT",
#endif
	[SIGCHLD]	= "CHLD",
	[SIGCONT]	= "CONT",
	[SIGSTOP]	= "STOP",
	[SIGTSTP]	= "TSTP",
	[SIGTTIN]	= "TTIN",
	[SIGTTOU]	= "TTOU",
	[SIGURG]	= "URG",
	[SIGXCPU]	= "XCPU",
	[SIGXFSZ]	= "XFSZ",
	[SIGVTALRM]	= "VTALRM",
	[SIGPROF]	= "PROF",
	[SIGWINCH]	= "WINCH",
	[SIGIO]		= "IO",
#ifdef SIGPWR
	[SIGPWR]	= "PWR",
#endif
	[SIGSYS]	= "SYS",
};

const char *sys_signame[NSIG];

int sys_nsig = NSIG;

__attribute__((constructor))
static void
flx_timeout_init_sigtab(void)
{
	int i;

	for (i = 0; i < NSIG; i++)
		sys_signame[i] = sigabbrev[i];
}
