#!/bin/sh
# install.sh - stage a built port and (optionally) copy it into the
# FreeLinX/src rootfs template.
#
# Staging model: the <staging> tree is a rootfs-compatible overlay. A port
# stages its built artifact at staging/$(INSTALL_RELPATH) (e.g. staging/bin/sh).
# With -r, the same relpath is copied into the src rootfs (src/rootfs/bin/sh).
#
# The port's own install target owns staging and its build prerequisites.  This
# wrapper invokes that target first, then (with -r) mirrors the staged output
# into the FreeLinX/src rootfs.

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

[ "$#" -gt 0 ] || flx_die "usage: $(basename "$0") [-r] PORT [PORT...]"

# Read a single value out of a port Makefile, stripping the assignment prefix.
flx_port_var() {
    sed -n "s/^$1[[:space:]]*[:?]\?=[[:space:]]*//p" "$2/Makefile" 2>/dev/null \
        | head -n1 | tr -d ' ' | tr -d '"'
}

flx_port_dir() {
    case "$1" in
        */*) printf '%s/%s/%s\n' "$FREELINX_ROOT" "${1%%/*}" "${1##*/}" ;;
        *)
            for _cat in base shells net firmware; do
                [ -d "$FREELINX_ROOT/$_cat/$1" ] && { printf '%s/%s/%s\n' "$FREELINX_ROOT" "$_cat" "$1"; return 0; }
            done
            return 1
            ;;
    esac
}

for p in "$@"; do
    _dir=$(flx_port_dir "$p") || flx_die "no such port: $p"
    [ -f "$_dir/Makefile" ] || flx_die "$p: no Makefile"

    flx_info "Staging $p..."
    ( cd "$_dir" && make FREELINX_ROOT="$FREELINX_ROOT" install ) || \
        flx_die "$p: install failed (see above)"

    # Resolve the port's own staging metadata so it matches mk/install.mk.
    _relpath=$(flx_port_var INSTALL_RELPATH "$_dir")
    _name=$(flx_port_var NAME "$_dir"); _name=${_name:-$p}
    if [ -z "$_relpath" ]; then
        _inst_bin=$(flx_port_var INSTALL_BIN "$_dir")
        if [ -z "$_inst_bin" ]; then
            case "$_name" in *-sh) _inst_bin=sh ;; *) _inst_bin=$_name ;; esac
        fi
        _relpath="bin/$_inst_bin"
    fi

    _stage="$FREELINX_STAGING_ROOT/$_relpath"
    _rootfs="$FREELINX_ROOTFS_DIR/$_relpath"
    if [ ! -e "$_stage" ]; then
        flx_die "$p: install target did not create expected staged output at $_stage"
    fi

    flx_info "Installing $p to staging overlay: $_stage"

    if [ "$INSTALL_TO_ROOTFS" -eq 1 ] && [ ! -d "$_stage" ]; then
        # Validate the configured rootfs destination once, before any copy.
        _rootfs_base=$(flx_validate_rootfs_dir "$FREELINX_ROOTFS_DIR") \
            || exit 1
        _rootfs="$_rootfs_base/$_relpath"
        flx_info "  copying into FreeLinX/src rootfs -> $_rootfs"
        mkdir -p "$(dirname "$_rootfs")"
        cp -f "$_stage" "$_rootfs"
    fi

    # Optional per-port front-ends/aliases (space-separated names, e.g.
    # "chgrp").  Each alias is the same built binary copied under another
    # name, so argv[0]-driven behavior (chown vs chgrp) works.  Requires the
    # port's INSTALL_BIN to be a single binary.
    _aliases=$(sed -n "s/^INSTALL_ALIASES[[:space:]]*[:?]\?=[[:space:]]*//p" \
        "$_dir/Makefile" 2>/dev/null | head -n1)
    for _alias in $_aliases; do
        _astage="$FREELINX_STAGING_ROOT/bin/$_alias"
        flx_info "  aliasing $p as $_alias -> $_astage"
        cp -f "$_stage" "$_astage"
        if [ "$INSTALL_TO_ROOTFS" -eq 1 ]; then
            _arootfs="$_rootfs_base/bin/$_alias"
            mkdir -p "$(dirname "$_arootfs")"
            cp -f "$_astage" "$_arootfs"
            flx_info "  copying $_alias into FreeLinX/src rootfs -> $_arootfs"
        fi
    done
done
flx_info "install finished"
