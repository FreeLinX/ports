/*
 * FreeLinX/ports - base/compat/nbsys : pthread_types.h
 *
 * WHY THIS FILE EXISTS
 *
 * NetBSD's <sys/types.h> ends with:
 *
 *	#if !defined(_KERNEL) && !defined(_STANDALONE)
 *	#if (_POSIX_C_SOURCE - 0L) >= 199506L || (_XOPEN_SOURCE - 0) >= 500 || \
 *	    defined(_NETBSD_SOURCE)
 *	#include <pthread_types.h>
 *	#endif
 *	#endif
 *
 * so any NetBSD source compiled with _XOPEN_SOURCE >= 500 - or with
 * _NETBSD_SOURCE, which this build defines for most ports - reaches for
 * <pthread_types.h> before it has included <pthread.h> for anything else.
 * musl has no such header: it declares every type in it from <pthread.h>
 * directly, with no separate opaque-types file, because the opaque structs are
 * an internal implementation detail of NetBSD's libpthread that user code has
 * no reason to know about.
 *
 * Without this file the port dies before it compiles a line of its own code:
 *
 *	.../nbsys/sys/sys/types.h:369:10: fatal error: 'pthread_types.h' file not found
 *
 * The mapping is exact.  NetBSD's pthread_types.h declares typedef *names* -
 *
 *	pthread_t, pthread_attr_t, pthread_mutex_t, pthread_mutexattr_t,
 *	pthread_cond_t, pthread_condattr_t, pthread_once_t, pthread_spinlock_t,
 *	pthread_spin_t, pthread_rwlock_t, pthread_rwlockattr_t,
 *	pthread_barrier_t, pthread_barrierattr_t, pthread_key_t
 *
 * - and leaves the structs incomplete for user code; musl's <pthread.h>
 * declares the same set of names as complete types.  A typedef name is all a
 * user of the header can spell, so making the struct visible is a superset of
 * what the original provided, not a different thing.
 *
 * The one type NetBSD's file also has is __pthread_spin_t, the internal
 * spelling of pthread_spin_t.  It is a libpthread-private name; a port that
 * reaches for it is reaching past the library's interface, and it is not
 * aliased here.
 *
 * There is no <pthread.h> in this overlay to conflict with, so including
 * musl's is unambiguous.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 */

#ifndef _FLX_NBSYS_PTHREAD_TYPES_H_
#define _FLX_NBSYS_PTHREAD_TYPES_H_

/* musl's own header guard keeps a second inclusion harmless. */
#include <pthread.h>

#endif /* !_FLX_NBSYS_PTHREAD_TYPES_H_ */
