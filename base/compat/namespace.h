/* FreeLinX/ports - base/compat : NetBSD libc namespace.h stand-in.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * NetBSD compiles libc itself with a per-source "namespace.h" that remaps
 * internal symbols (stat -> __stat50 etc.) for ABI discipline.  When libc
 * sources (fts.c, vis.c, getbsize.c) are rebuilt into a static app, that
 * remapping is meaningless, so the NetBSD tree's own namespace.h would
 * declare nothing useful here -- musl has no symbol-versioning contract.
 *
 * A source in lib/libc/gen that includes "namespace.h" finds this file via
 * the -I compat include path (quoted includes fall back to -I dirs when the
 * file is not beside the .c).  It intentionally defines nothing.
 */
#ifndef _FREELINX_NAMESPACE_H_
#define _FREELINX_NAMESPACE_H_

#endif /* !_FREELINX_NAMESPACE_H_ */