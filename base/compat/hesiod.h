/*
 * FreeLinX/ports - base/compat/hesiod.h : hesiod(3) API stand-in.
 *
 * musl has no Hesiod (BIND-ish zone services) client.  hesinfo(1)
 * implements the NetBSD user interface exactly but cannot reach a real
 * Hesiod server, so the resolver calls report failure with a clear
 * message (hesinfo then prints "Hesiod name not found").  The structs
 * and prototypes follow the NetBSD <hesiod.h> layout so callers
 * continue to compile unmodified.
 */

#ifndef _FREELINX_COMPAT_HESIOD_H_
#define _FREELINX_COMPAT_HESIOD_H_

#define	MAXHOSTNAMELEN	256

/* Default domain to bound queries to (unused by the stub resolver). */
#define	HES_DOMAIN	""
#define	DEF_RHS			""

int	hesiod_init(void **);

char	*hesiod_to_bind(void *, const char *, const char *);
char	**hesiod_resolve(void *, const char *, const char *);
void	hesiod_free_list(void *, char **);
void	hesiod_end(void *);

#endif /* !_FREELINX_COMPAT_HESIOD_H_ */