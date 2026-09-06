#ifndef	_CDBR_H
#define	_CDBR_H

#include <sys/cdefs.h>
#if defined(_KERNEL) || defined(_STANDALONE)
#include <sys/types.h>
#else
#include <inttypes.h>
#include <stddef.h>
#endif

#define	CDBR_DEFAULT	0

struct cdbr;

__BEGIN_DECLS

#if !defined(_KERNEL) && !defined(_STANDALONE)
struct cdbr	*cdbr_open(const char *, int);
#endif
struct cdbr	*cdbr_open_mem(void *, size_t, int,
    void (*)(void *, void *, size_t), void *);
uint32_t	 cdbr_entries(struct cdbr *);
int		 cdbr_get(struct cdbr *, uint32_t, const void **, size_t *);
int		 cdbr_find(struct cdbr *, const void *, size_t,
    const void **, size_t *);
void		 cdbr_close(struct cdbr *);

__END_DECLS

#endif /* _CDBR_H */
