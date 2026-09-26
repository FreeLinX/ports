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
            for _cat in base shells net firmware; do
                [ -d "$FREELINX_ROOT/$_cat/$1" ] && { printf '%s/%s/%s\n' "$FREELINX_ROOT" "$_cat" "$1"; return 0; }
            done
            return 1
            ;;
    esac
}

flx_all_ports() {
    find "$FREELINX_ROOT/base" "$FREELINX_ROOT/shells" "$FREELINX_ROOT/net" "$FREELINX_ROOT/firmware" -mindepth 1 -maxdepth 1 -type d 2>/dev/null \
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
    _all=yes
fi

# A port may declare itself unbuildable on a Linux kernel by setting
# PORT_NOT_PORTABLE to a reason in its Makefile (see mk/port.mk).  In a
# whole-tree run those are skipped and counted, so the run ends with an honest
# "N built, M skipped" instead of dying on the first NetBSD-kernel-only tool.
# When a port is named explicitly the declaration is still respected, but it is
# reported as an error rather than silently skipped.
flx_not_portable() {
    # prints the reason on stdout if the port declares one, else nothing
    [ -f "$1/Makefile" ] || return 0
    sed -n 's/^[[:space:]]*PORT_NOT_PORTABLE[[:space:]]*:*=[[:space:]]*//p' "$1/Makefile" \
        | sed 's/[[:space:]]*\\$//' | head -1
}

_flx_built=0
_flx_failed=0
_flx_skipped=0
_flx_skip_list=""

for p in "$@"; do
    _dir=$(flx_port_dir "$p") || flx_die "no such port: $p"
    if [ ! -f "$_dir/Makefile" ]; then
        # A directory that is not a port must not be able to end a whole-tree
        # run.  An empty leftover named after some other tool once stopped a
        # 293-port build at the letter "t", after 200-odd ports had already
        # built, and the summary never printed.  In a whole-tree run the
        # directory is reported and skipped; naming it explicitly is still an
        # error, because then the caller asked for something that is not there.
        if [ -n "${_all:-}" ]; then
            flx_warn "skipping $p: no Makefile; not a port"
            continue
        fi
        flx_die "$p: no Makefile; nothing to build"
    fi

    _reason=$(flx_not_portable "$_dir")
    if [ -n "$_reason" ]; then
        # In a whole-tree run this is expected, so skip and count it.  When the
        # port was named explicitly it is still a failure the caller asked for,
        # but keep going so one unbuildable port does not hide the rest of the
        # list; the non-zero exit at the end still reports it.
        if [ -n "${_all:-}" ]; then
            flx_info "skipping $p: $_reason"
            _flx_skipped=$((_flx_skipped + 1))
        else
            flx_warn "$p: cannot be built for FreeLinX: $_reason"
            _flx_failed=$((_flx_failed + 1))
        fi
        _flx_skip_list="$_flx_skip_list $p"
        continue
    fi

    flx_info "Building $p..."
    if ( cd "$_dir" && make FREELINX_ROOT="$FREELINX_ROOT" all ); then
        flx_info "$p: build complete"
        _flx_built=$((_flx_built + 1))
    else
        _flx_failed=$((_flx_failed + 1))
        flx_warn "$p: build failed (see above)"
    fi
done

flx_info "build finished: $_flx_built built, $_flx_failed failed, $_flx_skipped skipped as not portable"
[ -n "$_flx_skip_list" ] && flx_info "not portable:$_flx_skip_list"
[ "$_flx_failed" -eq 0 ]
