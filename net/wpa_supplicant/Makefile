# FreeLinX/ports - net/wpa_supplicant : wpa_supplicant 2.11 (WiFi supplicant).
#
# Upstream wpa_supplicant (Linux net/wireless supplicant) built statically with
# the FreeLinX clang + LLD + musl toolchain. Connects to WPA2/WPA3 networks via
# cfg80211/nl80211. Depends on libnl-genl/libnl-3 (net/libnl) and static musl
# OpenSSL (base/openssl) for crypto. BSD-2-Clause.
#
# wpa_supplicant has no autoconf configure; it reads a generated `.config` and
# drives its own Makefile. project-port.mk exports CC/CFLAGS/LDFLAGS (target +
# sysroot + -static + -fuse-ld=lld + --rtlib=compiler-rt) into the child env, so
# the .config probes and the final link use the FreeLinX toolchain. We write a
# minimal .config selecting the nl80211 driver (no legacy WEXT), TLS=openssl,
# and add -I/-L paths to our static musl libnl + openssl in build/deps.
# (wpa_supplicant's own CONFIG_LIBNL32/CONFIG_TLS=openssl add the -lnl-*/-lcrypto
# flags; we only supply the search dirs.)

NAME   := wpa_supplicant
VERSION:= 2.11
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:= libnl openssl
BUILD_DEPENDENCIES:= musl, libnl (net/libnl), OpenSSL (base/openssl)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= wpa_supplicant
INSTALL_RELPATH:= bin/wpa_supplicant

# Static musl deps from net/libnl and base/openssl.
LIBnl_PREFIX  = $(FREELINX_BUILD_DIR)/deps/libnl
OSSL_PREFIX   = $(FREELINX_BUILD_DIR)/deps/openssl

# wpa_supplicant is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_LDFLAGS  =

# Link-only switches in the exported CFLAGS too: wpa_supplicant's probes invoke
# $(CC) $(CFLAGS) ... and clang's default link would fall back to host ld.bfd
# against the musl sysroot.
FLX_PROJECT_CFLAGS := -O2 --rtlib=compiler-rt -fuse-ld=lld -static \
	-I$(LIBnl_PREFIX)/include/libnl3 -I$(OSSL_PREFIX)/include

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/wpa_supplicant/wpa_supplicant
BUILD_BIN   = $(PROJECT_BIN)

# Minimal nl80211-only .config pulling in static musl libnl + openssl. The -I
# and -L dirs let wpa_supplicant's own -lnl-3/-lnl-genl-3/-lcrypto resolve.
PROJECT_CFG_CMDS = \
	cd wpa_supplicant && \
	{ \
	  echo 'CONFIG_DRIVER_NL80211=y'; \
	  echo 'CONFIG_LIBNL32=y'; \
	  echo 'CONFIG_CTRL_IFACE=y'; \
	  echo 'CONFIG_CTRL_IFACE_UNIX=y'; \
	  echo 'CONFIG_BACKEND=file'; \
	  echo 'CONFIG_NO_RANDOM_POOL=y'; \
	  echo 'CONFIG_TLS=openssl'; \
	  echo 'CFLAGS += -I$(LIBnl_PREFIX)/include/libnl3 -I$(OSSL_PREFIX)/include'; \
	  echo 'LIBS += -L$(LIBnl_PREFIX)/lib -L$(OSSL_PREFIX)/lib64'; \
	} > .config

BUILD_CMDS = \
	cd wpa_supplicant && \
	make -j"$(FREELINX_JOBS)" wpa_supplicant
