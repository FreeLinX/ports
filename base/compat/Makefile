# FreeLinX/ports - base/compat : no-port container.
#
# base/compat is NOT a port: it holds the FreeLinX compat layer (headers under
# sys/, plus small FreeLinX-written or vendored NetBSD libc objects) that the
# real ports compile into themselves via their COMPAT_SRCS.  It exists here so
# the all-ports iterators (make build/check/install) only see a build stub.
NAME   := compat
CATEGORY:= base
VERSION:= freelinx

all:
	@printf '[FreeLinX/ports] compat: library-only dir, nothing to build\n'

check:
	@printf '[FreeLinX/ports] compat: library-only dir, nothing to check\n'

.PHONY: all check