# FreeLinX/ports - base/audiocfg : NetBSD 10.1 `/usr/bin/audiocfg`.
NAME   := audiocfg
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= audiocfg
INSTALL_RELPATH:= bin/audiocfg

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/audiocfg/audiodev.c \
	usr/src/usr.bin/audiocfg/audiodev.h \
	usr/src/usr.bin/audiocfg/drvctl.c \
	usr/src/usr.bin/audiocfg/drvctl.h \
	usr/src/usr.bin/audiocfg/dtmf.c \
	usr/src/usr.bin/audiocfg/dtmf.h \
	usr/src/usr.bin/audiocfg/main.c

PORT_SRCS = usr.bin/audiocfg/audiodev.c \
	usr.bin/audiocfg/drvctl.c \
	usr.bin/audiocfg/dtmf.c \
	usr.bin/audiocfg/main.c
