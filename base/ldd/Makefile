# FreeLinX/ports - base/ldd : NetBSD 10.1 `/usr/bin/ldd`.
NAME   := ldd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ldd
INSTALL_RELPATH:= bin/ldd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ldd/dummy.c \
	usr/src/usr.bin/ldd/ldd.c \
	usr/src/usr.bin/ldd/ldd.h \
	usr/src/usr.bin/ldd/ldd_elfxx.c \
	usr/src/libexec/ld.elf_so/debug.h \
	usr/src/libexec/ld.elf_so/rtld.h \
	usr/src/libexec/ld.elf_so/rtldenv.h \
	usr/src/libexec/ld.elf_so/hash.h \
	usr/src/libexec/ld.elf_so/sysident.h

# ldd.c does #include <debug.h> and <rtld.h>; NetBSD keeps the runtime-loader
# headers in the ld.elf_so directory rather than with the ldd sources, so they
# have to be pulled in separately and that directory added to the include path.
FLX_CPPFLAGS += -I$(SRC_DIR)/libexec/ld.elf_so

PORT_SRCS = usr.bin/ldd/dummy.c \
	usr.bin/ldd/ldd.c \
	usr.bin/ldd/ldd_elfxx.c

# Not portable to Linux: see the reason below. Declared rather than left
# to fail - an expected failure should not be reported as a regression, and
# a port that cannot work here should say so instead of looking broken.
PORT_NOT_PORTABLE:= NetBSD's runtime dynamic loader. ldd.c includes <rtld.h> and pokes
