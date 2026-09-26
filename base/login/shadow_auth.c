/*
 * FreeLinX/ports - base/login : the password database musl does not give us.
 *
 * WHY THIS FILE EXISTS
 *
 * NetBSD keeps the password hash and the account policy in one file, the
 * passwd database, and login(1) uses both through struct passwd:
 *
 *   pw_passwd   the crypt(3) hash, compared directly:
 *                 !strcmp(crypt(p, pwd->pw_passwd), pwd->pw_passwd)
 *   pw_passwd[0] == '\0'  meaning "this account needs no password"
 *   pw_expire   absolute time the *account* expires, 0 = never
 *   pw_change   0 = the password never expires on age,
 *               _PASSWORD_CHGNOW (-1) = change it now,
 *               otherwise the absolute time it expires
 *
 * FreeLinX uses the two-file arrangement every Linux system uses, because a
 * world-readable passwd file is not acceptable for a hash database:
 *
 *   /etc/passwd  account data, second field "x"
 *   /etc/shadow  the hash and the policy, mode 0600
 *
 * musl's getpwnam(3) reads /etc/passwd, so it hands back "x" as the hash and
 * has no pw_expire/pw_change at all - and no libc struct can be extended to
 * give it them, because the same struct is what every other program in the
 * system is compiled against.
 *
 * That is not a cosmetic gap.  A login built on musl's struct passwd alone
 * compares crypt(p, "x") against "x", which never matches, so every
 * password login fails and the machine is unreachable through its own console.
 * So this file supplies the four values login(1) needs, read from /etc/shadow:
 *
 *   field 1  the crypt(3) hash
 *   field 2  lastchg  days since the epoch the password was last changed
 *   field 4  max      days the password stays valid after that; 0 = never
 *   field 6  inact    days after the password expired before the account is
 *                    locked; 0 = the account is never locked on age
 *   field 7  expire   days since the epoch the account expires; 0 = never
 *
 * A missing or unreadable /etc/shadow yields the same answers NetBSD's login
 * gives when the database says nothing: no hash, so authentication fails; no
 * expiry, so nobody is nagged.  login(1) is setuid root and must not refuse a
 * login because an advisory file is missing - but it must also not accept one
 * it cannot verify, which is why a missing hash is a failure, not a bypass.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 */

#include <sys/types.h>

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "shadow_auth.h"

#ifndef FLX_SHADOW_PATH
#define FLX_SHADOW_PATH "/etc/shadow"
#endif

#define FLX_SECSPERDAY 86400L
#define FLX_SHADOW_FIELDS 9
#define FLX_MAX_HASH 256

struct shadow_ent {
	char hash[FLX_MAX_HASH];
	long lastchg;
	long maxdays;
	long inact;
	long expire;
	int found;
};

/*
 * Split a colon-separated line in place.  Returns the number of fields found,
 * capped at maxf.
 */
static int
split(char *line, char *fld[], int maxf)
{
	int n = 0;

	if (maxf < 1)
		return 0;
	fld[n++] = line;
	for (char *p = line; *p != '\0'; p++) {
		if (*p != ':')
			continue;
		*p = '\0';
		if (n < maxf)
			fld[n++] = p + 1;
	}
	return n;
}

static void
chomp(char *s)
{
	size_t n = strlen(s);

	while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
		s[--n] = '\0';
}

static long
num_or_zero(const char *s)
{
	if (s == NULL || *s == '\0')
		return 0;
	return strtol(s, NULL, 10);
}

/*
 * Copy the value of field `want` (1-based) of the first line whose field 0 is
 * `name`, into buf.  Returns 1 if such a line exists.
 */
static int
field_of(const char *path, const char *name, int want, char *buf, size_t bufsz)
{
	FILE *f;
	char line[1024];
	char *fld[FLX_SHADOW_FIELDS + 1];
	int found = 0;

	if (bufsz > 0)
		buf[0] = '\0';
	f = fopen(path, "re");
	if (f == NULL)
		return 0;
	while (fgets(line, sizeof(line), f) != NULL) {
		chomp(line);
		if (line[0] == '\0' || line[0] == '#')
			continue;
		if (split(line, fld, FLX_SHADOW_FIELDS + 1) < want)
			continue;
		if (strcmp(fld[0], name) != 0)
			continue;
		if (buf != NULL && bufsz > 0)
			snprintf(buf, bufsz, "%s", fld[want - 1]);
		found = 1;
		break;
	}
	fclose(f);
	return found;
}

static void
read_shadow(const char *name, struct shadow_ent *e)
{
	char line[1024];
	char *fld[FLX_SHADOW_FIELDS + 1];
	FILE *f;

	memset(e, 0, sizeof(*e));
	f = fopen(FLX_SHADOW_PATH, "re");
	if (f == NULL)
		return;
	while (fgets(line, sizeof(line), f) != NULL) {
		chomp(line);
		if (line[0] == '\0' || line[0] == '#')
			continue;
		if (split(line, fld, FLX_SHADOW_FIELDS) < FLX_SHADOW_FIELDS)
			continue;
		if (strcmp(fld[0], name) != 0)
			continue;
		snprintf(e->hash, sizeof(e->hash), "%s", fld[1]);
		e->lastchg = num_or_zero(fld[2]);
		e->maxdays = num_or_zero(fld[4]);
		e->inact   = num_or_zero(fld[6]);
		e->expire  = num_or_zero(fld[7]);
		e->found = 1;
		break;
	}
	fclose(f);
}

/*
 * The crypt(3) hash to authenticate against, or NULL when there is none.
 *
 * /etc/shadow wins when it has an entry with a non-empty second field.  An
 * entry whose second field is empty falls back to /etc/passwd, which is what
 * the two files have always meant on a Linux system: an empty shadow field
 * says "no password here", not "no shadow entry".  That fallback is also what
 * keeps a NetBSD-style single-file passwd working unchanged.
 *
 * A hash that cannot be a hash is reported as no hash rather than passed to
 * crypt(3):
 *
 *   ""   no password anywhere
 *   "*"  no password login (the traditional "no login" marker)
 *   "!"  locked: a password that cannot be matched
 *   "!!" locked and never had one
 *   "x"  no hash here, look in the other file - the placeholder the shadow
 *        tools leave in /etc/passwd, which is what a FreeLinX system's
 *        /etc/passwd holds for every account
 *
 * A leading "!" is the whole convention, and its presence after the first
 * character means the account is locked, not that the hash is the rest.
 *
 * The "x" case is the one this port is most likely to meet, and it is worth
 * spelling out.  crypt(3) reads its first two characters as the salt, so
 * crypt(p, "x") does not fail - it returns a perfectly valid one-character-salt
 * DES hash of the password, which then fails to match "x".  The login is
 * refused either way, so the outcome does not depend on this test; it is here
 * so that the refusal is a decision rather than a coincidence, and so a
 * two-character placeholder can never be mistaken for a real (if archaic) salt.
 */
const char *
flx_shadow_hash(const char *name)
{
	static char buf[FLX_MAX_HASH];
	const struct passwd *pw;

	if (name == NULL || *name == '\0')
		return NULL;

	if (field_of(FLX_SHADOW_PATH, name, 2, buf, sizeof(buf))) {
		if (buf[0] != '\0')
			return (buf[0] == '!' || buf[0] == '*' || buf[0] == 'x')
			    ? NULL : buf;
	}

	/* no shadow entry, or an empty hash field: fall back to /etc/passwd */
	pw = getpwnam(name);
	if (pw == NULL || pw->pw_passwd == NULL)
		return NULL;
	if (pw->pw_passwd[0] == '\0' || pw->pw_passwd[0] == 'x')
		return NULL;
	if (pw->pw_passwd[0] == '!' || pw->pw_passwd[0] == '*')
		return NULL;
	snprintf(buf, sizeof(buf), "%s", pw->pw_passwd);
	return buf;
}

/*
 * 1 when the account exists but must not be logged in to.
 *
 * Two independent reasons:
 *
 *   the hash is one of the "no login" markers - see flx_shadow_hash()
 *   the password is older than max and the inact window has run out, which
 *     shadow(5) defines as an account that is disabled rather than one that
 *     keeps being nagged to change its password
 *
 * The second is the reason this is a separate query and not just "the hash is
 * bad": on a Linux system the hash of an account disabled for inactivity is a
 * perfectly good hash, so authentication against it would succeed.
 */
int
flx_shadow_locked(const char *name)
{
	struct shadow_ent e;
	long chgday, nowdays;

	if (name == NULL || *name == '\0')
		return 0;

	read_shadow(name, &e);
	if (!e.found)
		return 0;
	if (e.hash[0] == '!' || e.hash[0] == '*' || e.hash[0] == '\0')
		return 1;
	if (e.maxdays == 0 || e.inact == 0)
		return 0;

	chgday = e.lastchg + e.maxdays;
	nowdays = (long)(time(NULL) / FLX_SECSPERDAY);
	return nowdays >= chgday + e.inact;
}

/*
 * The time the account expires, as an absolute time_t.  0 = never, which is
 * what NetBSD's pw_expire = 0 means and what login(1) tests for.
 */
time_t
flx_shadow_account_expire(const char *name)
{
	struct shadow_ent e;

	if (name == NULL)
		return 0;
	read_shadow(name, &e);
	if (!e.found || e.expire == 0)
		return 0;
	return (time_t)(e.expire * FLX_SECSPERDAY);
}

/*
 * The time the password expires, as an absolute time_t, FLX_PASSWORD_CHGNOW
 * when it must be changed immediately, or 0 when it never expires on age or
 * when the account is locked - a locked account has its own answer, and
 * reporting "change your password" for it would send the user down the wrong
 * path.
 *
 * shadow(5) has no "must change now" value: the state of "older than max" is
 * what a max of 0 means "never" and a non-zero max means "expired".  NetBSD
 * distinguishes them because its database stores the decision; login(1)
 * distinguishes them because it acts on the two differently - refuse the login
 * against asking for a password change.  This is where that one real
 * difference between the two password databases is resolved.
 */
time_t
flx_shadow_password_expire(const char *name)
{
	struct shadow_ent e;
	long chgday, nowdays;

	if (name == NULL)
		return 0;
	read_shadow(name, &e);
	if (!e.found || e.maxdays == 0)
		return 0;

	chgday = e.lastchg + e.maxdays;
	nowdays = (long)(time(NULL) / FLX_SECSPERDAY);

	if (chgday > nowdays)
		return (time_t)(chgday * FLX_SECSPERDAY);
	if (e.inact > 0 && nowdays >= chgday + e.inact)
		return 0;		/* locked: flx_shadow_locked() answers this */
	return FLX_PASSWORD_CHGNOW;
}
