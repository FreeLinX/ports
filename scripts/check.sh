#!/bin/sh
# check.sh - read-only diagnostics: configuration, toolchain availability,
# port inventory. Does not modify anything and does not fake results.

set -eu
. "$(dirname "$0")/common.sh"

while [ "$#" -gt 0 ]; do
    case "$1" in
        -h|--help) flx_usage; exit 0 ;;
        -*) flx_die "unknown option: $1 (see -h)" ;;
        *) break ;;
    esac
done

flx_load_config

flx_info "Configuration: $FREELINX_CONFIG"
flx_info "  arch:   $FREELINX_ARCH"
flx_info "  triple: $FREELINX_TRIPLE"
flx_info "  prefix: $FREELINX_PREFIX"
flx_info "  link:   $FREELINX_LINK_MODE"
flx_info "  toolchain dir: $FREELINX_TOOLCHAIN_DIR"
flx_info "  sysroot:       $FREELINX_SYSROOT"

flx_info ""
flx_info "Toolchain:"
if flx_detect_toolchain; then
    flx_info "  state: available"
else
    flx_info "  state: NOT available (toolchain not yet bootstrapped or FREELINX_* not set)"
fi

flx_info ""
flx_info "Ports:"
flx_all_ports() {
    find "$FREELINX_ROOT/base" "$FREELINX_ROOT/shells" -mindepth 1 -maxdepth 1 -type d 2>/dev/null \
        | sed 's#^.*/##' | sort -u
}
for _p in $(flx_all_ports); do
    _d=""
    for _cat in base shells; do
        [ -d "$FREELINX_ROOT/$_cat/$_p" ] && _d="$FREELINX_ROOT/$_cat/$_p" && break
    done
    _v="?"
    if [ -f "$_d/Makefile" ]; then
        _v=$(sed -n 's/^VERSION[[:space:]]*[:?]\?=[[:space:]]*//p' "$_d/Makefile" 2>/dev/null | head -n1 | tr -d ' ')
        [ -z "$_v" ] && _v='?'
    fi
    [ -n "$_d" ] && flx_info "  $_p ($_v)"
done

flx_info ""
flx_info "check complete (no changes made)"
