# FreeLinX/ports - base/units : NetBSD 10.1 `/usr/bin/units`.
NAME   := units
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= units
INSTALL_RELPATH:= bin/units

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/units/pathnames.h \
	usr/src/usr.bin/units/units.c \
	usr/src/usr.bin/units/units.lib

PORT_SRCS = usr.bin/units/units.c

# units(1) needs its data file ($_PATH_UNITSLIB = /usr/share/misc/units.lib).
# The generic mk/install.mk rule only stages INSTALL_BIN; add the data file
# the same way sysutils/runit stages its extra binaries.
UNITS_LIB = $(SRC_DIR)/usr.bin/units/units.lib

install: $(STAGE_ROOT)/usr/share/misc/units.lib

$(STAGE_ROOT)/usr/share/misc/units.lib: $(UNITS_LIB)
	@printf '[FreeLinX/ports] staging %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 644 $< $@

install-rootfs: $(ROOTFS_DIR)/usr/share/misc/units.lib

$(ROOTFS_DIR)/usr/share/misc/units.lib: $(STAGE_ROOT)/usr/share/misc/units.lib check-install-rootfs
	@printf '[FreeLinX/ports] installing %s into rootfs\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 644 $< $@
