/*
 * FreeLinX/ports - base/compat : musl <disktab.h> stand-in.
 *
 * usr.bin/getent includes <disktab.h> for the disktab database.  The
 * actual data access goes through the cget(3)/getcap(3) API which
 * FreeLinX provides from libtinfo; this header only needs to exist.
 */

#ifndef _FREELINX_COMPAT_DISKTAB_H_
#define _FREELINX_COMPAT_DISKTAB_H_

#define	_PATH_DISKTAB	"/etc/disktab"

#endif /* !_FREELINX_COMPAT_DISKTAB_H_ */