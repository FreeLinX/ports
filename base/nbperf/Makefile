# FreeLinX/ports - base/nbperf : NetBSD 10.1 `/usr/bin/nbperf`.
NAME   := nbperf
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= nbperf
INSTALL_RELPATH:= bin/nbperf

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/nbperf/graph2.c \
	usr/src/usr.bin/nbperf/graph2.h \
	usr/src/usr.bin/nbperf/graph3.c \
	usr/src/usr.bin/nbperf/nbperf-bdz.c \
	usr/src/usr.bin/nbperf/nbperf-chm.c \
	usr/src/usr.bin/nbperf/nbperf-chm3.c \
	usr/src/usr.bin/nbperf/nbperf.c \
	usr/src/usr.bin/nbperf/nbperf.h

PORT_SRCS = usr.bin/nbperf/graph2.c \
	usr.bin/nbperf/graph3.c \
	usr.bin/nbperf/nbperf-bdz.c \
	usr.bin/nbperf/nbperf-chm.c \
	usr.bin/nbperf/nbperf-chm3.c \
	usr.bin/nbperf/nbperf.c
