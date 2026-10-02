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
            for _cat in base shells net firmware; do
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

    # A native port has no upstream to fetch, and there are two ways a distinfo
    # says so.  base/dmesg, base/flxpasswd, base/flxuseradd, base/mount,
    # base/umount and sysutils/flxpart carry an empty DISTINFO_ARCHIVE and a
    # placeholder URL of "(native FreeLinX implementation)".  base/ifconfig and
    # base/route carry a bare DISTINFO_NAME and nothing else, because they are
    # built from files/ inside the port directory.
    #
    # This has to come before the guard below, or the second kind aborts on it:
    #
    #   ./scripts/fetch.sh: line 44: DISTINFO_URL: distinfo missing DISTINFO_URL
    #
    # which is a hard error for a port that has nothing to fetch, and put two
    # working ports into the failure report.
    _native=0
    if [ -z "${DISTINFO_URL:-}" ] || [ -z "${DISTINFO_ARCHIVE:-}" ]; then
        _native=1
    else
        case "$DISTINFO_URL" in
            \(*\)|*native*|*"local"*) _native=1 ;;
        esac
    fi
    if [ "$_native" -eq 1 ]; then
        flx_info "$_port: native port, nothing to fetch"
        return 0
    fi

    : "${DISTINFO_URL:?distinfo missing DISTINFO_URL}"

    # Saved under DISTINFO_ARCHIVE, which is the name the build looks for, and
    # not under basename "$DISTINFO_URL".  A GitHub branch archive has the
    # basename of the branch and nothing else:
    #
    #   DISTINFO_URL=https://github.com/ozkl/doomgeneric/archive/refs/heads/master.tar.gz
    #   DISTINFO_ARCHIVE=doomgeneric-master.tar.gz
    #
    # curl -O wrote dist/master.tar.gz, the build asked for
    # dist/doomgeneric-master.tar.gz, and the two never met.  fetch.sh reported
    # "sha256 verified" over the wrong file, so the fetch looked like it had
    # worked:
    #
    #   [FreeLinX/ports] base/doom: already downloaded:
    #       /home/kanan/FreeLinX/ports/dist/master.tar.gz
    #   tar (child): .../dist/doomgeneric-master.tar.gz: Cannot open
    #
    # `-o "$DISTINFO_ARCHIVE"` also survives a URL with no basename at all,
    # which basename would answer with "curl" or empty.
    : "${DISTINFO_ARCHIVE:?distinfo missing DISTINFO_ARCHIVE}"
    _dst="${FREELINX_DIST_DIR}/$DISTINFO_ARCHIVE"
    if [ -f "$_dst" ]; then
        flx_info "$_port: already downloaded: $_dst"
    else
        flx_info "$_port: fetching $DISTINFO_URL"
        mkdir -p "$FREELINX_DIST_DIR"
        (cd "$FREELINX_DIST_DIR" && curl -fLS -o "$DISTINFO_ARCHIVE" "$DISTINFO_URL") || {
            rm -f "${FREELINX_DIST_DIR:?}/$DISTINFO_ARCHIVE"
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
