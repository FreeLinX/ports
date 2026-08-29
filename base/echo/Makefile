# FreeLinX/ports - base/echo : NetBSD 10.1 `/bin/echo`.
#
# Builds the ACTUAL NetBSD cat utility source (bin/echo/echo.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# It is not an invented echo or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - none required: echo.c only needs err/warn and the FreeLinX musl-compat
#     getprogname(3) shim for its setprogname() call.
NAME   := echo
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/echo -> src/rootfs/bin/echo.  Must be set before
# mk/base-port.mk is included (rule-target names freeze at parse time).
INSTALL_BIN:= echo
INSTALL_RELPATH:= bin/echo

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# src set members for bin/echo (usr/src/bin/echo/...; usr/src/ stripped).
NETBSD_MEMBERS = usr/src/bin/echo/echo.c
PORT_SRCS = bin/echo/echo.c