/* FreeLinX/ports - base/compat : nbtool_config.h stand-in.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD's usr.bin/nbtool_config machinery compiles libc sources into the
 * install-time tools by defining HAVE_NBTOOL_CONFIG_H on the command line
 * (see usr.bin/Makefile.inc, the "nbtool" ports).  That makes <nbtool_config.h>
 * name the autoconf-style feature set for the host build.  bin/ls, bin/cp,
 * bin/rm pull lib/libc/gen/fts.c the same way (usr.bin/xinstall and bin/rm
 * both build fts.c into their tool).  The FreeLinX ports follow the same
 * convention: `-DHAVE_NBTOOL_CONFIG_H=1` on the compile line includes this
 * file.
 *
 * For FreeLinX, being on the command line is enough: defining
 * HAVE_NBTOOL_CONFIG_H prevents fts.c's `#if ! HAVE_NBTOOL_CONFIG_H` fallback
 * (which would wrongly advertise HAVE_STRUCT_DIRENT_D_NAMLEN for musl, whose
 * struct dirent has no d_namlen), while leaving every actual HAVE_* feature
 * macro undefined, so fts.c takes its portable paths.  This file therefore
 * deliberately defines nothing further.
 */
#ifndef _FREELINX_NBTOOL_CONFIG_H_
#define _FREELINX_NBTOOL_CONFIG_H_

#endif /* !_FREELINX_NBTOOL_CONFIG_H_ */