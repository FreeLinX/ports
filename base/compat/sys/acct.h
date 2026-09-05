/*
 * FreeLinX/ports - base/compat/sys/acct.h : NetBSD <sys/acct.h>.
 *
 * musl has no accounting headers.  lastcomm(1) reads files written by
 * the BSD kernel accounting subsystem; FreeLinX has none, so the struct
 * is present for the decoder to compile against (a read of a real BSD
 * acct file will still decode correctly since the layout matches).
 */

#ifndef _FREELINX_COMPAT_SYS_ACCT_H_
#define _FREELINX_COMPAT_SYS_ACCT_H_

#include <sys/types.h>

typedef u_int32_t	comp_t;

struct acct {
	char		ac_comm[16];	/* command name */
	comp_t		ac_utime;	/* user time */
	comp_t		ac_stime;	/* system time */
	comp_t		ac_etime;	/* elapsed time */
	time_t		ac_btime;	/* starting time */
	uid_t		ac_uid;		/* user id */
	gid_t		ac_gid;		/* group id */
	short		ac_mem;		/* memory usage */
	comp_t		ac_io;		/* chars written */
	dev_t		ac_tty;		/* controlling tty */
	u_char		ac_flag;	/* accounting flags */
};

#define	AFORK		'\001'	/* forked but without exec */
#define	ASU		'\002'	/* used super-user privileges */
#define	ACOMPAT		'\004'	/* used compatibility mode */
#define	ACORE		'\010'	/* dumped core */
#define	AXSIG		'\020'	/* killed by a signal */

#define	AHZ		100		/* pseudo clocks/second used by acct */

#define	fldsiz(a, b)	(sizeof(((struct a *)0)->b))

#endif /* !_FREELINX_COMPAT_SYS_ACCT_H_ */