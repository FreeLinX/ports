/* FreeLinX/ports - base/compat : musl getbsize(3) prototype.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's <util.h> (and <libutil.h>) declares getbsize(3) (and many other
 * BSD-only helpers).  musl ships no <util.h> at all, so bin/ls cannot get
 * its getbsize() prototype from the system.  Rather than vendoring NetBSD's
 * whole <util.h> (which drags in <sys/ansi.h>, <sys/inttypes.h> and the
 * auth/pidfile/pty helpers, absent from musl), this is a FreeLinX stand-in
 * declaring only the interface ls actually calls.  The implementation is
 * NetBSD's own lib/libc/gen/getbsize.c, extracted from the src set: it is
 * built into the ls link set (ls uses getbsize() to honour $BLOCKSIZE).
 *
 * Do not add declarations here without a source that needs them.
 */
#ifndef _FREELINX_UTIL_H_
#define _FREELINX_UTIL_H_

#include <sys/cdefs.h>
#include <stdint.h>

__BEGIN_DECLS
char *getbsize(int *, long *);
void *emalloc(size_t);
void *ecalloc(size_t, size_t);
void *erealloc(void *, size_t);
char *estrdup(const char *);
char *estrndup(const char *, size_t);
char *strspct(char *, size_t, intmax_t, intmax_t, size_t);
char *strpct(char *, size_t, uintmax_t, uintmax_t, size_t);
/* BSD libutil easprintf(3) (asprintf + err(1) on failure), used by
 * NetBSD hexdump's odsyntax.c.  Implemented in compat/easprintf.c. */
int easprintf(char ** __restrict, const char * __restrict, ...);
__END_DECLS

#endif /* !_FREELINX_UTIL_H_ */