#!/bin/sh
# FreeLinX/ports package generator (package.sh)
#
# Packages built ports into .xpkg archives and generates a repo index.json.
#
# Usage:
#   ./scripts/package.sh [category/port]
#   ./scripts/package.sh all
#
# If no port is specified, it packages all available ports.
# Output archives are saved in ports/packages/

set -eu
. "$(dirname "$0")/common.sh"

flx_load_config

PKG_DIR="$FREELINX_ROOT/packages"
mkdir -p "$PKG_DIR"

ROOTFS_DIR="$FREELINX_ROOT/../src/rootfs"

XPKG_CREATE="$FREELINX_ROOT/../xpkg/xpkg-create"
if [ ! -x "$XPKG_CREATE" ]; then
    printf '[FreeLinX/ports][error] xpkg-create not found or not executable: %s\n' "$XPKG_CREATE" >&2
    exit 1
fi

package_one_port() {
    _target="$1"
    _pname="$(basename "$_target")"
    _portdir=""
    
    for _cat in base shells net firmware sysutils audio devel editors games graphics security textproc www x11; do
        if [ -d "$FREELINX_ROOT/$_cat/$_pname" ]; then
            _portdir="$FREELINX_ROOT/$_cat/$_pname"
            break
        fi
    done

    if [ -z "$_portdir" ] || [ ! -f "$_portdir/Makefile" ]; then
        return 1
    fi

    _version=$(sed -n 's/^VERSION[[:space:]]*[:?]\?=[[:space:]]*//p' "$_portdir/Makefile" | head -n1 | tr -d ' ')
    [ -z "$_version" ] && _version="1.0"

    _depends=$(sed -n 's/^PKG_DEPENDS[[:space:]]*[:?]\?=[[:space:]]*//p' "$_portdir/Makefile" | head -n1 | tr -d ' ')
    [ -z "$_depends" ] && _depends=""

    _bin=$(sed -n 's/^INSTALL_BIN[[:space:]]*[:?]\?=[[:space:]]*//p' "$_portdir/Makefile" | head -n1 | tr -d ' ')
    [ -z "$_bin" ] && _bin="$_pname"

    _desc=$(sed -n 's/^#[[:space:]]*FreeLinX\/ports - [^:]*:[[:space:]]*\(.*\)/\1/p' "$_portdir/Makefile" | head -n1)
    [ -z "$_desc" ] && _desc="FreeLinX $_pname package"

    # Stage directory for this specific port
    _temp_stage=$(mktemp -d "/tmp/flx_pkg_${_pname}.XXXXXX")

    _found=0

    # Special bundling for flxtools suite
    if [ "$_pname" = "flxtools" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -f "$_sdir/usr/bin/flxbg" ] || [ -h "$_sdir/usr/bin/flxbg" ]; then
                mkdir -p "$_temp_stage/usr/bin" "$_temp_stage/sbin"
                # usr/bin: flx* desktop tools (current renamed binaries)
                for _tb in flxbg flxpanel flxshot flx3dtest flxfs flxnet flxpower flxbrowser; do
                    if [ -e "$_sdir/usr/bin/$_tb" ]; then
                        cp -a "$_sdir/usr/bin/$_tb" "$_temp_stage/usr/bin/$_tb"
                    fi
                done
                # mkfs.flxfs symlink -> flxfs
                if [ -e "$_sdir/usr/bin/mkfs.flxfs" ]; then
                    cp -a "$_sdir/usr/bin/mkfs.flxfs" "$_temp_stage/usr/bin/mkfs.flxfs"
                fi
                # sbin: flx system tools (current renamed binaries)
                for _tb in flxwifi flxdriver flxpart flxadduser flxinstall flxautomount flxdriverhotplug; do
                    if [ -e "$_sdir/sbin/$_tb" ]; then
                        cp -a "$_sdir/sbin/$_tb" "$_temp_stage/sbin/$_tb"
                    fi
                done
                _found=1
                break
            fi
        done
    elif [ "$_pname" = "flxt" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -f "$_sdir/usr/bin/flxt" ]; then
                mkdir -p "$_temp_stage/usr/bin"
                cp -a "$_sdir/usr/bin/flxt" "$_temp_stage/usr/bin/"
                ln -sf flxt "$_temp_stage/usr/bin/flxpad"
                _found=1
            fi
        done
    elif [ "$_pname" = "dillo" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in dillo dpid dpidc dillo-install-hyphenation; do
                if [ -f "$_sdir/usr/bin/$_b" ]; then
                    mkdir -p "$_temp_stage/usr/bin"
                    cp -a "$_sdir/usr/bin/$_b" "$_temp_stage/usr/bin/"
                    _found=1
                fi
            done
            if [ -d "$_sdir/usr/lib/dillo" ]; then
                mkdir -p "$_temp_stage/usr/lib"
                cp -a "$_sdir/usr/lib/dillo" "$_temp_stage/usr/lib/"
                _found=1
            fi
            if [ -d "$_sdir/etc/dillo" ]; then
                mkdir -p "$_temp_stage/etc"
                cp -a "$_sdir/etc/dillo" "$_temp_stage/etc/"
                _found=1
            fi
            if [ -f "$_sdir/usr/share/applications/dillo.desktop" ]; then
                mkdir -p "$_temp_stage/usr/share/applications"
                cp -a "$_sdir/usr/share/applications/dillo.desktop" "$_temp_stage/usr/share/applications/"
            fi
        done
    elif [ "$_pname" = "st" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in st xterm uxterm; do
                if [ -f "$_sdir/usr/bin/$_b" ] || [ -h "$_sdir/usr/bin/$_b" ]; then
                    mkdir -p "$_temp_stage/usr/bin"
                    cp -a "$_sdir/usr/bin/$_b" "$_temp_stage/usr/bin/"
                    _found=1
                fi
            done
        done
    elif [ "$_pname" = "flexfiboot" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in flexfiboot efibootmgr; do
                for _bdir in sbin usr/sbin; do
                    if [ -f "$_sdir/$_bdir/$_b" ] || [ -h "$_sdir/$_bdir/$_b" ]; then
                        mkdir -p "$_temp_stage/$_bdir"
                        cp -a "$_sdir/$_bdir/$_b" "$_temp_stage/$_bdir/"
                        _found=1
                    fi
                done
            done
        done
    elif [ "$_pname" = "mupdf" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in mupdf mupdf-x11; do
                if [ -f "$_sdir/usr/bin/$_b" ] || [ -h "$_sdir/usr/bin/$_b" ]; then
                    mkdir -p "$_temp_stage/usr/bin"
                    cp -a "$_sdir/usr/bin/$_b" "$_temp_stage/usr/bin/"
                    _found=1
                fi
            done
            if [ -f "$_sdir/usr/share/applications/mupdf.desktop" ]; then
                mkdir -p "$_temp_stage/usr/share/applications"
                cp -a "$_sdir/usr/share/applications/mupdf.desktop" "$_temp_stage/usr/share/applications/"
            fi
        done
    elif [ "$_pname" = "mpv" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -f "$_sdir/usr/bin/mpv" ]; then
                mkdir -p "$_temp_stage/usr/bin"
                cp -a "$_sdir/usr/bin/mpv" "$_temp_stage/usr/bin/"
                _found=1
            fi
            if [ -f "$_sdir/usr/share/applications/mpv.desktop" ]; then
                mkdir -p "$_temp_stage/usr/share/applications"
                cp -a "$_sdir/usr/share/applications/mpv.desktop" "$_temp_stage/usr/share/applications/"
            fi
        done
    elif [ "$_pname" = "xorg" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -f "$_sdir/usr/bin/Xorg" ]; then
                mkdir -p "$_temp_stage/usr/bin" "$_temp_stage/etc/X11"
                cp -a "$_sdir/usr/bin/Xorg" "$_temp_stage/usr/bin/Xorg"
                cp -a "$_sdir/etc/X11/xorg.conf" "$_temp_stage/etc/X11/xorg.conf"
                _found=1
                # Modules: canonical /usr/lib/xorg/modules (whole dirs)
                mkdir -p "$_temp_stage/usr/lib/xorg/modules/drivers" "$_temp_stage/usr/lib/xorg/modules/input"
                cp -a "$_sdir/usr/lib/xorg/modules/drivers/"* "$_temp_stage/usr/lib/xorg/modules/drivers/"
                cp -a "$_sdir/usr/lib/xorg/modules/input/"* "$_temp_stage/usr/lib/xorg/modules/input/"
                for _lib in libfbdevhw.so libshadow.so libshadowfb.so; do
                    cp -a "$_sdir/usr/lib/xorg/modules/$_lib" "$_temp_stage/usr/lib/xorg/modules/$_lib"
                done
                # Modules: xtree copy (ModulePath[0] in xorg.conf): drivers/*,
                # input/evdev_drv.so only (stale .la excluded), and 3 base libs
                mkdir -p "$_temp_stage/usr/share/X11/xtree/lib/xorg/modules/drivers" "$_temp_stage/usr/share/X11/xtree/lib/xorg/modules/input"
                cp -a "$_sdir/usr/share/X11/xtree/lib/xorg/modules/drivers/"* "$_temp_stage/usr/share/X11/xtree/lib/xorg/modules/drivers/"
                cp -a "$_sdir/usr/share/X11/xtree/lib/xorg/modules/input/"evdev_drv.so "$_temp_stage/usr/share/X11/xtree/lib/xorg/modules/input/"
                for _lib in libfbdevhw.so libshadow.so libshadowfb.so; do
                    cp -a "$_sdir/usr/share/X11/xtree/lib/xorg/modules/$_lib" "$_temp_stage/usr/share/X11/xtree/lib/xorg/modules/$_lib"
                done
                # XKB data (3.8M)
                cp -a "$_sdir/usr/share/X11/xkb" "$_temp_stage/usr/share/X11/xkb"
                break
            fi
        done
    elif [ "$_pname" = "openbox" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -e "$_sdir/usr/bin/openbox" ]; then
                # Binaries (openbox is a symlink → ../share/X11/xtree/bin/openbox)
                mkdir -p "$_temp_stage/usr/bin" "$_temp_stage/usr/lib/openbox" "$_temp_stage/etc/xdg/openbox"
                cp -a "$_sdir/usr/bin/openbox" "$_temp_stage/usr/bin/openbox"
                cp -a "$_sdir/usr/bin/openbox-session" "$_temp_stage/usr/bin/openbox-session"
                cp -a "$_sdir/usr/bin/obxprop" "$_temp_stage/usr/bin/obxprop"
                # xtree binaries (flat dir; the real openbox binary lives here)
                mkdir -p "$_temp_stage/usr/share/X11/xtree/bin" "$_temp_stage/usr/share/X11/xtree/share"
                cp -a "$_sdir/usr/share/X11/xtree/bin/"* "$_temp_stage/usr/share/X11/xtree/bin/"
                cp -a "$_sdir/usr/share/X11/xtree/share/themes" "$_temp_stage/usr/share/X11/xtree/share/themes"
                # Configs + autostart
                cp -a "$_sdir/etc/xdg/openbox/"* "$_temp_stage/etc/xdg/openbox/"
                cp -a "$_sdir/usr/lib/openbox/openbox-autostart" "$_temp_stage/usr/lib/openbox/openbox-autostart"
                _found=1
                break
            fi
        done
    elif [ "$_pname" = "urxvt" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in urxvt rxvt; do
                if [ -f "$_sdir/usr/bin/$_b" ] || [ -h "$_sdir/usr/bin/$_b" ]; then
                    mkdir -p "$_temp_stage/usr/bin"
                    cp -a "$_sdir/usr/bin/$_b" "$_temp_stage/usr/bin/"
                    _found=1
                fi
            done
        done
    elif [ "$_pname" = "x11-apps" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _b in xeyes xmag xdemo xsetroot glxgears; do
                if [ -f "$_sdir/usr/bin/$_b" ] || [ -h "$_sdir/usr/bin/$_b" ]; then
                    mkdir -p "$_temp_stage/usr/bin"
                    cp -a "$_sdir/usr/bin/$_b" "$_temp_stage/usr/bin/"
                    _found=1
                fi
            done
        done
    elif [ "$_pname" = "fonts" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -d "$_sdir/usr/share/fonts/TTF" ]; then
                mkdir -p "$_temp_stage/usr/share/fonts/TTF" "$_temp_stage/etc/fonts"
                cp -a "$_sdir/usr/share/fonts/TTF/"* "$_temp_stage/usr/share/fonts/TTF/"
                cp -a "$_sdir/etc/fonts/fonts.conf" "$_temp_stage/etc/fonts/fonts.conf"
                _found=1
                break
            fi
        done
    elif [ "$_pname" = "i3status" ]; then
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            if [ -f "$_sdir/usr/bin/i3status" ]; then
                mkdir -p "$_temp_stage/usr/bin"
                cp -a "$_sdir/usr/bin/i3status" "$_temp_stage/usr/bin/i3status"
                _found=1
            fi
            if [ -f "$_sdir/etc/i3status.conf" ]; then
                mkdir -p "$_temp_stage/etc"
                cp -a "$_sdir/etc/i3status.conf" "$_temp_stage/etc/i3status.conf"
                _found=1
            fi
        done
    else
        # Search staging root, then fallback to rootfs
        for _sdir in "$FREELINX_STAGING_ROOT" "$ROOTFS_DIR"; do
            for _bdir in bin sbin usr/bin usr/sbin usr/games; do
                _root="$_sdir/$_bdir"
                for _cand in "$_pname" "$_bin"; do
                    if [ -f "$_root/$_cand" ] || [ -h "$_root/$_cand" ]; then
                        mkdir -p "$_temp_stage/$_bdir"
                        cp -a "$_root/$_cand" "$_temp_stage/$_bdir/"
                        _found=1
                    fi
                done
            done
            if [ -d "$_sdir/usr/lib/$_pname" ]; then
                mkdir -p "$_temp_stage/usr/lib"
                cp -a "$_sdir/usr/lib/$_pname" "$_temp_stage/usr/lib/"
                _found=1
            fi
            if [ -d "$_sdir/etc/$_pname" ]; then
                mkdir -p "$_temp_stage/etc"
                cp -a "$_sdir/etc/$_pname" "$_temp_stage/etc/"
                _found=1
            fi
        done
    fi

    if [ "$_found" -eq 0 ]; then
        rm -rf "$_temp_stage"
        return 0
    fi

    _out_pkg="$PKG_DIR/${_pname}-${_version}.xpkg"
    printf '[FreeLinX/ports] Creating package: %s\n' "$_out_pkg"
    if [ -n "$_depends" ]; then
        "$XPKG_CREATE" create \
            --name "$_pname" \
            --version "$_version" \
            --description "$_desc" \
            --arch "$FREELINX_ARCH" \
            --depends "$_depends" \
            --stage "$_temp_stage" \
            --output "$_out_pkg"
    else
        "$XPKG_CREATE" create \
            --name "$_pname" \
            --version "$_version" \
            --description "$_desc" \
            --arch "$FREELINX_ARCH" \
            --stage "$_temp_stage" \
            --output "$_out_pkg"
    fi

    rm -rf "$_temp_stage"
}

if [ $# -gt 0 ] && [ "$1" != "all" ]; then
    package_one_port "$1"
else
    printf '[FreeLinX/ports] Scanning and packaging all ports into xpkg...\n'
    for _pdir in "$FREELINX_ROOT"/*/*; do
        if [ -d "$_pdir" ] && [ -f "$_pdir/Makefile" ]; then
            _pn="$(basename "$_pdir")"
            package_one_port "$_pn" 2>/dev/null || true
        fi
    done
fi

# Update index.json
printf '[FreeLinX/ports] Updating repository index: %s/index.json\n' "$PKG_DIR"
"$XPKG_CREATE" index --dir "$PKG_DIR" --output "$PKG_DIR/index.json"
printf '[FreeLinX/ports] Packaging complete. %d packages indexed.\n' "$(grep -c '"file":' "$PKG_DIR/index.json" 2>/dev/null || echo 0)"
