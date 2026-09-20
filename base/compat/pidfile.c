/* FreeLinX/ports - base/compat/pidfile.c : BSD libutil pidfile(3).
 *
 * Real musl backend: create/lock the pidfile, truncate it and write the
 * daemon's PID.  If the file is already locked by a running process,
 * ENXIO is returned (NetBSD semantics).
 */
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int
pidfile(const char *path)
{
	char buf[32];
	int fd, n;

	if (path == NULL)
		path = "/var/run/pidfile";
	fd = open(path, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		return -1;
	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		close(fd);
		errno = ENXIO;
		return -1;
	}
	n = snprintf(buf, sizeof(buf), "%ld\n", (long)getpid());
	if (n < 0 || (size_t)n >= sizeof(buf)) {
		close(fd);
		errno = EIO;
		return -1;
	}
	if (ftruncate(fd, 0) == -1 || write(fd, buf, (size_t)n) != n) {
		close(fd);
		return -1;
	}
	return 0;
}
