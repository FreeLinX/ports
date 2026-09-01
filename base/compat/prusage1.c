/* FreeLinX/ports - base/compat : prusage1(3) for musl, used by base/time.
 *
 * NetBSD's usr.bin/time links prusage1() from bin/csh/time.c, but that file
 * drags in the whole csh.h universe (varent, Char, STRtime, short2str ...)
 * behind #ifndef NOT_CSH and a csh/ prefix structure that does not survive
 * being shared.  So the single function usr.bin/time actually needs is
 * re-homed here, with the two static helpers (pdeltat/pcsecs) and the %P
 * "strpct()" formatting inlined (musl has no strpct(3)).
 *
 * The format verbs are the GNU `/usr/bin/time -v` subset NetBSD supports:
 * %U %S %E %P %X %D %K %M %t %I %O %F %W %C %e %w %c %r %s %k %R.
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <time.h>

static void
pdeltat(FILE *fp, int prec, struct timeval *t1, struct timeval *t0)
{
	struct timeval td;

	timersub(t1, t0, &td);
	(void)fprintf(fp, "%ld.%0*ld", (long)td.tv_sec,
	    prec, (long)(td.tv_usec / 100000));
}

#define	P2DIG(fp, i)	(void)fprintf(fp, "%ld%ld", (i) / 10, (i) % 10)

static void
pcsecs(FILE *fp, long l)	/* print mm:ss.dd, l is in sec*100 */
{
	long i;

	i = l / 360000;
	if (i) {
		(void)fprintf(fp, "%ld:", i);
		i = (l % 360000) / 100;
		P2DIG(fp, i / 60);
		goto minsec;
	}
	i = l / 100;
	(void)fprintf(fp, "%ld", i / 60);
minsec:
	i %= 60;
	(void)fputc(':', fp);
	P2DIG(fp, i);
	(void)fputc('.', fp);
	P2DIG(fp, (l % 100));
}

void
prusage1(FILE *fp, const char *cp, int prec,
    struct rusage *r0, struct rusage *r1,
    struct timespec *e, struct timespec *b)
{
	long i;
	time_t t;
	time_t ms;

	ms = (e->tv_sec - b->tv_sec) * 100 + (e->tv_nsec - b->tv_nsec) / 10000000;
	t = (r1->ru_utime.tv_sec - r0->ru_utime.tv_sec) * 100 +
	    (r1->ru_utime.tv_usec - r0->ru_utime.tv_usec) / 10000 +
	    (r1->ru_stime.tv_sec - r0->ru_stime.tv_sec) * 100 +
	    (r1->ru_stime.tv_usec - r0->ru_stime.tv_usec) / 10000;

	for (; *cp; cp++)
		if (*cp != '%')
			(void) fputc(*cp, fp);
		else if (cp[1])
			switch (*++cp) {
			case 'D':	/* (average) unshared data size */
				(void)fprintf(fp, "%ld", t == 0 ? 0L :
				    (long)((r1->ru_idrss + r1->ru_isrss -
				     (r0->ru_idrss + r0->ru_isrss)) / t));
				break;
			case 'E':	/* elapsed (wall-clock) time */
				pcsecs(fp, (long) ms);
				break;
			case 'F':	/* page faults */
				(void)fprintf(fp, "%ld", r1->ru_majflt - r0->ru_majflt);
				break;
			case 'I':	/* FS blocks in */
				(void)fprintf(fp, "%ld", r1->ru_inblock - r0->ru_inblock);
				break;
			case 'K':	/* (average) total data memory used */
				(void)fprintf(fp, "%ld", t == 0 ? 0L :
				    (long)(((r1->ru_ixrss + r1->ru_isrss + r1->ru_idrss) -
				     (r0->ru_ixrss + r0->ru_idrss + r0->ru_isrss)) / t));
				break;
			case 'M':	/* max. Resident Set Size */
				(void)fprintf(fp, "%ld", r1->ru_maxrss);
				break;
			case 'O':	/* FS blocks out */
				(void)fprintf(fp, "%ld", r1->ru_oublock - r0->ru_oublock);
				break;
			case 'P':	/* percent time spent running */
				if (ms == 0) {
					(void)fputs("0.0%", fp);
				} else {
					(void)fprintf(fp, "%.*f%%", prec,
					    (double)t * 100.0 / (double)ms);
				}
				break;
			case 'R':	/* page reclaims */
				(void)fprintf(fp, "%ld", r1->ru_minflt - r0->ru_minflt);
				break;
			case 'S':	/* system CPU time used */
				pdeltat(fp, prec, &r1->ru_stime, &r0->ru_stime);
				break;
			case 'U':	/* user CPU time used */
				pdeltat(fp, prec, &r1->ru_utime, &r0->ru_utime);
				break;
			case 'W':	/* number of swaps */
				i = r1->ru_nswap - r0->ru_nswap;
				(void)fprintf(fp, "%ld", i);
				break;
			case 'X':	/* (average) shared text size */
				(void)fprintf(fp, "%ld", t == 0 ? 0L :
				    (long)((r1->ru_ixrss - r0->ru_ixrss) / t));
				break;
			case 'c':	/* num. involuntary context switches */
				(void)fprintf(fp, "%ld", r1->ru_nivcsw - r0->ru_nivcsw);
				break;
			case 'k':	/* number of signals received */
				(void)fprintf(fp, "%ld", r1->ru_nsignals - r0->ru_nsignals);
				break;
			case 'r':	/* socket messages received */
				(void)fprintf(fp, "%ld", r1->ru_msgrcv - r0->ru_msgrcv);
				break;
			case 's':	/* socket messages sent */
				(void)fprintf(fp, "%ld", r1->ru_msgsnd - r0->ru_msgsnd);
				break;
			case 'w':	/* num. voluntary context switches */
				(void)fprintf(fp, "%ld", r1->ru_nvcsw - r0->ru_nvcsw);
				break;
			}
	(void)fputc('\n', fp);
}