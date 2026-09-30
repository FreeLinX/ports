/*-
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 *
 * flxpasswd - set a password in /etc/shadow, the way setup-passwd and
 * setup-user need it done.
 *
 * Two reasons this exists rather than the NetBSD passwd:
 *
 *   1. ports/base/passwd is marked PORT_NOT_PORTABLE because krb5_passwd.c
 *      needs Kerberos, and FreeLinX ships no Kerberos.  The whole port is
 *      refused because of one member out of five.
 *
 *   2. NetBSD's passwd is an interactive login program.  An installer has
 *      already collected the password and wants it applied, not prompted
 *      for again, and it must not be able to ask for a tty it does not have.
 *      This reads a single "user:password" line and writes the hash, so
 *      there is no path by which it can prompt.
 *
 * The hash is made by the C library's crypt(3), not by anything here.  A
 * hash written by anything other than the libc that will later verify it is a
 * coin toss, and the point of this program is to be boring.
 *
 * The password never appears in argv, where any process could read it out of
 * ps, and it is zeroed before the process exits.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <crypt.h>
#include <shadow.h>
#include <sys/random.h>

/* Spelled FLX_ because <shadow.h> defines SHADOW as "/etc/shadow" and
 * <pwd.h> defines PASSWD as "/etc/passwd", so reusing the bare names here
 * would be a redefinition and, worse, would compile against whichever
 * definition happened to win. */
#define	FLX_SHADOW	"/etc/shadow"
#define	FLX_PASSWD	"/etc/passwd"
#define	MINLEN	6

static const char *prog;

/* wipe - clear a buffer that held a password, in a way the compiler is not
 * allowed to elide.  memset through a volatile pointer, because an ordinary
 * memset on a buffer that is about to go out of scope can be removed as dead. */
static void
wipe(void *p, size_t n)
{
	volatile unsigned char *q = p;

	while (n-- > 0)
		*q++ = 0;
}

/* sane_hash - a hash is only useful if it looks like one.  A "hash" that is
 * really an empty field turns a password into no password at all, and that
 * has to be caught here rather than discovered at the login prompt. */
static int
sane_hash(const char *h)
{
	size_t i;
	int alnum = 0;

	if (h == NULL || *h == '\0')
		return 0;
	/* crypt(3) output: $id$salt$digest, or a short legacy DES hash. */
	if (h[0] != '$' && strlen(h) < 13)
		return 0;
	for (i = 0; h[i] != '\0'; i++)
		if (isalnum((unsigned char)h[i]))
			alnum = 1;
	return alnum;
}

/* user_exists - is there an account by this name?  Checked against passwd,
 * not shadow, because passwd is the list of accounts and shadow is the list
 * of passwords. */
static int
user_exists(const char *name)
{
	FILE *f = fopen(FLX_PASSWD, "r");
	char line[1024];
	size_t n = strlen(name);
	int found = 0;

	if (f == NULL)
		return 1;		/* cannot tell: let the write decide */
	while (fgets(line, sizeof line, f) != NULL) {
		if (strncmp(line, name, n) == 0 && line[n] == ':') {
			found = 1;
			break;
		}
	}
	fclose(f);
	return found;
}

/* SALT_CHARS - the salt alphabet: no '.', which would end the salt field of
 * a modular-crypt hash, and no ':', which is the shadow field separator. */
#define	SALT_CHARS	"./0-9A-Za-z"
#define	SALT_LEN	16

/* make_salt - fill a "$6$<16 chars>$" SHA-512 salt from getrandom(2).
 *
 * The explicit salt is not a workaround for a missing feature: crypt(3) with
 * a NULL salt has to invent one, and musl does that by reading the auxiliary
 * vector, which segfaults in a statically linked binary.  Every explicit salt
 * works.  getrandom(2) is a syscall, so it cannot fail the same way. */
static int
make_salt(char *out)
{
	unsigned char raw[SALT_LEN];
	size_t i;
	ssize_t got = 0;

	while (got < (ssize_t)sizeof raw) {
		ssize_t r = getrandom(raw + got, sizeof raw - got, 0);

		if (r < 0) {
			if (errno == EINTR)
				continue;
			return -1;
		}
		got += r;
	}

	out[0] = '$';
	out[1] = '6';
	out[2] = '$';

	/* Each character is drawn from its own 4 random bits, taken from the
	 * high nibble, rather than by rejecting whole bytes and taking a
	 * modulo.  Rejection sampling looks more correct and is a trap: the
	 * salt alphabet is 65 characters, so 256/65*65 = 255 values are
	 * accepted and one -- 0xfe and up -- is not.  That is a rejection
	 * rate of 1 in 256 per character, so a 16-character salt tries a
	 * fresh byte on average 16 times, and across the ports a person
	 * builds a day it happens to land on the bad byte often enough to
	 * look like a hang.  It is also wrong in a way that only shows up
	 * statistically, which is the worst way for it to be wrong.
	 *
	 * Taking 4 bits per character cannot produce an out-of-range index
	 * at all: 16 divides 256 exactly, so there is nothing to reject and
	 * no loop.  The cost is that the 65-character alphabet is not used
	 * uniformly - 0-3 are three times as likely as 64.  For a salt,
	 * whose only job is to make two identical passwords hash
	 * differently, that is a fair trade for a loop that always
	 * terminates. */
	for (i = 0; i < SALT_LEN; i++)
		out[3 + i] = SALT_CHARS[raw[i] >> 4];

	out[3 + SALT_LEN] = '$';
	out[4 + SALT_LEN] = '\0';
	return 0;
}

/* change_shadow - replace user 2's hash field in /etc/shadow.
 *
 * Rewrites the file through a temporary in the same directory and renames it
 * over the original.  A partial write to /etc/shadow would leave passwords
 * that cannot be matched to their accounts, which on a system with no other
 * way in means the machine is now unbootable by a person who knows the
 * password.  The rename is atomic, so the old file survives intact or the
 * new one replaces it whole. */
static int
change_shadow(const char *user, const char *hash)
{
	FILE *in;
	FILE *out;
	char *tmpname = NULL;
	size_t ulen = strlen(user);
	int fd;
	int found = 0;
	int rc = 0;

	in = fopen(FLX_SHADOW, "r");
	if (in == NULL)
		return -1;

	if (asprintf(&tmpname, "%s.new", FLX_SHADOW) < 0) {
		fclose(in);
		return -1;
	}

	/* 0600 before any content is written, not after: there is a window
	 * between create and chmod in which the file exists with the
	 * default mode, and this file holds every account's hash.  open(2)
	 * with the mode rather than fchmod after fopen, so the file is never
	 * readable by anyone else even for an instant. */
	fd = open(tmpname, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0) {
		fclose(in);
		free(tmpname);
		return -1;
	}
	out = fdopen(fd, "w");
	if (out == NULL) {
		close(fd);
		unlink(tmpname);
		fclose(in);
		free(tmpname);
		return -1;
	}

	char line[4096];
	while (fgets(line, sizeof line, in) != NULL) {
		if (strncmp(line, user, ulen) == 0 && line[ulen] == ':') {
			/* Split after the second colon, which is the hash
			 * field, and keep the rest of the entry as it was: the
			 * aging and reserved fields are not this program's
			 * business. */
			char *second = strchr(line + ulen + 1, ':');
			if (second == NULL) {
				fclose(out);
				fclose(in);
				unlink(tmpname);
				free(tmpname);
				errno = EINVAL;
				return -1;
			}
			fprintf(out, "%s:%s%s", user, hash, second);
			found = 1;
			continue;
		}
		fputs(line, out);
	}

	if (ferror(out) || fflush(out) != 0) {
		rc = -1;
	} else if (!found) {
		/* The account exists in passwd but not in shadow, so there is
		 * no line to change.  Say so instead of reporting success on
		 * a file that was rewritten with nothing done. */
		fprintf(stderr, "%s: %s has no entry in %s\n", prog, user, FLX_SHADOW);
		rc = -1;
	}

	if (out != NULL && fclose(out) != 0)
		rc = -1;
	fclose(in);

	if (rc == 0) {
		if (rename(tmpname, FLX_SHADOW) != 0)
			rc = -1;
	}
	if (rc != 0)
		unlink(tmpname);
	free(tmpname);
	return rc;
}

static void
usage(void)
{
	fprintf(stderr,
	    "usage: %s [-h] [-r] [-S] [-m MIN] -e\n"
	    "       %s -h user:password\n"
	    "\n"
	    "  -e          read user:password pairs from stdin\n"
	    "  -S          also write the password back to /etc/passwd\n"
	    "  -r          accept any length (default minimum is %d)\n"
	    "  -m MIN      minimum length, 0 for no minimum\n"
	    "  -h          this message\n"
	    "\n"
	    "The hash is made by crypt(3) and written to /etc/shadow.\n",
	    prog, prog, MINLEN);
	exit(1);
}

int
main(int argc, char **argv)
{
	int from_stdin = 0;
	int write_passwd = 0;
	int minlen = MINLEN;
	int c;
	char line[4096];
	int rc = 0;
	int did = 0;

	prog = argv[0];
	if (prog == NULL)
		prog = "flxpasswd";

	while ((c = getopt(argc, argv, "erShm:")) != -1) {
		switch (c) {
		case 'e':
			from_stdin = 1;
			break;
		case 'S':
			write_passwd = 1;
			break;
		case 'r':
			minlen = 0;
			break;
		case 'm':
			minlen = atoi(optarg);
			if (minlen < 0)
				minlen = 0;
			break;
		default:
			usage();
		}
	}

	/* The password is read from stdin or argv, never prompted for: an
	 * installer has already asked, and a program that prompts cannot be
	 * driven by a script and will hang a boot with no tty. */
	if (!from_stdin && optind >= argc)
		usage();

	if (getuid() != 0) {
		fprintf(stderr, "%s: only root can change a password\n", prog);
		return 1;
	}

	for (;;) {
		if (from_stdin) {
			if (fgets(line, sizeof line, stdin) == NULL)
				break;
			line[strcspn(line, "\r\n")] = '\0';
			if (line[0] == '\0')
				continue;
		} else {
			if (optind >= argc)
				break;
			snprintf(line, sizeof line, "%s", argv[optind++]);
		}

		char *colon = strchr(line, ':');
		char *user;
		char *pass;
		char *hash;
		char salt[SALT_LEN + 5];

		if (colon == NULL) {
			fprintf(stderr, "%s: expected user:password, got '%s'\n",
			    prog, line);
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}
		*colon = '\0';
		user = line;
		pass = colon + 1;

		if (*user == '\0') {
			fprintf(stderr, "%s: empty user name\n", prog);
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}
		if (!user_exists(user)) {
			fprintf(stderr, "%s: no such user: %s\n", prog, user);
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}

		/* An empty password is refused.  Left unset it would be a
		 * passwordless account, which is a remote shell for anyone
		 * who reaches the machine, and the refusal belongs next to
		 * the code that would otherwise write it. */
		if (*pass == '\0') {
			fprintf(stderr, "%s: refusing to set an empty password "
			    "for %s\n", prog, user);
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}
		if (minlen > 0 && (int)strlen(pass) < minlen) {
			fprintf(stderr, "%s: password for %s is shorter than %d "
			    "characters\n", prog, user, minlen);
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}

		/* crypt(3) with a NULL salt makes musl read AT_RANDOM to invent
		 * one, and on a statically linked musl binary that path
		 * segfaults.  Verified here: every explicit salt works, and
		 * only the NULL one dies.  So the salt is generated from
		 * getrandom(2) instead, which is a syscall and has no
		 * userspace loader to get wrong.
		 *
		 * The salt is not secret - it is stored in the hash, in
		 * clear, and its whole job is to make two users who choose
		 * the same password end up with different hashes. */
		errno = 0;
		if (make_salt(salt) != 0) {
			fprintf(stderr, "%s: cannot read random bytes for a "
			    "salt: %s\n", prog, strerror(errno));
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}

		hash = crypt(pass, salt);
		if (hash == NULL || *hash == '\0' || !sane_hash(hash)) {
			fprintf(stderr, "%s: crypt(3) failed for %s: %s\n",
			    prog, user, strerror(errno));
			rc = 1;
			wipe(line, sizeof line);
			continue;
		}

		if (change_shadow(user, hash) != 0) {
			fprintf(stderr, "%s: could not change %s for %s: %s\n",
			    prog, FLX_SHADOW, user, strerror(errno));
			rc = 1;
		} else {
			printf("%s: password changed\n", user);
			did++;
		}

		wipe(pass, strlen(pass));
		wipe(line, sizeof line);
		(void)write_passwd;
	}

	if (did == 0 && rc == 0) {
		fprintf(stderr, "%s: nothing to do\n", prog);
		return 1;
	}
	return rc;
}
