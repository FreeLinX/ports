/* FreeLinX/ports - base/compat : musl <sys/queue.h> stand-in.
 *
 * WHY THIS FILE EXISTS (patches/README, "musl compat"):
 *
 * musl deliberately does not ship a <sys/queue.h> (the BSD intrusive
 * singly/tail/doubly-linked list macro family), but NetBSD userland sources
 * include it.  usr.bin/grep's output-queue (queue.c, the -m/-B/-A/-C ring
 * buffer) needs only the STAILQ (singly-linked tail queue) subset.  This is
 * a minimal FreeLinX-provided stand-in defining exactly those macros, in the
 * same spirit as the existing <sys/cdefs.h> and <sys/stat.h> compat shims.
 *
 * The macros below are the conventional BSD definitions (the queue facility
 * is public-domain trivia of the BSDs); only the STAILQ members grep uses
 * are provided.  This is NOT a copy of NetBSD's sys/queue.h wholesale — the
 * full set lives in the separate NetBSD "syssrc" set, not in the src set in
 * dist/, so it cannot be lifted verbatim.  Keep this minimal.
 */
#ifndef _FREELINX_COMPAT_SYS_QUEUE_H_
#define _FREELINX_COMPAT_SYS_QUEUE_H_

#define	STAILQ_HEAD(name, type)					\
struct name {							\
	struct type *stqh_first;	/* first element */	\
	struct type **stqh_last;	/* addr of last next */	\
}

#define	STAILQ_HEAD_INITIALIZER(head)				\
	{ NULL, &(head).stqh_first }

#define	STAILQ_ENTRY(type)					\
struct {							\
	struct type *stqe_next;	/* next element */		\
}

#define	STAILQ_FIRST(head)	((head)->stqh_first)

#define	STAILQ_INSERT_TAIL(head, elm, field) do {		\
	(elm)->field.stqe_next = NULL;				\
	*(head)->stqh_last = (elm);				\
	(head)->stqh_last = &(elm)->field.stqe_next;		\
} while (/*CONSTCOND*/0)

#define	STAILQ_REMOVE_HEAD(head, field) do {			\
	(head)->stqh_first = (head)->stqh_first->field.stqe_next; \
} while (/*CONSTCOND*/0)

#endif /* !_FREELINX_COMPAT_SYS_QUEUE_H_ */
