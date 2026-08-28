#!/bin/sh
# clean.sh - remove generated artifacts (build/, staging/, dist/).
# Never touches the live system or the FreeLinX/src rootfs.

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

flx_info "Cleaning generated artifacts..."
for d in "$FREELINX_BUILD_DIR" "$FREELINX_STAGING_DIR" "$FREELINX_DIST_DIR"; do
    if [ -d "$d" ]; then
        rm -rf "$d"
        flx_info "  removed $d"
    fi
done
flx_info "clean finished"
