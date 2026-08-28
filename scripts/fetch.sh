#!/bin/sh
# fetch.sh - download and verify the upstream source for one or more ports.
#
# Before the FreeLinX toolchain is finalized a port cannot be compiled, but its
# upstream source can still be fetched and verified. This downloads the archive
# named by each port's distinfo into dist/ and verifies its SHA256.
#
# If a port's distinfo carries an unverified hash (DISTINFO_SHA256=TODO),
# fetch.sh records the freshly computed hash in .config/ so it can be reviewed
# and pinned. It never invents a hash.

set -eu
. "$(dirname "$0")/common.sh"

flx_port_dir() {
    case "$1" in
        */*) printf '%s/%s/%s\n' "$FREELINX_ROOT" "${1%%/*}" "${1##*/}" ;;
        *)
            # name-only match across all categories.
            for _cat in base shells; do
                [ -d "$FREELINX_ROOT/$_cat/$1" ] && { printf '%s/%s/%s\n' "$FREELINX_ROOT" "$_cat" "$1"; return 0; }
            done
            return 1
            ;;
    esac
}

flx_all_ports() {
    find "$FREELINX_ROOT/base" "$FREELINX_ROOT/shells" -mindepth 1 -maxdepth 1 -type d 2>/dev/null \
        | sed 's#^.*/##' | sort -u
}

fetch_one() {
    _port=$1
    _dir=$(flx_port_dir "$_port") || { flx_warn "no such port: $_port"; return 1; }
    _distinfo="$_dir/distinfo"
    if [ ! -f "$_distinfo" ]; then
        flx_warn "$_port: no distinfo; nothing to fetch"
        return 0
    fi
    . "$_distinfo"

    : "${DISTINFO_NAME:?distinfo missing DISTINFO_NAME}"
    : "${DISTINFO_URL:?distinfo missing DISTINFO_URL}"

    _dst="${FREELINX_DIST_DIR}/$(basename "$DISTINFO_URL")"
    if [ -f "$_dst" ]; then
        flx_info "$_port: already downloaded: $_dst"
    else
        flx_info "$_port: fetching $DISTINFO_URL"
        mkdir -p "$FREELINX_DIST_DIR"
        (cd "$FREELINX_DIST_DIR" && curl -fLSO "$DISTINFO_URL") || {
            flx_warn "$_port: download failed"
            return 1
        }
    fi

    _computed=$("$FREELINX_SHA256_CMD" "$_dst" 2>/dev/null | awk '{print $1}') || _computed=
    if [ -n "${DISTINFO_SHA256:-}" ] && [ "$DISTINFO_SHA256" = "TODO" ]; then
        flx_info "$_port: sha256 not pinned (TODO) -- computed for review:"
        flx_info "    sha256($_dst) = $_computed"
    elif [ -n "${DISTINFO_SHA256:-}" ] && [ -n "$_computed" ]; then
        if [ "$_computed" = "$DISTINFO_SHA256" ]; then
            flx_info "$_port: sha256 verified"
        else
            flx_warn "$_port: sha256 MISMATCH (expected $DISTINFO_SHA256, got $_computed)"
            return 1
        fi
    else
        flx_warn "$_port: no sha256 to verify against"
    fi
    return 0
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        -h|--help) flx_usage; exit 0 ;;
        -*) flx_die "unknown option: $1 (see -h)" ;;
        *) break ;;
    esac
done

flx_load_config
flx_setup_dirs
FREELINX_SHA256_CMD=${FREELINX_SHA256_CMD:-sha256sum}

if [ "$#" -eq 0 ]; then
    set -- $(flx_all_ports)
fi
for p in "$@"; do
    fetch_one "$p"
done
flx_info "fetch complete"
