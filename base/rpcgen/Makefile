# FreeLinX/ports - base/rpcgen : NetBSD 10.1 `/usr/bin/rpcgen`.
NAME   := rpcgen
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rpcgen
INSTALL_RELPATH:= bin/rpcgen

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rpcgen/rpc_clntout.c \
	usr/src/usr.bin/rpcgen/rpc_cout.c \
	usr/src/usr.bin/rpcgen/rpc_hout.c \
	usr/src/usr.bin/rpcgen/rpc_main.c \
	usr/src/usr.bin/rpcgen/rpc_parse.c \
	usr/src/usr.bin/rpcgen/rpc_parse.h \
	usr/src/usr.bin/rpcgen/rpc_sample.c \
	usr/src/usr.bin/rpcgen/rpc_scan.c \
	usr/src/usr.bin/rpcgen/rpc_scan.h \
	usr/src/usr.bin/rpcgen/rpc_svcout.c \
	usr/src/usr.bin/rpcgen/rpc_tblout.c \
	usr/src/usr.bin/rpcgen/rpc_util.c \
	usr/src/usr.bin/rpcgen/rpc_util.h

PORT_SRCS = usr.bin/rpcgen/rpc_clntout.c \
	usr.bin/rpcgen/rpc_cout.c \
	usr.bin/rpcgen/rpc_hout.c \
	usr.bin/rpcgen/rpc_main.c \
	usr.bin/rpcgen/rpc_parse.c \
	usr.bin/rpcgen/rpc_sample.c \
	usr.bin/rpcgen/rpc_scan.c \
	usr.bin/rpcgen/rpc_svcout.c \
	usr.bin/rpcgen/rpc_tblout.c \
	usr.bin/rpcgen/rpc_util.c
