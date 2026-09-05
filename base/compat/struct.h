/*
 * FreeLinX/ports - base/compat/struct.h : stand-in for the NetBSD
 * <struct.h> that usr.bin/lastcomm includes.  lastcomm only needs the
 * NODEV sentinel and the libutil devname(3) prototype from it.
 */

#ifndef _FREELINX_COMPAT_STRUCT_H_
#define _FREELINX_COMPAT_STRUCT_H_

#include <sys/types.h>
#include <sys/stat.h>

#ifndef NODEV
#define	NODEV		((dev_t)-1)
#endif

char	*devname(dev_t, mode_t);

#endif /* !_FREELINX_COMPAT_STRUCT_H_ */