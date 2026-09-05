/*
 * FreeLinX/ports - base/compat : musl <termcap.h> stand-in.
 *
 * usr.bin/qsubst is the only port still wanting the 4.4BSD termcap
 * interface (tgetent/tgetstr/tgetflag/tputs, with -ltermcap).  musl
 * ships no termcap; FreeLinX provides it from the ncurses libtinfo
 * static archive built as a build dependency.  This header declares
 * exactly the entry points qsubst uses and matches the BSD signatures.
 */

#ifndef _FREELINX_COMPAT_TERMCAP_H_
#define _FREELINX_COMPAT_TERMCAP_H_

#define	TCBUFSIZE	2048

char	*getenv(const char *);
char	*tgetstr(const char *, char **);
int	tgetent(char *, const char *);
int	tgetflag(const char *);
int	tgetnum(const char *);
int		tputs(const char *, int, int (*)(int));

/* cget(3)/getcap(3) family, provided by libtinfo (getent uses these). */
int	cgetent(char **, char **, const char *);
int	cgetfirst(char **, char **);
int	cgetnext(char **, char **);
int	cgetclose(void);
char	*cgetcap(char *, const char *, int);
int	cgetstr(char *, const char *, char **);
int	cgetnum(char *, const char *, long *);
int	cgetuname(int (*)(const char *));

#endif /* !_FREELINX_COMPAT_TERMCAP_H_ */