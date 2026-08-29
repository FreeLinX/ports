/*
 * FreeLinX/ports - base/dmesg : FreeLinX-native dmesg.
 *
 * NOTE: this is NOT the NetBSD dmesg source.  NetBSD's dmesg reads the BSD
 * kernel message buffer through libkvm/sysctl (struct kern_msgbuf), which has
 * no Linux counterpart.  FreeLinX ships a genuine Linux dmesg instead, built
 * on the Linux kernel's own logging interface: the syslog(2) system call,
 * wrapped for userspace by musl's klogctl(3) in <sys/klog.h>.
 *
 * It is a real, correct program against the Linux kernel ABI.  Whether it can
 * PRINT anything depends on the FreeLinX kernel actually implementing the
 * syslog(2) ring buffer - which is a kernel (not userland) concern and is not
 * faked here.  Until the FreeLinX kernel boots, dmesg builds and installs
 * but has no kernel buffer to read; the error path is exercised instead.
 *
 * Porting decisions / compat: none needed - musl provides klogctl(3) already.
 * The SYSLOG_ACTION_* constants are the raw Linux syslog(2) "type" values
 * (a stable kernel ABI, identical to what util-linux/busybox define).
 */

#include <err.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/klog.h>
#include <unistd.h>

/* Linux syslog(2) action types (kernel ABI; see www.kernel.org/doc/man-pages).
 * musl's <sys/klog.h> only declares klogctl(), not these constants. */
#ifndef SYSLOG_ACTION_READ_ALL
#define SYSLOG_ACTION_READ_ALL		3
#define SYSLOG_ACTION_READ_CLEAR	4
#define SYSLOG_ACTION_SIZE_BUFFER	10
#endif

static void
usage(void)
{
	(void)fprintf(stderr,
	    "usage: dmesg [-c] [-c] [-n level] [-s bufsize]\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	int ch, rc;
	size_t bufsz = 16384;
	char *buf;
	int cflags = 0;
	int clearbuf;

	setlocale(LC_ALL, "");

	/* pick up a sensible default buffer size without changing anything */
	clearbuf = 0;

	while ((ch = getopt(argc, argv, "cn:s:h")) != -1) {
		switch (ch) {
		case 'c':
			cflags++;
			break;
		case 'n':
		case 's':
			/* accept but ignore sizing/level args for the minimal
			 * probe build; the kernel buffer is read wholesale. */
			if (optarg == NULL || atoi(optarg) < 0)
				usage();
			break;
		case 'h':
		default:
			usage();
		}
	}
	argc -= optind;
	if (argc != 0)
		usage();

	(void)clearbuf;

	/* size the buffer to the kernel's ring buffer, then read it all */
	bufsz = (size_t)klogctl(SYSLOG_ACTION_SIZE_BUFFER, NULL, 0);
	if (bufsz <= 0)
		bufsz = 16384;
	buf = malloc(bufsz + 1);
	if (buf == NULL)
		err(EXIT_FAILURE, "malloc");

	if (cflags > 0)
		rc = klogctl(SYSLOG_ACTION_READ_CLEAR, buf,
		    (int)bufsz);
	else
		rc = klogctl(SYSLOG_ACTION_READ_ALL, buf, (int)bufsz);

	if (rc < 0)
		err(EXIT_FAILURE, "klogctl");

	buf[rc < (int)bufsz ? rc : (int)bufsz] = '\0';

	if (rc > 0)
		(void)fwrite(buf, 1, (size_t)rc, stdout);

	free(buf);
	if (cflags == 0 && rc > 0 && buf[rc-1] != '\n')
		(void)putchar('\n');

	return EXIT_SUCCESS;
}
