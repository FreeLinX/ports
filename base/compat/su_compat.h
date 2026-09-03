/* musl compat: declare BSD functions not in musl libc */
#ifndef _FREELINX_SU_COMPAT_H
#define _FREELINX_SU_COMPAT_H
#include <string.h>
int consttime_memequal(const void *, const void *, size_t);
size_t estrlcpy(char *, const char *, size_t);
char *estrdup(const char *);
void *emalloc(size_t);
#ifndef _PASSWORD_WARNDAYS
#define _PASSWORD_WARNDAYS 14
#endif
#ifndef _PASSWORD_MAXLEN
#define _PASSWORD_MAXLEN 128
#endif
#endif
