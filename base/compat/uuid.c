/*
 * FreeLinX/ports - base/compat/uuid.c : UUID support for musl.
 *
 * RFC 4122 version 4 UUIDs generated from getrandom(2); string
 * encode/decode and comparison helpers for usr.bin/uuidgen.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/random.h>
#include <uuid.h>

int
uuidgen(uuid_t *store, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		unsigned char b[16];

		if (getrandom(b, sizeof(b), 0) != (ssize_t)sizeof(b))
			return -1;
		b[6] = (b[6] & 0x0f) | 0x40;	/* version 4 */
		b[8] = (b[8] & 0x3f) | 0x80;	/* RFC 4122 variant */
		memcpy(store[i], b, 16);
	}
	return 0;
}

void
uuid_to_string(const uuid_t *u, char **s, uint32_t *status)
{
	char *p = malloc(37);

	if (p == NULL) {
		if (status != NULL)
			*status = uuid_s_no_memory;
		return;
	}
	snprintf(p, 37,
	    "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-"
	    "%02x%02x%02x%02x%02x%02x",
	    (*u)[0], (*u)[1], (*u)[2], (*u)[3],
	    (*u)[4], (*u)[5], (*u)[6], (*u)[7],
	    (*u)[8], (*u)[9], (*u)[10], (*u)[11],
	    (*u)[12], (*u)[13], (*u)[14], (*u)[15]);
	*s = p;
	if (status != NULL)
		*status = uuid_s_ok;
}

int
uuid_compare(const uuid_t *a, const uuid_t *b, uint32_t *status)
{
	*status = uuid_s_ok;
	return memcmp(*a, *b, 16);
}

int
uuid_create(uuid_t *u, uint32_t *status)
{
	if (uuidgen(u, 1) != 0) {
		if (status != NULL)
			*status = uuid_s_no_memory;
		return -1;
	}
	if (status != NULL)
		*status = uuid_s_ok;
	return 0;
}

int
uuid_create_nil(uuid_t *u, uint32_t *status)
{
	memset(*u, 0, 16);
	if (status != NULL)
		*status = uuid_s_ok;
	return 0;
}

int
uuid_enc_be(void *buf, const uuid_t *u)
{
	memcpy(buf, *u, 16);
	return 0;
}

int
uuid_from_string(const char *s, uuid_t *u, uint32_t *status)
{
	unsigned int v[16];
	int i;

	if (sscanf(s,
	    "%2x%2x%2x%2x-%2x%2x-%2x%2x-%2x%2x-%2x%2x%2x%2x%2x%2x",
	    &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7],
	    &v[8], &v[9], &v[10], &v[11], &v[12], &v[13], &v[14],
	    &v[15]) != 16) {
		if (status != NULL)
			*status = uuid_s_invalid_string_uuid;
		return -1;
	}
	for (i = 0; i < 16; i++)
		(*u)[i] = (unsigned char)v[i];
	if (status != NULL)
		*status = uuid_s_ok;
	return 0;
}