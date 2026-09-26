/*	$NetBSD: shadow_auth.h,v 1.1 2026 FreeLinX OS Project	*/

/*
 * FreeLinX/ports - base/login : the password-database interface musl lacks.
 *
 * NetBSD's login.c authenticates and enforces the account/password expiry
 * policy through struct passwd - pw_passwd, pw_expire, pw_change.  musl's
 * getpwnam(3) reads /etc/passwd, whose hash field is "x" on a FreeLinX system,
 * and the struct cannot be extended to carry the rest.  So login.c asks for
 * those four values here instead, and this port reads them out of /etc/shadow.
 *
 * See shadow_auth.c for the field mapping and for why a login built on musl's
 * struct passwd alone would reject every password.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 */

#ifndef _FLX_SHADOW_AUTH_H_
#define _FLX_SHADOW_AUTH_H_

#include <sys/types.h>
#include <time.h>

/*
 * NetBSD's <pwd.h> values for the two policy states, and <sys/param.h>'s
 * SECSPERDAY.  login.c compares against all three directly, so they have to be
 * the same numbers; the definitions come from here because musl has no
 * headers that provide them.
 */
#define FLX_PASSWORD_CHGNOW	((time_t)-1)
#define FLX_PASSWORD_WARNDAYS	14

#ifndef SECSPERDAY
#define SECSPERDAY	86400
#endif

/*
 * The crypt(3) hash to authenticate against, or NULL when the account has no
 * usable password.  NULL covers "no such account", "empty hash", and the
 * "!"/"*" no-login markers alike: in each case there is nothing to match, and
 * login(1) must fail rather than skip the check.
 */
const char *flx_shadow_hash(const char *);

/*
 * 1 when the account exists but must not be logged in to, because the hash is
 * a no-login marker or because the password is past max and the inact window
 * has run out.
 */
int flx_shadow_locked(const char *);

/* Absolute time the account expires; 0 = never. */
time_t flx_shadow_account_expire(const char *);

/*
 * Absolute time the password expires, FLX_PASSWORD_CHGNOW when it must be
 * changed now, 0 when it never expires on age or when the account is locked.
 */
time_t flx_shadow_password_expire(const char *);

/*
 * int setlogin(const char *);
 *   0 on success, -1 with errno set otherwise.  Does not write utmp: this
 *   same file writes the USER_PROCESS record itself, in update_db().
 */
int setlogin(const char *);

/*
 * char *getlogin(void);
 *   The name passed to setlogin(), or NULL.  musl does not declare this.
 */
char *getlogin(void);

/*
 * int ttyaction(const char *tty, const char *action, const char *user);
 *   0 on success.  FreeLinX has no /etc/ttys action database, so there is never
 *   an action to run and this always succeeds; see login_ttyaction.c for why
 *   failing here would be wrong.
 */
int ttyaction(const char *, const char *, const char *);

#endif /* !_FLX_SHADOW_AUTH_H_ */
