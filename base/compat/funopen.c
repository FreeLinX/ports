/* FreeLinX/ports - base/compat/funopen.c : BSD funopen(3) on fopencookie(3).
 *
 * NetBSD's compress (zopen.c) uses funopen(3): create a FILE * over user
 * read/write/seek/close callbacks.  musl provides fopencookie(3) with a
 * nearly identical contract, so funopen(3) is a thin adapter: hold the BSD
 * callbacks + user cookie in a small heap block and route the fopencookie
 * cookie functions through them.  Pipe the desired open mode from which
 * callbacks are present (matches the BSD use pattern in zopen.c).
 */
/* fopencookie(3)/cookie_io_functions_t live under _GNU_SOURCE in musl's
 * <stdio.h>; declare it before pulling the header in. */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef int (funopen_readfn)(void *, char *, int);
typedef int (funopen_writefn)(void *, const char *, int);
typedef off_t (funopen_seekfn)(void *, off_t, int);
typedef int (funopen_closefn)(void *);

struct funopen_ctx {
	funopen_readfn	*readfn;
	funopen_writefn	*writefn;
	funopen_seekfn	*seekfn;
	funopen_closefn	*closefn;
	void		*usercookie;
};

static ssize_t
_cb_read(void *opaque, char *buf, size_t len)
{
	struct funopen_ctx *ctx = opaque;

	if (ctx->readfn == NULL)
		return -1;
	return (ssize_t)ctx->readfn(ctx->usercookie, buf, (int)len);
}

static ssize_t
_cb_write(void *opaque, const char *buf, size_t len)
{
	struct funopen_ctx *ctx = opaque;

	if (ctx->writefn == NULL)
		return -1;
	return (ssize_t)ctx->writefn(ctx->usercookie, buf, (int)len);
}

static int
_cb_seek(void *opaque, off_t *offset, int whence)
{
	struct funopen_ctx *ctx = opaque;
	off_t newpos;

	if (ctx->seekfn == NULL)
		return -1;
	newpos = ctx->seekfn(ctx->usercookie, (off_t)*offset, whence);
	if (newpos < 0)
		return -1;
	*offset = newpos;
	return 0;
}

static int
_cb_close(void *opaque)
{
	struct funopen_ctx *ctx = opaque;
	int rv = 0;

	if (ctx->closefn != NULL)
		rv = ctx->closefn(ctx->usercookie);
	free(ctx);
	return rv;
}

FILE *
funopen(const void *cookie, funopen_readfn *readfn, funopen_writefn *writefn,
    funopen_seekfn *seekfn, funopen_closefn *closefn)
{
	struct funopen_ctx *ctx;
	cookie_io_functions_t io = { _cb_read, _cb_write, _cb_seek, _cb_close };
	const char *mode = "r+";

	ctx = (struct funopen_ctx *)malloc(sizeof(*ctx));
	if (ctx == NULL)
		return NULL;
	ctx->readfn = readfn;
	ctx->writefn = writefn;
	ctx->seekfn = seekfn;
	ctx->closefn = closefn;
	ctx->usercookie = (void *)cookie;

	if (readfn != NULL && writefn == NULL)
		mode = "r";
	else if (writefn != NULL && readfn == NULL)
		mode = "w";

	return fopencookie(ctx, mode, io);
}