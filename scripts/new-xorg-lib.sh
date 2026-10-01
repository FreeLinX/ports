#!/bin/sh
# FreeLinX/ports - scripts/new-xorg-lib.sh CATEGORY/NAME VERSION [DEPS] [EXTRA]
#
# Write a port that is one of the X.Org autotools libraries, using
# mk/xorg-lib.mk for the recipe.  Prints the path it wrote.
#
# NAME is the port name, lib<Something>, and it is also the upstream project's
# name, so the distfile is dist/NAME-VERSION.tar.xz and it comes from
# xorg.freedesktop.org's own archive.  DEPS defaults to libX11, which every one
# of them needs; name the others, space separated.  EXTRA, if given, becomes
# XORG_LIB_LIBS.
#
# The distfile must already be in dist/ and the dependencies must be ports that
# already exist -- this writes a port, it does not check the tree.
set -eu

_c=${1:?usage: new-xorg-lib.sh CATEGORY/NAME VERSION [DEPS] [EXTRA]}
_v=${2:?usage: new-xorg-lib.sh CATEGORY/NAME VERSION [DEPS] [EXTRA]}
_deps=${3:-libX11}
_extra=${4:-}

_cat=${_c%%/*}
_name=${_c#*/}
_dir="$_cat/$_name"
_tgz="dist/$_name-$_v.tar.xz"
_url="https://xorg.freedesktop.org/archive/individual/lib/$_name-$_v.tar.xz"

[ -f mk/xorg-lib.mk ] || { printf 'run me from the ports root\n' >&2; exit 1; }
[ -f "$_tgz" ] || { printf 'no %s; fetch it first\n' "$_tgz" >&2; exit 1; }
[ ! -e "$_dir/Makefile" ] || { printf '%s already exists\n' "$_dir" >&2; exit 1; }

_sha=$(sha256sum "$_tgz" | cut -d' ' -f1)

mkdir -p "$_dir"
cat > "$_dir/Makefile" <<EOF
# FreeLinX/ports - $_cat/$_name : $_name $_v.
#
# Real upstream source:
#   $_url
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := $_name
VERSION := $_v
CATEGORY:= $_cat
LICENSE := MIT (X11)
DEPENDENCIES:= $_deps
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := \$(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := $_deps
EOF
[ -n "$_extra" ] && printf 'XORG_LIB_LIBS := %s\n' "$_extra" >> "$_dir/Makefile"
cat >> "$_dir/Makefile" <<'EOF'

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

include ../../mk/xorg-lib.mk
EOF

cat > "$_dir/distinfo" <<EOF
# FreeLinX/ports - $_cat/$_name : $_name $_v.
#
# Real upstream source:
#   $_url
DISTINFO_NAME=$_name-$_v
DISTINFO_ARCHIVE=$_name-$_v.tar.xz
DISTINFO_URL=$_url
DISTINFO_SHA256=$_sha
EOF

printf '%s\n' "$_dir"
