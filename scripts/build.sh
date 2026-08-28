#!/bin/sh
# build.sh - build one or more FreeLinX ports against the FreeLinX toolchain.
#
# Flow: load config -> check toolchain -> for each port, run make in the
# port's directory (the port Makefile owns the compile+link recipe). The
# build never claims success if the toolchain is unavailable or a step
# fails: it reports the exact blocker and exits non-zero.

set -eu
. "$(dirname "$0")/common.sh"

flx_port_dir() {
    case "$1" in
        */*) printf '%s/%s/%s\n' "$FREELINX_ROOT" "${1%%/*}" "${1##*/}" ;;
        *)
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

while [ "$#" -gt 0 ]; do
    case "$1" in
        -h|--help) flx_usage; exit 0 ;;
        -*) flx_die "unknown option: $1 (see -h)" ;;
        *) break ;;
    esac
done

flx_load_config
flx_setup_dirs

flx_info "Checking toolchain..."
if flx_detect_toolchain; then
    flx_info "toolchain available"
else
    flx_die "toolchain is not available: $FREELINX_CC / $FREELINX_LD / $FREELINX_SYSROOT. \
Bootstrap FreeLinX/toolchain first (or set FREELINX_CC/FREELINX_LD/FREELINX_SYSROOT). \
No build was attempted."
fi

if [ "$#" -eq 0 ]; then
    flx_info "no PORT given; building all ports"
    set -- $(flx_all_ports)
fi

for p in "$@"; do
    _dir=$(flx_port_dir "$p") || flx_die "no such port: $p"
    flx_info "Building $p..."
    if [ ! -f "$_dir/Makefile" ]; then
        flx_die "$p: no Makefile; nothing to build"
    fi
    ( cd "$_dir" && make FREELINX_ROOT="$FREELINX_ROOT" all ) || \
        flx_die "$p: build failed (see above)"
    flx_info "$p: build complete"
done
flx_info "build finished"
