# FreeLinX/ports - net/freelinx-route : BSD-styled route for Linux.
#
# A small, BSD-flavoured route written from scratch FOR FreeLinX. It talks to
# the Linux kernel over AF_NETLINK (NETLINK_ROUTE) via libnl-3, which is the
# interface the Linux kernel actually provides (there is no BSD AF_ROUTE on
# Linux, so the NetBSD netbsd-route source cannot link without a rewrite).
#
# SOURCE: our own C source lives in this port's directory (route.c); there is
# nothing to fetch. It is built ONLY with the FreeLinX clang + LLD + musl
# toolchain, statically, against the libnl-3 that FreeLinX builds itself
# (net/libnl). Zero GNU tools in the system or in the build.
#
# GNU-free static link: clang + -static -fuse-ld=lld --rtlib=compiler-rt
# (musl has no crtbegin/crtend/-lgcc; compiler-rt is clang's own runtime and
# LLD is our linker; no GNU ld/ld.bfd/libgcc anywhere).
#
# License: BSD-2-Clause (FreeLinX original code).

NAME   := freelinx-route
VERSION:= 1.0
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:= libnl (net/libnl)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= flx-route
INSTALL_RELPATH:= sbin/flx-route

include ../../mk/port.mk

# libnl dependency prefix (built by net/libnl, linked statically).
FLX_LIBNL_PREFIX = $(FREELINX_BUILD_DIR)/deps/libnl
FLX_LIBNL_INC    = -I$(FLX_LIBNL_PREFIX)/include/libnl3
FLX_LIBNL_LIBS   = -L$(FLX_LIBNL_PREFIX)/lib -lnl-route-3 -lnl-3

# In-port source.
SRC        := $(CURDIR)/route.c
BUILD_DIR  := $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN  := $(BUILD_DIR)/flx-route

$(BUILD_DIR):
	@$(MKDIR) -p $(BUILD_DIR)

$(BUILD_BIN): $(SRC) | $(BUILD_DIR)
	@set -e; \
	printf '[FreeLinX/ports] cc %s\n' "$(notdir $(SRC))"; \
	$(CC) $(FREELINX_CFLAGS) $(FLX_LIBNL_INC) -O2 -c -o "$(BUILD_DIR)/route.o" "$(SRC)"; \
	printf '[FreeLinX/ports] ld %s\n' "$(notdir $(BUILD_BIN))"; \
	$(CC) $(FREELINX_LDFLAGS) -static --rtlib=compiler-rt -o "$(BUILD_BIN)" \
	    "$(BUILD_DIR)/route.o" $(FLX_LIBNL_LIBS)

.PHONY: do-build
do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] built: %s\n' "$(BUILD_BIN)"
