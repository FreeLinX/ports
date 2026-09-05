/*
 * FreeLinX/ports - base/compat/uuid.h : minimal NetBSD <uuid.h>.
 *
 * usr.bin/uuidgen needs uuidgen(3) (bulk UUID v4 generation) and
 * uuid_to_string(3).  musl provides neither; UUID v4 data is drawn
 * from getrandom(2).
 */

#ifndef _FREELINX_COMPAT_UUID_H_
#define _FREELINX_COMPAT_UUID_H_

#include <sys/types.h>

typedef unsigned char	uuid_t[16];

/* status codes (uuid_s_*) kept shareable with NetBSD */
#define	uuid_s_ok		0
#define	uuid_s_bad_version	1
#define	uuid_s_invalid_string_uuid 2
#define	uuid_s_no_memory	3

int	uuidgen(uuid_t *, int);
void	uuid_to_string(const uuid_t *, char **, uint32_t *);
int	uuid_compare(const uuid_t *, const uuid_t *, uint32_t *);
int	uuid_create(uuid_t *, uint32_t *);
int	uuid_create_nil(uuid_t *, uint32_t *);
int	uuid_from_string(const char *, uuid_t *, uint32_t *);
int	uuid_enc_be(void *, const uuid_t *);

#endif /* !_FREELINX_COMPAT_UUID_H_ */