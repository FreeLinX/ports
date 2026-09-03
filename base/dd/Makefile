# FreeLinX/ports - base/dd : NetBSD 10.1 `/bin/dd`.
#
# Disk dump utility.  NetBSD source set, patched for musl/Linux via sed
# during build (SIGINFO, DEFFILEMODE, ioctl cast).
NAME   := dd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= dd
INSTALL_RELPATH:= bin/dd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/dd/dd.c usr/src/bin/dd/dd.h usr/src/bin/dd/args.c \
		usr/src/bin/dd/conv.c usr/src/bin/dd/conv_tab.c usr/src/bin/dd/misc.c \
		usr/src/bin/dd/position.c usr/src/bin/dd/dd_hostops.c usr/src/bin/dd/dd_swab.c \
		usr/src/bin/dd/extern.h
PORT_SRCS = bin/dd/dd.c bin/dd/args.c bin/dd/conv.c bin/dd/conv_tab.c \
	    bin/dd/misc.c bin/dd/position.c bin/dd/dd_hostops.c bin/dd/dd_swab.c
FLX_CPPFLAGS = -I$(FLX_COMPAT) -I$(SRC_DIR) -DHAVE_NBTOOL_CONFIG_H=0

# Apply musl/Linux fixups before compiling (files already extracted by do-prepare)
$(BUILD_BIN): do-prepare
	@set -e; \
	cd $(SRC_DIR) && \
	printf '[FreeLinX/ports] applying Linux fixups to dd\n' && \
	sed -i '/#include <sys\/param.h>/a\#ifndef SIGINFO\n#define SIGINFO SIGUSR1\n#endif\n\n#ifndef DEFFILEMODE\n#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)\n#endif' bin/dd/dd.c && \
	sed -i 's/\.op_ioctl = ioctl,/.op_ioctl = (int (*)(int, unsigned long, ...))ioctl,/g' bin/dd/dd.c bin/dd/dd_hostops.c; \
	mkdir -p "$(OBJ_DIR)"; \
	for s in $(ALL_SRCS); do \
		o="$(OBJ_DIR)/$$(printf '%s' "$$s" | sed 's|/|_|g')"; o="$${o%.c}.o"; \
		printf '[FreeLinX/ports] cc %s\n' "$${s##*/}"; \
		$(CC) $(FREELINX_CFLAGS) $(FLX_CPPFLAGS) -c -o "$$o" "$$s"; \
	done; \
	printf '[FreeLinX/ports] ld %s\n' "$@"; \
	$(FLX_LD) -o "$@"
