/* FreeLinX/ports - base/compat : empty nbtool_config.h.
 *
 * xinstall.c does `#if HAVE_NBTOOL_CONFIG_H : #include "nbtool_config.h"`.
 * FreeLinX builds xinstall with -DHAVE_NBTOOL_CONFIG_H=1 so that all of the
 * BSD chflags(2)/st_flags handling (which has no Linux equivalent) is
 * compiled out, exactly as NetBSD does for its host build tools.  No config
 * definitions are needed. */
