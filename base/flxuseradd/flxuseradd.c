/*-
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 *
 * flxuseradd - create a user account, the way setup-user needs it done.
 *
 * There is no useradd in FreeLinX and no port for one: ports/base/passwd is
 * PORT_NOT_PORTABLE over Kerberos, and it is an interactive program the
 * installer cannot drive anyway.  So the account handling the installer needs
 * is written here, non-interactively and against this system's own layout.
 *
 * It edits /etc/passwd, /etc/group and /etc/shadow by rewriting one line
 * each, in place, in a fixed order.  The order matters and is the whole
 * design:
 *
 *   1. pick a uid, or take the one asked for
 *   2. write the group line, so the account's primary group exists
 *   3. write the passwd line, so the account exists
 *   4. write the shadow line, so the account has a password field
 *
 * Each step is skipped if the account or group already exists, so this is
 * safe to re-run: creating an account that is already there reports that and
 * changes nothing, rather than producing a second passwd entry for the same
 * name, which is the failure mode that makes a system unmanageable.
 *
 * Nothing is prompted for.  An installer has already asked, and a program that
 * prompts cannot be driven by a script and will hang a boot with no tty.
 */

#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

/* Spelled FLX_ because <pwd.h> defines PASSWD and <grp.h> defines GROUP, and
 * reusing the bare names here would collide with the headers. */
#define	FLX_PASSWD	"/etc/passwd"
#define	FLX_GROUP	"/etc/group"
#define	FLX_SHADOW	"/etc/shadow"
#define	FLX_SKEL	"/etc/skel"

#define	MIN_UID		1000
#define	MAX_UID		60000
#define	MIN_GID		1000
#define	MAX_GID		60000
/* The privileged accounts go up to 655, so 60000 leaves room to grow without
 * ever colliding with a system id. */
#define	DEFAULT_SHELL	"/bin/sh"

static const char *prog;
static int verbose;

/* shell_quote - wrap s in single quotes for sh, so a value containing a
 * space or a quote cannot break the command that uses it.  Every path and
 * group name reaches a shell command line in this program, so this is not
 * theoretical: a home directory of /home/o'brien is a legal path. */
static const char *
shell_quote(const char *s)
{
	static char buf[8][2048];
	static int n;
	char *out;
	const char *p;

	n = (n + 1) % 8;
	out = buf[n];
	if (s == NULL)
		s = "";
	if (strlen(s) * 3 + 3 >= sizeof buf[0])
		return s;		/* too long to quote safely; the caller
					 * has already bounded the length */

	/* Opening quote, and 'x' escaping for every single quote inside. */
	p = s;
	{
		char *w = out;

		*w++ = '\'';
		while (*p != '\0') {
			if (*p == '\'') {
				*w++ = '\'';
				*w++ = '\\';
				*w++ = '\'';
			} else {
				*w++ = *p;
			}
			p++;
		}
		*w++ = '\'';
		*w = '\0';
	}
	return out;
}

/* split - break a colon-separated line into NUL-terminated fields.
 *
 * Every colon in the line becomes a NUL, and the pointers to the starts of
 * the fields are stored in fv.  Returns the number of fields found, or -1 if
 * the line is empty.
 *
 * This replaces the obvious-looking "return a pointer to the nth field" on
 * purpose.  Such a helper hands back a pointer into the middle of the line,
 * and every caller then compares it with strcmp against the whole rest of the
 * line: "wheel:x:10:flxuser" compares unequal to "wheel" and nothing is ever
 * found.  Terminating the fields in place is what makes strcmp mean what it
 * looks like it means.
 *
 * MAX_FIELDS is 16: passwd needs 7, group needs 4, shadow needs 9. */
#define	MAX_FIELDS	16

static int
split(char *line, char *fv[MAX_FIELDS])
{
	int n = 0;
	char *p = line;

	if (*line == '\0')
		return -1;
	fv[n++] = p;
	while (*p != '\0' && n < MAX_FIELDS) {
		if (*p != ':') {
			p++;
			continue;
		}
		*p = '\0';
		fv[n++] = p + 1;
		p++;
	}
	/* A line with more colons than MAX_FIELDS has the tail left in the
	 * last field, which is a malformed line; the callers that care check
	 * the field count. */
	return n;
}

/* each_field - call fn for every line of a colon-separated file, stopping if
 * fn returns non-zero.  The line is handed over writable and is clobbered, so
 * fn must copy anything it keeps. */
static int
each_field(const char *path, int (*fn)(char *, void *), void *arg)
{
	FILE *f;
	char line[4096];
	int rc = 0;

	f = fopen(path, "r");
	if (f == NULL)
		return -1;
	while (fgets(line, sizeof line, f) != NULL) {
		line[strcspn(line, "\n")] = '\0';
		rc = fn(line, arg);
		if (rc != 0)
			break;
	}
	fclose(f);
	return rc;
}

/* --- uid and gid selection ------------------------------------------------ */

struct want {
	const char *name;
	long found;		/* the id, or -1 if the name is not present */
	long max;
	int is_group;
};

/* scan_ids - note whether the name already exists, and the highest id in the
 * file, so a new account can be given an id nothing else is using.
 *
 * The id field is the same number in both files on purpose: passwd is
 * name:passwd:uid:gid:gecos:dir:shell and group is name:passwd:gid:members,
 * so field 2 is the uid in one and the gid in the other. */
static int
scan_ids(char *line, void *arg)
{
	struct want *w = arg;
	char *fv[MAX_FIELDS];
	long v;

	if (split(line, fv) < 3)
		return 0;
	if (strcmp(fv[0], w->name) == 0) {
		w->found = strtol(fv[2], NULL, 10);
		return 1;		/* stop: this one is found */
	}
	/* Skip the privileged accounts when working out where to start, or a
	 * file containing nobody at 65534 pushes every new account to
	 * 65535.  Only the normal-account range is worth counting. */
	v = strtol(fv[2], NULL, 10);
	if (v > w->max && v >= MIN_UID && v < MAX_UID)
		w->max = v;
	return 0;
}

/* scan_gid - find one group's gid.  Field 3, not field 2: a group line is
 * name:passwd:gid:members, and field 2 of a group is its gid, so this is
 * only correct if the caller is really reading a group file.  Kept separate
 * from scan_ids so the field number is stated where it is used. */
static int
scan_gid(char *line, void *arg)
{
	struct want *w = arg;
	char *fv[MAX_FIELDS];

	if (split(line, fv) < 3)
		return 0;
	if (strcmp(fv[0], w->name) == 0) {
		w->found = strtol(fv[2], NULL, 10);
		return 1;
	}
	return 0;
}

static long
find_free_id(const char *name, const char *path, long lo, long hi, int is_group)
{
	struct want w;
	long id;

	memset(&w, 0, sizeof w);
	w.name = name;
	w.found = -1;
	w.max = 0;
	w.is_group = is_group;
	if (each_field(path, scan_ids, &w) < 0)
		return -1;
	if (w.found >= 0)
		return w.found;		/* already exists */

	id = w.max + 1;
	if (id < lo)
		id = lo;
	if (id >= hi)
		id = lo;
	return id;
}

/* --- file rewriting ------------------------------------------------------- */

/* rewrite - replace or append one line in a colon-separated file.
 *
 * The file is written to a temporary beside it and renamed over the
 * original, so a failure part way through leaves the old file intact.  A
 * half-written /etc/passwd is a machine whose accounts no longer resolve.
 *
 * key names the line to replace: the entry's first field, which is the
 * account or group name in all three of passwd, group and shadow.  new_line
 * is the complete replacement, without a trailing newline.  Returns 0 on
 * success and -1 on error; a key that is not present is appended, which is
 * what every caller here wants. */
static int
rewrite(const char *path, const char *key, const char *new_line)
{
	char *tmpname = NULL;
	FILE *in;
	FILE *out;
	char line[4096];
	int found = 0;
	int rc = 0;
	int fd;
	mode_t mode;
	struct stat st;

	/* shadow holds every account's hash and must be 0600.  passwd and
	 * group hold no secrets and are read by everything on the system, so
	 * they must stay world readable: 0600 there breaks every tool that
	 * looks up a name as a non-root user, which is most of them.  The
	 * mode is copied from the file being replaced, so whatever the
	 * system already had is preserved. */
	if (stat(path, &st) == 0)
		mode = st.st_mode & 07777;
	else
		mode = (strcmp(path, FLX_SHADOW) == 0) ? 0600 : 0644;

	in = fopen(path, "r");
	if (in == NULL)
		return -1;

	if (asprintf(&tmpname, "%s.flxnew", path) < 0) {
		fclose(in);
		return -1;
	}

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

	/* The replacement has to be built and matched before split() runs:
	 * split() writes NULs over the colons, and if a line turns out not to
	 * be the one being replaced it still has to be written out intact.
	 * Matching first and splitting only on a hit avoids destroying the
	 * lines that are being copied through. */
	while (fgets(line, sizeof line, in) != NULL) {
		size_t n = strcspn(line, "\n");
		int is_key;

		line[n] = '\0';

		/* Compare the key field without splitting the whole line: the
		 * name is field 0, so it ends at the first colon. */
		{
			const char *k = line;
			const char *e = strchr(k, ':');
			size_t klen = e != NULL ? (size_t)(e - k) : strlen(k);

			is_key = !found && strlen(key) == klen &&
			    strncmp(k, key, klen) == 0;
		}

		if (is_key) {
			if (fprintf(out, "%s\n", new_line) < 0)
				rc = -1;
			found = 1;
			continue;
		}
		if (fputs(line, out) == EOF)
			rc = -1;
		if (fputc('\n', out) == EOF)
			rc = -1;
	}

	if (rc == 0 && !found) {
		if (fprintf(out, "%s\n", new_line) < 0)
			rc = -1;
	}
	if (ferror(out) || fflush(out) != 0)
		rc = -1;
	if (fclose(out) != 0)
		rc = -1;
	fclose(in);

	if (rc == 0) {
		/* On the temp file, not on the result: the rename below
		 * replaces the inode, so a mode set afterwards would apply to
		 * an unlinked file. */
		if (chmod(tmpname, mode) != 0)
			rc = -1;
		else if (rename(tmpname, path) != 0)
			rc = -1;
	}
	if (rc != 0)
		unlink(tmpname);
	free(tmpname);
	return rc;
}

/* in_list - is name one of the comma-separated names in list?
 *
 * A substring test would say "carol" is already a member of a group
 * containing "caroline", and then silently leave the real carol out. */
static int
in_list(const char *list, const char *name)
{
	const char *p = list;
	size_t n = strlen(name);

	while (*p != '\0') {
		const char *e = strchr(p, ',');
		size_t len = e != NULL ? (size_t)(e - p) : strlen(p);

		if (len == n && strncmp(p, name, n) == 0)
			return 1;
		if (e == NULL)
			break;
		p = e + 1;
	}
	return 0;
}

/* home_root - where home directories go.  /home normally, but the tests
 * redirect it into a scratch directory, and a hard-coded /home would make
 * them write outside it.  Set from the FLX_HOME environment variable when
 * present. */
static const char *
home_root(void)
{
	const char *h = getenv("FLX_HOME");

	return (h != NULL && *h != '\0') ? h : "/home";
}

static int
account_exists(const char *name)
{
	FILE *f = fopen(FLX_PASSWD, "r");
	char line[4096];
	int found = 0;

	if (f == NULL)
		return 0;
	while (fgets(line, sizeof line, f) != NULL) {
		char *p = line;
		size_t n = strlen(name);

		if (strncmp(p, name, n) == 0 && p[n] == ':') {
			found = 1;
			break;
		}
	}
	fclose(f);
	return found;
}

static int
group_exists(const char *name)
{
	FILE *f = fopen(FLX_GROUP, "r");
	char line[4096];
	int found = 0;

	if (f == NULL)
		return 0;
	while (fgets(line, sizeof line, f) != NULL) {
		char *p = line;
		size_t n = strlen(name);

		if (strncmp(p, name, n) == 0 && (p[n] == ':' || p[n] == '\n')) {
			found = 1;
			break;
		}
	}
	fclose(f);
	return found;
}

/* --- home directory ------------------------------------------------------- */

static int
make_home(const char *home, int uid, int gid)
{
	char cmd[4096];
	char shell[256];
	FILE *f;
	int rc = 0;

	if (mkdir(home, 0755) != 0 && errno != EEXIST) {
		fprintf(stderr, "%s: cannot create %s: %s\n", prog, home,
		    strerror(errno));
		return -1;
	}

	/* Copy the skeleton rather than leaving the home empty, so a new
	 * account has the same starting point as every other one.  Done in sh
	 * because there is no portable recursive copy in C, and sh is not a
	 * dependency this program should be introducing. */
	if (snprintf(cmd, sizeof cmd,
	    "cd %s 2>/dev/null && for f in .*; do [ -e \"$f\" ] || continue; "
	    "cp -p \"$f\" %s/ 2>/dev/null; done; "
	    "for f in *; do [ -e \"$f\" ] || continue; "
	    "cp -p \"$f\" %s/ 2>/dev/null; done",
	    shell_quote(FLX_SKEL), shell_quote(home), shell_quote(home)) > 0) {
		if (system(cmd) != 0)
			rc = -1;		/* not fatal: an empty home is usable */
	}

	/* Chown without following a symlink, and only files this process owns,
	 * so a skeleton that points at a shared file does not change it. */
	if (snprintf(cmd, sizeof cmd,
	    "chown -h %s %s 2>/dev/null; "
	    "find %s -maxdepth 1 ! -samefile %s -exec chown -h %d:%d {} + "
	    "2>/dev/null; true", shell_quote(home), shell_quote(home),
	    shell_quote(home), shell_quote(home), uid, gid) > 0) {
		if (system(cmd) != 0)
			rc = -1;
	}

	/* The shell is copied because a login shell will not start without
	 * one, and a system with no shells is a system nobody can log into. */
	if (access(FLX_SKEL, F_OK) == 0 &&
	    snprintf(shell, sizeof shell, "%s/.profile", home) > 0) {
		if (access(shell, F_OK) != 0) {
			f = fopen(shell, "w");
			if (f != NULL) {
				fputs("# Written by flxuseradd.\n", f);
				fclose(f);
			}
		}
	}
	return rc;
}

static void
usage(void)
{
	fprintf(stderr,
	    "usage: %s -u NAME [-g GID] [-G GROUPS] [-d HOME] [-s SHELL]\n"
	    "       %s -a GROUPS -u NAME            add NAME to existing GROUPS\n"
	    "\n"
	    "  -u NAME      the account to create (required)\n"
	    "  -g GID       primary group id, or a group name to create\n"
	    "  -G GROUPS    comma-separated extra groups to join\n"
	    "  -d HOME      home directory (default /home/NAME)\n"
	    "  -s SHELL     login shell (default %s)\n"
	    "  -U UID       explicit uid\n"
	    "  -N           no home directory\n"
	    "  -v           say what is being done\n"
	    "  -h           this message\n"
	    "\n"
	    "The account is created with a locked password field; set the\n"
	    "password with flxpasswd.\n",
	    prog, prog, DEFAULT_SHELL);
	exit(1);
}

int
main(int argc, char **argv)
{
	const char *name = NULL;
	const char *gname = NULL;
	const char *groups = NULL;
	const char *home = NULL;
	const char *shell = DEFAULT_SHELL;
	long uid = -1;
	long gid = -1;
	int makehome = 1;
	int c;
	int created = 0;
	char line[4096];
	char homebuf[1024];

	prog = argv[0];
	if (prog == NULL)
		prog = "flxuseradd";

	while ((c = getopt(argc, argv, "u:g:G:d:s:U:Nvh")) != -1) {
		switch (c) {
		case 'u': name = optarg; break;
		case 'g': gname = optarg; break;
		case 'G': groups = optarg; break;
		case 'd': home = optarg; break;
		case 's': shell = optarg; break;
		case 'U': uid = strtol(optarg, NULL, 10); break;
		case 'N': makehome = 0; break;
		case 'v': verbose = 1; break;
		default: usage();
		}
	}
	if (name == NULL)
		usage();

	/* A useradd(8) name: lower case letters, digits, dash, underscore,
	 * starting with a letter or underscore.  Anything else ends up in a
	 * path or a shell somewhere later, so it is refused here. */
	{
		const char *p;

		if (!((*name >= 'a' && *name <= 'z') || *name == '_')) {
			fprintf(stderr, "%s: '%s' must start with a lower case "
			    "letter or an underscore\n", prog, name);
			return 1;
		}
		for (p = name; *p != '\0'; p++) {
			if ((*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') ||
			    *p == '-' || *p == '_')
				continue;
			fprintf(stderr, "%s: '%s' may only contain lower case "
			    "letters, digits, '-' and '_'\n", prog, name);
			return 1;
		}
	}

	if (getuid() != 0) {
		fprintf(stderr, "%s: only root can create an account\n", prog);
		return 1;
	}

	if (home == NULL) {
		snprintf(homebuf, sizeof homebuf, "%s/%s", home_root(), name);
		home = homebuf;
	}

	/* Already there: say so and change nothing.  A second passwd entry for
	 * the same name is how a system's account list stops being
	 * trustworthy. */
	if (account_exists(name)) {
		fprintf(stderr, "%s: %s already exists\n", prog, name);
		return 1;
	}

	/* --- 1. ids ---
	 *
	 * Deliberately NOT getpwuid/getgrnam: those read the system's
	 * databases, which is /etc/passwd on the running system and is not
	 * the file being edited when the paths have been redirected.  The
	 * files being written are the authority here.
	 *
	 * The uid and the gid are allocated independently.  They are usually
	 * the same number, because a new account gets a group of its own and
	 * the next free number is the same in both files -- but they are two
	 * separate files with two separate sets of ids in use, so one being
	 * taken does not mean the other is. */
	if (uid < 0)
		uid = find_free_id(name, FLX_PASSWD, MIN_UID, MAX_UID, 0);
	if (gname == NULL)
		gname = name;		/* one group per user, named for them */

	if (gid < 0) {
		if (group_exists(gname)) {
			/* Joining an existing group, e.g. wheel, needs its gid,
			 * read out of the group file. */
			struct want w;

			memset(&w, 0, sizeof w);
			w.name = gname;
			w.found = -1;
			w.max = 0;
			w.is_group = 1;
			each_field(FLX_GROUP, scan_gid, &w);
			gid = w.found;
		}
		if (gid < 0)
			gid = find_free_id(gname, FLX_GROUP, MIN_GID, MAX_GID, 1);
	}
	if (uid < 0 || gid < 0) {
		fprintf(stderr, "%s: cannot find a free uid/gid\n", prog);
		return 1;
	}

	/* --- 2. the group, so the passwd line's primary gid resolves --- */
	if (!group_exists(gname)) {
		snprintf(line, sizeof line, "%s:x:%ld:", gname, gid);
		if (rewrite(FLX_GROUP, gname, line) != 0) {
			fprintf(stderr, "%s: could not write %s: %s\n", prog,
			    FLX_GROUP, strerror(errno));
			return 1;
		}
		if (verbose)
			printf("%s: created group %s (gid %ld)\n", prog, gname, gid);
		created = 1;
	}

	/* --- extra groups, joined after the account exists --- */
	if (groups != NULL && *groups != '\0') {
		char *copy = strdup(groups);
		char *save = NULL;
		char *tok;

		for (tok = strtok_r(copy, ",", &save); tok != NULL;
		     tok = strtok_r(NULL, ",", &save)) {
			if (*tok == '\0')
				continue;
			/* Only groups that exist: silently inventing a group
			 * here would make a sudoers %group rule that matches
			 * nothing, which looks like sudo is broken. */
			if (!group_exists(tok)) {
				fprintf(stderr, "%s: no such group: %s (not "
				    "joined)\n", prog, tok);
				continue;
			}
			/* Re-read the group line and append, so this does not
			 * lose the members already in it. */
			{
				FILE *f = fopen(FLX_GROUP, "r");
				char gl[4096];

				if (f == NULL)
					continue;
				while (fgets(gl, sizeof gl, f) != NULL) {
					char *fv[MAX_FIELDS];
					char *nm;
					char *mem;
					size_t glen = strcspn(gl, "\n");

					/* The newline has to go before the line is
					 * split on colons or appended to: fgets
					 * leaves it, and it ends up written back
					 * into the file as a stray line. */
					gl[glen] = '\0';

					if (split(gl, fv) < 4)
						continue;
					nm = fv[0];
					if (strcmp(nm, tok) != 0)
						continue;
					mem = fv[3];
					if (mem == NULL) {
						fprintf(stderr, "%s: cannot read "
						    "the member list of %s\n",
						    prog, tok);
						break;
					}
					/* Not already a member.  The member list is
					 * comma separated, so this is an exact
					 * match on a whole element rather than a
					 * substring, which would find "carol"
					 * inside "caroline". */
					if (!in_list(mem, name)) {
						/* Rebuilt field by field, not from
						 * gl: split() has already put NULs
						 * where the colons were, so using
						 * gl here would write back only
						 * the first field and silently
						 * drop the rest of the line. */
						char joined[4096];
						size_t o = 0;
						int i;

						for (i = 0; i < 3; i++) {
							int w = snprintf(joined + o,
							    sizeof joined - o,
							    "%s%s", fv[i], ":");

							if (w < 0 ||
							    (size_t)w >= sizeof joined - o)
								break;
							o += (size_t)w;
						}
						snprintf(joined + o, sizeof joined - o,
						    "%s%s\n", mem,
						    (*mem == '\0') ? "" : ",");
						{
							size_t l = strcspn(joined, "\n");

							joined[l] = '\0';
						}
						snprintf(line, sizeof line, "%s%s",
						    joined, name);

						if (rewrite(FLX_GROUP, tok, line) == 0 && verbose)
							printf("%s: %s joined "
							    "group %s\n",
							    prog, name, tok);
					}
					break;
				}
				fclose(f);
			}
		}
		free(copy);
	}

	/* --- 3. the account --- */
	snprintf(line, sizeof line, "%s:x:%ld:%ld:%s:%s:%s", name, uid, gid,
	    "FreeLinX user", home, shell);
	if (rewrite(FLX_PASSWD, name, line) != 0) {
		fprintf(stderr, "%s: could not write %s: %s\n", prog,
		    FLX_PASSWD, strerror(errno));
		return 1;
	}
	if (verbose)
		printf("%s: created %s (uid %ld, gid %ld)\n", prog, name, uid, gid);
	created = 1;

	/* --- 4. the shadow line, with the password locked --- */
	snprintf(line, sizeof line, "%s:!:%ld:0:99999:7:::", name,
	    (long)0);
	if (rewrite(FLX_SHADOW, name, line) != 0)
		fprintf(stderr, "%s: warning: could not write %s, so %s has "
		    "no password field\n", prog, FLX_SHADOW, name);

	/* --- home directory, last, so a failure above leaves no stray
	 * directory owned by a uid that is not in passwd --- */
	if (makehome && make_home(home, (int)uid, (int)gid) != 0)
		fprintf(stderr, "%s: warning: %s was not populated from %s\n",
		    prog, home, FLX_SKEL);

	if (!created) {
		fprintf(stderr, "%s: nothing was done\n", prog);
		return 1;
	}
	if (verbose)
		printf("%s: set the password with: %s %s\n", prog, "flxpasswd",
		    name);
	return 0;
}
