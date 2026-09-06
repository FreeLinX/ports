/* compiled_terms.c - FreeLinX: embedded terminfo fallback list.
 * Upstream generates this via `tic -Sx <db>` from genterms (embeds
 * ansi/dumb/vt100/vt220/wsvt25/xterm).  Regenerate later once tic builds;
 * an empty table keeps the reader working with on-disk databases only. */
struct compiled_term {
	const char *name;
	const char *cap;
	size_t caplen;
};
static const struct compiled_term compiled_terms[0] = {};
