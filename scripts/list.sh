#!/bin/sh
# list.sh - list available ports with a metadata summary.
# Read-only; used by `make list`.

set -eu
. "$(dirname "$0")/common.sh"

flx_load_config

printf '%-14s %-14s %-10s %s\n' "PORT" "CATEGORY" "VERSION" "STATUS"
printf '%s\n' "-----------------------------------------------"

flx_all_ports() {
    find "$FREELINX_ROOT/base" "$FREELINX_ROOT/shells" -mindepth 1 -maxdepth 1 -type d 2>/dev/null \
        | sed 's#^.*/##' | sort -u
}

for _p in $(flx_all_ports); do
    _d=""
    _catname=""
    for _cat in base shells; do
        [ -d "$FREELINX_ROOT/$_cat/$_p" ] && _d="$FREELINX_ROOT/$_cat/$_p" && _catname="$_cat" && break
    done
    [ -z "$_d" ] && continue
    _v="?"
    _s="ready"
    if [ -f "$_d/Makefile" ]; then
        _v=$(sed -n 's/^VERSION[[:space:]]*[:?]\?=[[:space:]]*//p' "$_d/Makefile" 2>/dev/null | head -n1 | tr -d ' ')
        [ -z "$_v" ] && _v='?'
    fi
    if [ -f "$_d/distinfo" ] && grep -q 'SHA256=TODO' "$_d/distinfo" 2>/dev/null; then
        _s="sha-pending"
    fi
    printf '%-14s %-14s %-10s %s\n' "$_p" "$_catname" "$_v" "$_s"
done
