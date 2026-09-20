# FreeLinX/ports - base/pwait : NetBSD 10.1 `/usr/bin/pwait`.
NAME   := pwait
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pwait
INSTALL_RELPATH:= bin/pwait

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/pwait/pwait.c

PORT_SRCS = usr.bin/pwait/pwait.c

# FreeLinX compat:
#  - kqueue(2)/kevent(2): NetBSD 10 pwait is kqueue/EVPFILT_PROC based
#    (see base/compat/kqueue.c - epoll+signalfd emulation).
#  - getprogname(3): musl shim in base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/kqueue.c $(FLX_COMPAT)/getprogname.c
