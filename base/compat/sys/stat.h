/* FreeLinX/ports - base/compat : musl <sys/stat.h> extension wrapper.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * This header is found first because the FreeLinX compat dir leads the
 * include path (-I base/compat).  It pulls in the real musl <sys/stat.h>
 * via #include_next and then adds what the NetBSD code expects and musl
 * does not provide.
 *
 * S_IFWHT (whiteout file type, NetBSD value 0x120000) is a BSD-only type
 * bit: Linux has no whiteout vnodes.  It is referenced by lib/libc/gen/
 * fts.c (FTS_WHITEOUT), bin/rm/rm.c (-W) and bin/ls/print.c (whiteout
 * indicator).  musl omits it.  Because the real type bits Linux can yield
 * in st_mode are confined to the 0170000 class mask and never take the
 * S_IFWHT value, defining the constant leaves all of that code compiled
 * verbatim yet unreachable at runtime -- the honest way to keep the BSD
 * sources unpatched here.
 *
 * No other musl/stat gap is handled in this file; add only what the base
 * sources actually require.
 */
#ifndef _FREELINX_COMPAT_SYS_STAT_H_
#define _FREELINX_COMPAT_SYS_STAT_H_

#include_next <sys/stat.h>

#ifndef S_IFWHT
#define	S_IFWHT	0x120000	/* whiteout (FreeLinX: never set by Linux) */
#endif
#ifndef S_ISWHT
#define	S_ISWHT(m)	(((m) & S_IFMT) == S_IFWHT)
#endif
/* NetBSD's <sys/stat.h> defines S_BLKSIZE (512) for the st_blocks unit;
 * musl's does not, and bin/ls uses it to sanity-check st_blocks. */
#ifndef S_BLKSIZE
#define	S_BLKSIZE	512
#endif

/*
 * NetBSD's usr.bin/touch calls utimens(2)/lutimens(2); musl only exposes
 * utimensat(2)/futimens(2).  compat/utimens.c provides the BSD-named pair on
 * top of utimensat.  The prototypes live here (beside <sys/stat.h>, where BSD
 * declares them) so touch.c needs no change beyond dropping the BSD-only
 * <tzfile.h>/<util.h>/parsedate block.
 */
int	utimens(const char *, const struct timespec [2]);
int	lutimens(const char *, const struct timespec [2]);

#endif /* !_FREELINX_COMPAT_SYS_STAT_H_ */