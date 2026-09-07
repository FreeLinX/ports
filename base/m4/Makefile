# FreeLinX/ports - base/m4 : NetBSD 10.1 `/usr/bin/m4`.
NAME   := m4
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= m4
INSTALL_RELPATH:= bin/m4

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/m4/eval.c \
	usr/src/usr.bin/m4/expr.c \
	usr/src/usr.bin/m4/extern.h \
	usr/src/usr.bin/m4/gnum4.c \
	usr/src/usr.bin/m4/look.c \
	usr/src/usr.bin/m4/main.c \
	usr/src/usr.bin/m4/mdef.h \
	usr/src/usr.bin/m4/misc.c \
	usr/src/usr.bin/m4/pathnames.h \
	usr/src/usr.bin/m4/stdd.h \
	usr/src/usr.bin/m4/trace.c \
	usr/src/usr.bin/m4/parser.y \
	usr/src/usr.bin/m4/tokenizer.l \
	usr/src/usr.bin/m4/lib/ohash.h \
	usr/src/usr.bin/m4/lib/ohash_int.h \
	usr/src/usr.bin/m4/lib/ohash_create_entry.c \
	usr/src/usr.bin/m4/lib/ohash_delete.c \
	usr/src/usr.bin/m4/lib/ohash_do.c \
	usr/src/usr.bin/m4/lib/ohash_entries.c \
	usr/src/usr.bin/m4/lib/ohash_enum.c \
	usr/src/usr.bin/m4/lib/ohash_init.c \
	usr/src/usr.bin/m4/lib/ohash_interval.c \
	usr/src/usr.bin/m4/lib/ohash_lookup_interval.c \
	usr/src/usr.bin/m4/lib/ohash_lookup_memory.c \
	usr/src/usr.bin/m4/lib/ohash_qlookup.c \
	usr/src/usr.bin/m4/lib/ohash_qlookupi.c

PORT_SRCS = usr.bin/m4/eval.c \
	usr.bin/m4/expr.c \
	usr.bin/m4/gnum4.c \
	usr.bin/m4/look.c \
	usr.bin/m4/main.c \
	usr.bin/m4/misc.c \
	usr.bin/m4/trace.c \
	usr.bin/m4/parser.c \
	usr.bin/m4/tokenizer.c \
	usr.bin/m4/lib/ohash_create_entry.c \
	usr.bin/m4/lib/ohash_delete.c \
	usr.bin/m4/lib/ohash_do.c \
	usr.bin/m4/lib/ohash_entries.c \
	usr.bin/m4/lib/ohash_enum.c \
	usr.bin/m4/lib/ohash_init.c \
	usr.bin/m4/lib/ohash_interval.c \
	usr.bin/m4/lib/ohash_lookup_interval.c \
	usr.bin/m4/lib/ohash_lookup_memory.c \
	usr.bin/m4/lib/ohash_qlookup.c \
	usr.bin/m4/lib/ohash_qlookupi.c

# The bundled ohash lives in the extracted usr.bin/m4/lib/ tree next to
# ohash.h (which look.c includes as <ohash.h>).
FLX_CPPFLAGS += -I$(SRC_DIR)/usr.bin/m4/lib
COMPAT_SRCS += $(FLX_COMPAT)/strtoi.c
COMPAT_SRCS += $(FLX_COMPAT)/yywrap.c

# m4's parser is byacc/flex generated (parser.y -> parser.c, tokenizer.l ->
# tokenizer.c); run the host tools during do-prepare before compiling.
FLX_PREGEN = cd $(SRC_DIR)/usr.bin/m4 && bison -y -d parser.y && mv y.tab.c parser.c && mv y.tab.h parser.h && flex -o tokenizer.c tokenizer.l
