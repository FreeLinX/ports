/*
 * FreeLinX/ports - base/compat/sys/sysvipc.h : SysV IPC info structs.
 *
 * NetBSD ipcrm(1)/ipcs(1) reflect the kernel SysV IPC tables through
 * sysctl(3) KERN_SYSVIPC queries.  The FreeLinX sysctl shim answers
 * those queries from /proc/sysvipc with the NetBSD 10 structures:
 * a *_sysctl_info header (info block + flexible array of per-object
 * *_ds_sysctl records) filled contiguously.
 */

#ifndef _FREELINX_COMPAT_SYS_SYSVIPC_H_
#define _FREELINX_COMPAT_SYS_SYSVIPC_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	SEM_ALLOC	0x8000		/* semid in use */

/* Reconstruct the real kernel-assigned ipcid from a table record.
 * The FreeLinX sysctl shim stores it in the top 16 bits of mode;
 * ipcs' fmt_perm only decodes the low 9 bits. */
#define	IXSEQ_TO_IPCID(ix, perm)	\
	(((unsigned int)(perm).mode >> 16) & 0xffff)

/* struct msginfo/seminfo/shminfo come from musl's <sys/msg.h>,
 * <sys/sem.h>, <sys/shm.h> and match the NetBSD member names ipcs
 * uses. */

struct msgid_ds_sysctl {
	struct ipc_perm	msg_perm;	/* operation permission struct */
	u_short		msg_seq;	/* message sequence number */
	time_t		msg_stime;	/* last msgsnd time */
	time_t		msg_rtime;	/* last msgrcv time */
	time_t		msg_ctime;	/* last change time */
	size_t		_msg_cbytes;	/* current number of bytes on queue */
	msgqnum_t	msg_qnum;	/* number of messages in the queue */
	msglen_t	msg_qbytes;	/* max number of bytes on queue */
	pid_t		msg_lspid;	/* pid of last msgsnd */
	pid_t		msg_lrpid;	/* pid of last msgrcv */
};

struct msg_sysctl_info {
	struct msginfo		msginfo;
	struct msgid_ds_sysctl	msgids[];
};

struct semid_ds_sysctl {
	struct ipc_perm	sem_perm;	/* operation permission struct */
	u_short		sem_seq;	/* semaphore sequence number */
	time_t		sem_otime;	/* last semop time */
	time_t		sem_ctime;	/* last change time */
	int		sem_nsems;	/* number of semaphores in set */
};

struct sem_sysctl_info {
	struct seminfo		seminfo;
	struct semid_ds_sysctl	semids[];
};

struct shmid_ds_sysctl {
	struct ipc_perm	shm_perm;	/* operation permission struct */
	u_short		shm_seq;	/* slot sequence number */
	time_t		shm_atime;	/* last attach time */
	time_t		shm_dtime;	/* last detach time */
	time_t		shm_ctime;	/* last change time */
	size_t		shm_segsz;	/* size of segment in bytes */
	pid_t		shm_cpid;	/* pid of creator */
	pid_t		shm_lpid;	/* pid of last shmat/shmdt */
	int		shm_nattch;	/* number of current attaches */
};

struct shm_sysctl_info {
	struct shminfo		shminfo;
	struct shmid_ds_sysctl	shmids[];
};

#endif /* !_FREELINX_COMPAT_SYS_SYSVIPC_H_ */