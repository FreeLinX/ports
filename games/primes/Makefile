# FreeLinX/ports - games/primes : NetBSD 10.1 prime number generator.
#
# Generate or test prime numbers.  BSD license.
# Multi-file port; no external library dependencies.

NAME   := primes
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= primes
INSTALL_RELPATH:= usr/games/primes

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/primes

NETBSD_MEMBERS = usr/src/games/primes/pattern.c usr/src/games/primes/pr_tbl.c \
	usr/src/games/primes/primes.c usr/src/games/primes/primes.h \
	usr/src/games/primes/spsp.c

PORT_SRCS = games/primes/pattern.c games/primes/pr_tbl.c \
	games/primes/primes.c games/primes/spsp.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c
