/*
 * FreeLinX/ports - base/compat/login_cap.h : minimal <login_cap.h>.
 *
 * pwhash(1) pulls login_cap in only for pw_getconf(3), the libutil
 * helper that reads the style/round setting from /etc/passwd.conf.
 */

#ifndef _FREELINX_COMPAT_LOGIN_CAP_H_
#define _FREELINX_COMPAT_LOGIN_CAP_H_

#include <sys/types.h>
#include <sys/param.h>

int	pw_getconf(char *, size_t, const char *, const char *);

#endif /* !_FREELINX_COMPAT_LOGIN_CAP_H_ */