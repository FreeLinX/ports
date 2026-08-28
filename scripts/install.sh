#!/bin/sh
# install.sh - stage a built port and (optionally) copy it into the
# FreeLinX/src rootfs as /bin/<name>.
#
# A port must be built before it can be installed; this script simply copies
# the already-built binary into the staging tree, then (with -r) into the
# FreeLinX/src rootfs. It never compiles, so it never claims a success that
# did not happen.

set -eu
. "$(dirname "$0")/common.sh"

INSTALL_TO_ROOTFS=0
while [ "$#" -gt 0 ]; do
    case "$1" in
        -r) INSTALL_TO_ROOTFS=1; shift ;;
        -h|--help) flx_usage; exit 0 ;;
        -*) flx_die "unknown option: $1 (see -h)" ;;
        *) break ;;
    esac
done

flx_load_config
flx_setup_dirs

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

[ "$#" -gt 0 ] || flx_die "usage: $(basename "$0") [-r] PORT [PORT...]"

for p in "$@"; do
    _dir=$(flx_port_dir "$p") || flx_die "no such port: $p"
    [ -f "$_dir/Makefile" ] || flx_die "$p: no Makefile"
    # Derive the staging layout from the port's own metadata so it always
    # matches mk/install.mk (staging/<triple>/<category>/<name>/bin/<binname>).
    _cat=$(sed -n 's/^CATEGORY[[:space:]]*[:?]\?=[[:space:]]*//p' "$_dir/Makefile" 2>/dev/null | head -n1 | tr -d ' ')
    _name=$(sed -n 's/^NAME[[:space:]]*[:?]\?=[[:space:]]*//p' "$_dir/Makefile" 2>/dev/null | head -n1 | tr -d ' ')
    _bi=$(sed -n 's/^INSTALL_BIN[[:space:]]*[:?]\?=[[:space:]]*//p' "$_dir/Makefile" 2>/dev/null | head -n1 | tr -d ' ')
    _cat=${_cat:-base}
    _name=${_name:-$p}
    _bin=sh
    case "$_name" in *-sh) _bin=sh ;; *) _bin=$_name ;; esac
    _bi=${_bi:-$_bin}
    _stage="$FREELINX_STAGING_DIR/$FREELINX_TRIPLE/$_cat/$_name/bin/$_bi"
    if [ ! -f "$_stage" ]; then
        flx_die "$p: no staged binary at $_stage (build it first)"
    fi

    flx_info "Installing $p to staging..."
    flx_info "  staged binary: $_stage"

    if [ "$INSTALL_TO_ROOTFS" -eq 1 ]; then
        flx_info "  copying into FreeLinX/src rootfs -> $FREELINX_ROOTFS_BIN/$_bi"
        mkdir -p "$FREELINX_ROOTFS_BIN"
        cp -f "$_stage" "$FREELINX_ROOTFS_BIN/$_bi"
    fi
done
flx_info "install finished"
