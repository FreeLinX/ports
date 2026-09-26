/*
 * FreeLinX/ports - base/login : the notice login(1) prints above the prompt.
 *
 * NetBSD's usr.bin/login/Makefile builds this file rather than keeping it:
 *
 *	copyrightstr.c: ${NETBSDSRCDIR}/sys/conf/copyright
 *		${_MKTARGET_CREATE}
 *		rm -f ${.TARGET}
 *		${TOOL_AWK} 'BEGIN { print "const char copyrightstr[] =" }\
 *			{ print "\""$$0"\\n\""}\
 *			END { print "\"\\n;" }' ${.ALLSRC} > ${.TARGET}
 *
 * and login.c prints the array when the session is a console login.  The
 * source is NetBSD's own sys/conf/copyright, which is not extracted here (it is
 * not a member of this port), and the awk dance would be a second way of
 * producing one file that this repository can just as well carry directly.
 *
 * The text is the FreeLinX notice, not NetBSD's: the binary is FreeLinX's, and
 * printing a copyright that does not cover it would be worse than printing
 * none.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 */

const char copyrightstr[] =
"FreeLinX OS\n"
"Copyright (c) 2026 FreeLinX OS Project\n"
"\n"
"This operating system and its userland are free software: you may use,\n"
"redistribute and modify it under the terms of the BSD 2-Clause License,\n"
"or, at your option, any later version published by the Project.\n"
"\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n";
