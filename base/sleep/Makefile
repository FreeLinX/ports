# FreeLinX/ports - base/sleep : NetBSD 10.1 `/bin/sleep`.
#
# Builds the ACTUAL NetBSD sleep utility source (bin/sleep/sleep.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes (patches/patch-sleep-musl):
#   - `__unused` attribute dropped (base/compat <sys/cdefs.h> deliberately
#     defines no __unused macro; musl's <sys/stat.h> uses it as a member name).
#   - strtod_l(..., LC_C_LOCALE) fallback removed (musl has no LC_C_LOCALE;
#     plain strtod already parses the C-locale radix '.', which is the only
#     locale FreeLinX ships).
#   - the BSD SIGINFO (Ctrl-T) progress handler is guarded with #ifdef SIGINFO
#     (Linux has no SIGINFO).
#   - timespecadd()/timespecsub() come from base/compat <sys/cdefs.h>.
#   - floor() lives in musl libm, so this port links -lm (the base recipe
#     links only -lc by default; see FLX_LD below).
NAME   := sleep
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/sleep -> src/rootfs/bin/sleep.
INSTALL_BIN:= sleep
INSTALL_RELPATH:= bin/sleep

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/sleep/sleep.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/sleep/sleep.c

# sleep uses floor(3) from musl libm; the default base link line has no -lm.
FLX_LD = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
		-nostdlib -L$(FREELINX_SYSROOT)/lib \
		$(FLX_CRT) \
		$(OBJ_DIR)/*.o \
		-lc -lm $(FLX_COMPILER_RT) \
		$(FLX_CRT_END)
