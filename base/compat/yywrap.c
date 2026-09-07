/* FreeLinX/ports - base/compat/yywrap.c : flex scanners without -lfl.
 *
 * flex-generated scanners call yywrap() at EOF unless "%option noyywrap" is
 * set; NetBSD sources rely on the host libfl.  musl has none, so provide the
 * classic terminating stub here (return 1 = "no more input").
 */
int
yywrap(void)
{
	return 1;
}