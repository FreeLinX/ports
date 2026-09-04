# FreeLinX/ports - base/awk : Brian Kernighan's One True Awk.
#
# The ACTUAL one-true-awk (BWK awk) from https://github.com/onetrueawk/awk,
# corresponding to "The AWK Programming Language, Second Edition" (2024).
# Built with the FreeLinX clang + LLD + musl toolchain, fully static.
#
# One-true-awk ships a simple makefile that compiles ~10 C files and runs
# yacc/bison on awkgram.y to produce ytab.c/ytab.h.  Rather than driving its
# makefile, this port compiles the translation units directly (the same
# approach used for the NetBSD base utilities) so the FreeLinX toolchain
# controls every flag.
#
# Build host requirement: bison or yacc (to generate ytab.c/ytab.h from
# awkgram.y if not already present in the source tree).
#
# No external dependency beyond musl.  No GNU runtime.

NAME   := awk
VERSION:= 2024
CATEGORY:= base
LICENSE:= MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), bison (host)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= awk
INSTALL_RELPATH:= usr/bin/awk

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/awk-master
PROJECT_BIN = $(SRC_TREE)/a.out

# One-true-awk is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# The FreeLinX toolchain's llvm-ar/llvm-ranlib require GLIBC_2.38 (host has
# 2.36) and cannot run here.  Not needed for awk (no archive step), but
# keep consistent with the other ports.
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

# One-true-awk source files (after bison/maketab generate the rest).
AWK_SRCS = awkgram.tab.c b.c lex.c lib.c main.c parse.c proctab.c run.c tran.c

# Generate awkgram.tab.c/h from awkgram.y (bison default naming), then build
# `maketab` with the HOST compiler and run it to emit proctab.c.
PROJECT_CFG_CMDS = \
	if [ ! -f awkgram.tab.c ]; then \
		bison -d awkgram.y; \
	fi && \
	if [ ! -f proctab.c ]; then \
		cc -g maketab.c -o maketab && \
		./maketab awkgram.tab.h > proctab.c; \
	fi

BUILD_CMDS = \
	for s in $(AWK_SRCS); do \
		printf '[FreeLinX/ports] cc %s\n' "$$s"; \
		$(CC) $(CFLAGS) -c -o "$${s%.c}.o" "$$s" || exit 1; \
	done && \
	printf '[FreeLinX/ports] ld awk\n' && \
	$(CC) $(LDFLAGS) -o a.out $(AWK_SRCS:.c=.o) -lm
