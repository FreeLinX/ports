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

# xorgproto's directory first, then everything else sorted -- the same order
# mk/common.mk builds FREELINX_PKGCONFIG_LIBDIR in.  This has to agree with the
# build, because the point of the check is to report what a consumer sees.  Sorted
# on its own it does not: xorgproto stages an inputproto.pc at 2.4.0 and
# x11/inputproto stages one at 2.3.2, and plain sort puts the 2.3.2 first, so
#
#   Package dependency requirement 'inputproto >= 2.3.99.1' could not be
#   satisfied.  Package 'inputproto' has version '2.3.2'
#
# is reported against xorg/lib/pkgconfig/xorg-server.pc, while
#
#   PKG_CONFIG_LIBDIR=<the build's order> pkg-config --exists 'xorg-server >= 1.18'
#
# exits 0.
XORGPROTO_PC="$DEPS/xorgproto/lib/pkgconfig"
PCDIRS=$(find "$DEPS" -type d -name pkgconfig -path '*/lib/*' | sort \
    | grep -vx "$XORGPROTO_PC" | tr '\n' ':')
if [ -d "$XORGPROTO_PC" ]; then
    PCDIRS="$XORGPROTO_PC:$PCDIRS"
fi
PCDIRS=${PCDIRS%:}
PKG_CONFIG_LIBDIR="$PWD/$PCDIRS"
export PKG_CONFIG_LIBDIR
unset PKG_CONFIG_PATH

bad=0

# --- 1. generated but not staged ------------------------------------------
# Only a gap when the module describes a library this tree actually staged.
#
# The distinction matters, and getting it wrong in either direction is wrong.
# A package generates a .pc for every variant it can build, not just the one it
# was built as, so libXaw 1.0.16 emits
#
#   Libs: -L${libdir} -lXaw6
#
# next to xaw7.pc's -lXaw7, and the tree has libXaw7.a and no libXaw6.a.  Staging
# xaw6.pc would be worse than not staging it: a consumer asking pkg-config for
# xaw6 would be told about a library, hand the linker -lXaw6, and get
#
#   ld.lld: error: unable to find library -lXaw6
#
# instead of the honest "Package 'xaw6' not found".  pcre2's libpcre2-16.pc and
# libpcre2-32.pc, libevent's libevent_openssl.pc and libXmu's xmuu.pc are the same
# shape: variants this tree did not build.
#
# So the test is not "is the file staged" but "does its Libs: name something in
# the tree".
printf 'generated .pc files that are not staged but describe a staged library:\n'
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
        src=$(find "build/work/$name" -maxdepth 3 -name "$pc" -print -quit)
        [ -n "$src" ] || continue
        have=0
        for lib in $(sed -n '/^Libs/{s/^Libs[^:=]*[:=][ ]*//;p;}' "$src" | tr ' \t' '\n\n' \
                     | sed -n 's|^-l||p'); do
            find "$DEPS" -name "lib$lib.a" -print -quit | grep -q . && have=1
        done
        [ "$have" -eq 1 ] || continue
        printf '  %-16s %s\n' "$name" "$pc"
        bad=1
    done
done
[ "$bad" -eq 0 ] && printf '  (none)\n'

# --- 1b. generated variants this tree did not build --------------------------
# Listed, not an error.  It is the difference between a port that made a choice
# and a port that forgot, and it is the list to read when a consumer says
# "Package 'xaw6' not found".
printf 'generated .pc files for variants this tree did not build (not staged):\n'
variants=0
for d in "$DEPS"/*/; do
    name=$(basename "$d")
    staged=$(ls "$d"lib/pkgconfig 2>/dev/null | sort)
    for pc in $(find "build/work/$name" -maxdepth 2 -name '*.pc' -printf '%f\n' 2>/dev/null | sort -u); do
        printf '%s\n' "$staged" | grep -qx "$pc" && continue
        find "$DEPS" -path "*/lib/pkgconfig/$pc" -print -quit | grep -q . && continue
        case "$pc" in
            *-uninstalled.pc) continue ;;
        esac
        src=$(find "build/work/$name" -maxdepth 3 -name "$pc" -print -quit)
        [ -n "$src" ] || continue
        for lib in $(sed -n '/^Libs/{s/^Libs[^:=]*[:=][ ]*//;p;}' "$src" | tr ' \t' '\n\n' \
                     | sed -n 's|^-l||p'); do
            find "$DEPS" -name "lib$lib.a" -print -quit | grep -q . && continue 2
        done
        printf '  %-16s %s -> %s\n' "$name" "$pc" \
            "$(sed -n '/^Libs/{s/^Libs[^:=]*[:=][ ]*//;p;}' "$src" | head -1 | tr -d '"')"
        variants=$((variants + 1))
    done
done
[ "$variants" -eq 0 ] && printf '  (none)\n'

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

# --- 3. a .pc whose prefix is not the staged tree ----------------------------
# A pkg-config file that says prefix=/usr has its "Cflags: -I/usr/include"
# dropped by pkg-config as a system path, so the module contributes no include
# flags at all.  The build then compiles against whatever the host has, and
# fails on a header the port believed it had provided:
#
#   -Ideps/xproto/include ... and no xorgproto at all, so XKBproto.h's
#   _X_NONSTRING is undefined and xorg-server stops at
#   "XKBproto.h:679:33: error: expected ';' at end of declaration list"
printf 'staged .pc files whose prefix is not inside the tree:\n'
badprefix=0
for pc in $(find "$DEPS" -name '*.pc' -path '*/lib/pkgconfig/*' | sort); do
    prefix=$(sed -n 's|^prefix=||p' "$pc" | head -1 | tr -d '"')
    case "$prefix" in
        "$DEPS"/*|"$PWD/$DEPS"/*) continue ;;
    esac
    printf '  %-46s prefix=%s\n' "${pc#$DEPS/}" "${prefix:-<empty>}"
    badprefix=1
    bad=1
done
[ "$badprefix" -eq 0 ] && printf '  (none)\n'

# --- 4. a staged .pc naming a library that is not in the tree -----------------
# The mirror of check 1.  A staged module whose Libs: names an archive nobody
# built passes checks 2 and 3 and then fails at link time:
#
#   ld.lld: error: unable to find library -lXaw7
#
# with nothing in the module's own file to explain it.  Check 1 asks whether an
# unstaged file should have been staged; this asks whether a staged one lies.
#
# The musl sysroot's own libraries are exempt: -lc, -lm, -lpthread and the rest
# are not in build/deps and never will be, and pkg-config's Libs.private for
# almost every module here names some of them.
printf 'staged .pc files whose Libs name a library that is not in the tree:\n'
SYSROOT=${FREELINX_SYSROOT:-}
nolib=0
for pc in $(find "$DEPS" -name '*.pc' -path '*/lib/pkgconfig/*' | sort); do
    for lib in $(sed -n '/^Libs/{s/^Libs[^:=]*[:=][ ]*//;p;}' "$pc" | tr ' \t' '\n\n' \
                 | sed -n 's|^-l||p' | sort -u); do
        find "$DEPS" -name "lib$lib.a" -print -quit | grep -q . && continue
        [ -n "$SYSROOT" ] && [ -f "$SYSROOT/lib/lib$lib.a" ] && continue
        printf '  %-46s -l%s\n' "${pc#$DEPS/}" "$lib"
        nolib=1
        bad=1
    done
done
[ "$nolib" -eq 0 ] && printf '  (none)\n'

exit "$bad"
