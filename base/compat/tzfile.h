/*
 * FreeLinX/ports - base/compat : musl <tzfile.h> stand-in.
 *
 * musl has no tzfile.h.  It is a NetBSD header describing the on-disk
 * zoneinfo format -- struct tzhead, the TZ_MAX_* limits, ttinfo/t leaps -- and
 * the only reason a program includes it is to read or write /usr/share/zoneinfo
 * by hand.  musl's tzset() reads TZ and calls the kernel's VDSO, or reads the
 * TZ environment variable directly when it is not a zoneinfo name; there is no
 * in-library zoneinfo parser for such a program to talk to, which is why the
 * header does not exist rather than existing and being incomplete.
 *
 * usr.bin/su includes it and uses nothing from it:
 *
 *   usr/bin/su/su.c:62:#include <tzfile.h>
 *
 * and no other identifier from the header appears in su.c, so nothing is
 * dropped by making this empty.  The one line is left in place rather than
 * patched out, so su.c still reads as the NetBSD source it is.
 *
 * A program that really does parse zoneinfo wants the format spelled out, and
 * then this header is the wrong answer: it wants NetBSD's, in include/.
 */

#ifndef _FREELINX_COMPAT_TZFILE_H_
#define _FREELINX_COMPAT_TZFILE_H_

#endif /* !_FREELINX_COMPAT_TZFILE_H_ */
