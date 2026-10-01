#!/bin/sh
# FreeLinX/ports - scripts/check-deps-pkgconfig.sh
#
# Check that the staged dependency tree in build/deps is a usable pkg-config
# closure, and that no port is sitting on a .pc it built but did not stage.
#
# Two failures matter and neither is visible from a build log.
#
#   1. A generated .pc that was never staged.  The port builds, links its own
#      test program, and looks finished.  The consumer is the one that fails,
#      with "Package 'xau' not found" or "Package requirements (ice >= 1.1.0)
#      were not met" -- and both of those name the consumer's own requirement
#      rather than the file that is missing.  Nine of the hand-written X.Org
#      ports were like this, and the module name usually is not the library's
#      name: libXau generates xau.pc, libICE generates ice.pc, libXt generates
#      xt.pc.
#
#   2. A staged .pc whose Requires cannot be resolved inside the tree.  Same
#      shape, one step later on.
#
# Run from the ports root.  Non-zero exit if anything is reported.
set -eu

DEPS=build/deps
[ -d "$DEPS" ] || { printf 'no %s; build something first\n' "$DEPS" >&2; exit 2; }

PCDIRS=$(find "$DEPS" -type d -name pkgconfig -path '*/lib/*' | sort | tr '\n' ':')
PCDIRS=${PCDIRS%:}
PKG_CONFIG_LIBDIR="$PWD/$PCDIRS"
export PKG_CONFIG_LIBDIR
unset PKG_CONFIG_PATH

bad=0

# --- 1. generated but not staged ------------------------------------------
printf 'generated .pc files that are not staged:\n'
for d in "$DEPS"/*/; do
    name=$(basename "$d")
    staged=$(ls "$d"lib/pkgconfig 2>/dev/null | sort)
    for pc in $(find "build/work/$name" -maxdepth 2 -name '*.pc' -printf '%f\n' 2>/dev/null | sort -u); do
        printf '%s\n' "$staged" | grep -qx "$pc" && continue
        # A module another dep already stages counts as staged.  libxcb builds
        # xcb-randr.pc and so does xcb-proto, and the xcb-proto one is the one
        # consumers should see, so reporting libxcb's copy as a gap is noise.
        find "$DEPS" -path "*/lib/pkgconfig/$pc" -print -quit | grep -q . && continue
        case "$pc" in
            *-uninstalled.pc) continue ;;
        esac
        printf '  %-16s %s\n' "$name" "$pc"
        bad=1
    done
done
[ "$bad" -eq 0 ] && printf '  (none)\n'

# --- 2. staged but unresolvable -------------------------------------------
printf 'staged .pc files whose requirements do not resolve in the tree:\n'
unresolved=0
for pc in $(find "$DEPS" -name '*.pc' -path '*/lib/pkgconfig/*' | sort); do
    # The module name is the file's name, not its Name: field.  ice.pc and
    # xcb.pc both say "ICE" and "XCB" there, and asking pkg-config for those
    # reports every one of them as missing.
    mod=$(basename "$pc" .pc)
    if ! out=$(pkg-config --print-errors --exists "$mod" 2>&1); then
        printf '  %-46s %s\n' "${pc#$DEPS/}" "$(printf '%s' "$out" | head -1)"
        unresolved=1
        bad=1
    fi
done
[ "$unresolved" -eq 0 ] && printf '  (none)\n'

exit "$bad"
