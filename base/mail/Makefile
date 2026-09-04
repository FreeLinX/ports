# FreeLinX/ports - base/mail : NetBSD 10.1 `/usr/bin/mail`.
NAME   := mail
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mail
INSTALL_RELPATH:= bin/mail

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mail/cmd1.c \
	usr/src/usr.bin/mail/cmd2.c \
	usr/src/usr.bin/mail/cmd3.c \
	usr/src/usr.bin/mail/cmd4.c \
	usr/src/usr.bin/mail/cmdtab.c \
	usr/src/usr.bin/mail/collect.c \
	usr/src/usr.bin/mail/complete.c \
	usr/src/usr.bin/mail/complete.h \
	usr/src/usr.bin/mail/def.h \
	usr/src/usr.bin/mail/dotlock.c \
	usr/src/usr.bin/mail/edit.c \
	usr/src/usr.bin/mail/extern.h \
	usr/src/usr.bin/mail/fio.c \
	usr/src/usr.bin/mail/format.c \
	usr/src/usr.bin/mail/format.h \
	usr/src/usr.bin/mail/getname.c \
	usr/src/usr.bin/mail/glob.h \
	usr/src/usr.bin/mail/head.c \
	usr/src/usr.bin/mail/lex.c \
	usr/src/usr.bin/mail/list.c \
	usr/src/usr.bin/mail/main.c \
	usr/src/usr.bin/mail/mime.h \
	usr/src/usr.bin/mail/mime_attach.c \
	usr/src/usr.bin/mail/mime_attach.h \
	usr/src/usr.bin/mail/mime_child.c \
	usr/src/usr.bin/mail/mime_child.h \
	usr/src/usr.bin/mail/mime_codecs.c \
	usr/src/usr.bin/mail/mime_codecs.h \
	usr/src/usr.bin/mail/mime_decode.c \
	usr/src/usr.bin/mail/mime_decode.h \
	usr/src/usr.bin/mail/mime_detach.c \
	usr/src/usr.bin/mail/mime_detach.h \
	usr/src/usr.bin/mail/mime_header.c \
	usr/src/usr.bin/mail/mime_header.h \
	usr/src/usr.bin/mail/names.c \
	usr/src/usr.bin/mail/pathnames.h \
	usr/src/usr.bin/mail/popen.c \
	usr/src/usr.bin/mail/quit.c \
	usr/src/usr.bin/mail/rcv.h \
	usr/src/usr.bin/mail/send.c \
	usr/src/usr.bin/mail/sig.c \
	usr/src/usr.bin/mail/sig.h \
	usr/src/usr.bin/mail/strings.c \
	usr/src/usr.bin/mail/support.c \
	usr/src/usr.bin/mail/temp.c \
	usr/src/usr.bin/mail/thread.c \
	usr/src/usr.bin/mail/thread.h \
	usr/src/usr.bin/mail/tty.c \
	usr/src/usr.bin/mail/v7.local.c \
	usr/src/usr.bin/mail/vars.c \
	usr/src/usr.bin/mail/version.c

PORT_SRCS = usr.bin/mail/cmd1.c \
	usr.bin/mail/cmd2.c \
	usr.bin/mail/cmd3.c \
	usr.bin/mail/cmd4.c \
	usr.bin/mail/cmdtab.c \
	usr.bin/mail/collect.c \
	usr.bin/mail/complete.c \
	usr.bin/mail/dotlock.c \
	usr.bin/mail/edit.c \
	usr.bin/mail/fio.c \
	usr.bin/mail/format.c \
	usr.bin/mail/getname.c \
	usr.bin/mail/head.c \
	usr.bin/mail/lex.c \
	usr.bin/mail/list.c \
	usr.bin/mail/main.c \
	usr.bin/mail/mime_attach.c \
	usr.bin/mail/mime_child.c \
	usr.bin/mail/mime_codecs.c \
	usr.bin/mail/mime_decode.c \
	usr.bin/mail/mime_detach.c \
	usr.bin/mail/mime_header.c \
	usr.bin/mail/names.c \
	usr.bin/mail/popen.c \
	usr.bin/mail/quit.c \
	usr.bin/mail/send.c \
	usr.bin/mail/sig.c \
	usr.bin/mail/strings.c \
	usr.bin/mail/support.c \
	usr.bin/mail/temp.c \
	usr.bin/mail/thread.c \
	usr.bin/mail/tty.c \
	usr.bin/mail/v7.local.c \
	usr.bin/mail/vars.c \
	usr.bin/mail/version.c
COMPAT_SRCS += $(FLX_COMPAT)/vis.c

COMPAT_SRCS += $(FLX_COMPAT)/erealloc.c

COMPAT_SRCS += $(FLX_COMPAT)/easprintf.c
