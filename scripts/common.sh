#!/bin/sh
# common.sh - shared helpers for the FreeLinX/ports scripts.
#
# Intended to be sourced by the other scripts in this directory:
#
#     . "$(dirname "$0")/common.sh"
#
# All scripts here are POSIX /bin/sh. No bash-only syntax. Environment
# variables (FREELINX_*) always override configuration files.

# ---------------------------------------------------------------------------
# Repository root: $0's parent one level up.
# ---------------------------------------------------------------------------
if [ -z "${FREELINX_ROOT:-}" ]; then
    FREELINX_ROOT=$(CDPATH='' cd -P "$(dirname "$0")/.." && pwd)
fi

# Defaults applied before the config file; the config re-applies with the
# ${VAR:-default} idiom so a pre-set environment variable wins.
FREELINX_ARCH=${FREELINX_ARCH:-x86_64}
FREELINX_CONFIG=${FREELINX_CONFIG:-"${FREELINX_ROOT}/config/default.conf"}

# ---------------------------------------------------------------------------
# Messaging helpers (matching FreeLinX/src style, tagged /ports).
# ---------------------------------------------------------------------------
flx_info() { printf '[FreeLinX/ports] %s\n' "$*"; }
flx_warn() { printf '[FreeLinX/ports][warning] %s\n' "$*" >&2; }
flx_die()  { printf '[FreeLinX/ports][error] %s\n' "$*" >&2; exit 1; }

have_cmd() { command -v "$1" >/dev/null 2>&1; }

flx_usage() {
    cat <<EOF
FreeLinX/ports: BSD-style ports framework for FreeLinX.

Usage: $(basename "$0") [OPTIONS] [PORT...]

Options:
  -a ARCH           target architecture (default: \$FREELINX_ARCH or x86_64)
  -c CONFIG         path to config file (default: config/default.conf)
  -t TRIPLE         target triple (default: \$FREELINX_TRIPLE or x86_64-linux-musl)
  -h                show this help

A PORT argument names a port by "category/name" (e.g. base/cat) or by name
(e.g. netbsd-sh). With no PORT argument, all ports are selected.
EOF
}

# ---------------------------------------------------------------------------
# Configuration loading.
# ---------------------------------------------------------------------------
flx_load_config() {
    if [ ! -f "$FREELINX_CONFIG" ]; then
        flx_die "configuration not found: $FREELINX_CONFIG (set FREELINX_CONFIG)"
    fi
    # shellcheck disable=SC1090
    . "$FREELINX_CONFIG"
}

# ---------------------------------------------------------------------------
# Toolchain detection.
#
# Reports 0 and sets these variables when a usable FreeLinX toolchain is
# present (a real directory/executable must exist -- we never claim otherwise):
#   FLX_CC, FLX_LD, FLX_SYSROOT, FLX_TOOLCHAIN_OK=1   (or FLX_TOOLCHAIN_OK=0)
# ---------------------------------------------------------------------------
flx_detect_toolchain() {
    FLX_TOOLCHAIN_OK=0
    FLX_CC=${FREELINX_CC}
    FLX_LD=${FREELINX_LD}
    FLX_SYSROOT=${FREELINX_SYSROOT}

    if [ -n "${FREELINX_CC}" ] && [ -x "${FREELINX_CC}" ]; then
        flx_info "compiler: ${FREELINX_CC}"
    else
        flx_warn "compiler not found: ${FREELINX_CC}"
        return 1
    fi
    if [ -n "${FREELINX_LD}" ] && [ -x "${FREELINX_LD}" ]; then
        flx_info "linker:   ${FREELINX_LD}"
    else
        flx_warn "linker not found: ${FREELINX_LD}"
        return 1
    fi
    if [ -n "${FREELINX_SYSROOT}" ] && [ -d "${FREELINX_SYSROOT}" ]; then
        flx_info "sysroot:  ${FREELINX_SYSROOT}"
    else
        flx_warn "sysroot not found: ${FREELINX_SYSROOT}"
        return 1
    fi
    FLX_TOOLCHAIN_OK=1
    return 0
}

# ---------------------------------------------------------------------------
# Directory setup.
# ---------------------------------------------------------------------------
flx_setup_dirs() {
    mkdir -p "${FREELINX_DIST_DIR}" 2>/dev/null || true
    mkdir -p "${FREELINX_STAGING_DIR}" 2>/dev/null || true
    mkdir -p "${FREELINX_BUILD_DIR}" 2>/dev/null || true
}
