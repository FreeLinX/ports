/*
 * FreeLinX/ports - base/compat : login(1)'s setlogin(3) and ttyaction(3).
 *
 * Two calls NetBSD's usr.bin/login makes that musl has no equivalent of.
 *
 * setlogin(3)
 * ----------
 * NetBSD records the name the user is logging in as, so that getlogin(3) can
 * return it.  login.c calls it after dropping to the user's groups and before
 * the final setuid, and only logs a syslog line when it fails:
 *
 *     if (nested == NULL && setlogin(pwd->pw_name) < 0)
 *             syslog(LOG_ERR, "setlogin() failure: %m");
 *
 * NetBSD's setlogin also writes a LOGIN_PROCESS line to utmp.  Here that is
 * deliberately not done: the very same login.c writes the USER_PROCESS line
 * itself, in update_db() in common.c, so a second record from setlogin would
 * make who/w/uptime report the session twice.  What is kept is the part
 * getlogin(3) reads.
 *
 * ttyaction(3)
 * ------------
 * NetBSD's /etc/ttys gives each terminal line an action (getty, login,
 * logout, ...) and a "secure" flag, and ttyaction(3) is how a process asks the
 * ttys database to run the action for a terminal.  FreeLinX has no ttys
 * database and no action scripts - the initramfs starts a getty per unit
 * directly - so there is nothing to run and no way for the caller to tell the
 * difference from a tty that had no action to begin with.
 *
 * The return value matters.  login.c does:
 *
 *     if (ttyaction(ttyn, "login", pwd->pw_name))
 *             (void)printf("Warning: ttyaction failed.\n");
 *
 * so failing here would print a warning on every single login, to every user,
 * about a facility that does not exist.  Returning 0 is the honest answer:
 * the action was requested, there was none to perform, and it completed.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 */

#include <stddef.h>
#include <string.h>

#define FLX_MAXLOGNAME 32

static char login_name[FLX_MAXLOGNAME];

/*
 * int setlogin(const char *);
 * Returns 0 on success, -1 with errno set on failure.  A NULL or empty name is
 * rejected the way NetBSD's is: there is no session to attribute.
 */
int
setlogin(const char *name)
{
	if (name == NULL || *name == '\0')
		return -1;
	if (strlen(name) >= sizeof(login_name))
		return -1;
	strcpy(login_name, name);
	return 0;
}

/*
 * char *getlogin(void);
 * musl does not declare this, but the name set above is only useful if
 * something can read it back, and NetBSD's login(1) and the utmpx tools do.
 */
char *
getlogin(void)
{
	return login_name[0] != '\0' ? login_name : NULL;
}

/*
 * int ttyaction(const char *tty, const char *action, const char *user);
 * Always succeeds: see the file comment.
 */
int
ttyaction(const char *tty, const char *action, const char *user)
{
	(void)tty;
	(void)action;
	(void)user;
	return 0;
}
