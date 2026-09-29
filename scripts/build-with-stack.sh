#!/bin/sh
# FreeLinX/ports - rebuild ports with the desktop stack's clang/musl sysroot.
#
# cowsay, figlet, fortune, dzen2 and lemonbar used to be linked against
# GCC-built objects on another machine; this builds them with the FreeLinX
# toolchain against Desktop-test/stack/work/sysroot (X11 apps link its shared
# libraries) and writes the packages to packages/.
#
#   scripts/build-with-stack.sh
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TOP="$(cd "$ROOT/.." && pwd)"
W="${STACK_WORK:-$TOP/Desktop-test/stack/work}"
CC="$W/bin/flx-cc"
PC="$W/bin/pkg-config"
STRIP="$TOP/toolchain/bin/llvm-strip"
CREATE="$TOP/xpkg/xpkg-create"
COMPAT="$ROOT/base/compat"
B="$W/build/ports-clean"
P="$ROOT/packages"

rm -rf "$B"; mkdir -p "$B/st"; cd "$B"

# cowsay
mkdir -p st/cowsay/usr/bin
"$CC" -O2 -static -o st/cowsay/usr/bin/cowsay "$ROOT/games/cowsay/cowsay.c"
ln -s cowsay st/cowsay/usr/bin/cowthink

# figlet
tar -xzf "$ROOT/dist/figlet-2.2.5.tar.gz"
(cd figlet-2.2.5 && "$CC" -O2 -static -D__BEGIN_DECLS= -D__END_DECLS= -DTLF_FONTS \
    -DDEFAULTFONTDIR='"/usr/share/figlet"' -DDEFAULTFONTFILE='"standard"' \
    -o figlet figlet.c zipio.c crc.c inflate.c utf8.c)
mkdir -p st/figlet/usr/bin st/figlet/usr/share/figlet
cp figlet-2.2.5/figlet st/figlet/usr/bin/
cp figlet-2.2.5/fonts/*.flf figlet-2.2.5/fonts/*.flc st/figlet/usr/share/figlet/

# fortune (NetBSD 10.1); the datfiles are data, not code
tar -xzf "$ROOT/dist/src.tgz" usr/src/games/fortune/fortune usr/src/games/fortune/strfile \
    usr/src/games/fortune/datfiles 2>/dev/null || :
f=usr/src/games/fortune/fortune
sed -i 's/^#define[[:space:]]*NAMLEN(d).*/#define NAMLEN(d) strlen((d)->d_name)/' "$f/fortune.c"
printf '#include <endian.h>\n#define BE64TOH(x) ((x) = be64toh(x))\n#define BE32TOH(x) ((x) = be32toh(x))\n' > "$f/flx_endian.h"
"$CC" -O2 -static -I"$COMPAT" -I"$f/../strfile" -include flx_bsd.h -include "$f/flx_endian.h" \
    -o "$f/fortune" "$f/fortune.c" "$COMPAT/arc4random.c" "$COMPAT/getprogname.c"
"$CC" -O2 -static -I"$COMPAT" -include flx_bsd.h -include "$f/flx_endian.h" \
    -o strfile "$f/../strfile/strfile.c" "$COMPAT/arc4random.c" "$COMPAT/getprogname.c"
mkdir -p st/fortune/usr/bin st/fortune/usr/share/fortune
cp "$f/fortune" st/fortune/usr/bin/
for d in fortunes fortunes2 limerick startrek zippy; do
    [ -f "usr/src/games/fortune/datfiles/$d" ] || continue
    cp "usr/src/games/fortune/datfiles/$d" st/fortune/usr/share/fortune/
    ./strfile -s "st/fortune/usr/share/fortune/$d"
done

# dzen2 and lemonbar (X11, shared stack libraries)
curl -sfL -o dzen.tgz https://codeload.github.com/robm/dzen/tar.gz/refs/heads/master
echo "b2098a6fdedbedc0e707a97283fc7cf6e37332ff6ca378432cb8505a2a1bf7bf  dzen.tgz" | sha256sum -c -
tar -xzf dzen.tgz
(cd dzen-master && "$CC" -O2 -DVERSION='"0.9.5"' -DDZEN_XFT $("$PC" --cflags xft x11) \
    -o dzen2 ./*.c $("$PC" --libs xft x11))
curl -sfL -o lemonbar.tgz https://codeload.github.com/silentz/lemonbar-xft/tar.gz/refs/heads/master
echo "722642c648ba2ba679dc08ab3ead6dba3d388692dd8778f6163a04418229cf77  lemonbar.tgz" | sha256sum -c -
tar -xzf lemonbar.tgz
m="xcb xcb-xinerama xcb-randr x11 x11-xcb xft"
(cd lemonbar-xft-master && "$CC" -O2 -std=c99 -D_GNU_SOURCE -DVERSION='"1.4-xft"' \
    $("$PC" --cflags $m) -o lemonbar lemonbar.c utils.c $("$PC" --libs $m))
mkdir -p st/dzen2/usr/bin st/lemonbar/usr/bin
cp dzen-master/dzen2 st/dzen2/usr/bin/
cp lemonbar-xft-master/lemonbar st/lemonbar/usr/bin/

for d in st/*; do
    for e in "$d"/usr/bin/*; do [ -L "$e" ] || "$STRIP" --strip-unneeded "$e"; done
    "$TOP/Desktop-test/check-nognu.sh" "$d" | tail -1
done

"$CREATE" create --name cowsay --version 3.7.0-1 --description "cowsay 3.7.0 (talking cow generator)." \
    --stage st/cowsay --output "$P/cowsay-3.7.0-1.xpkg"
"$CREATE" create --name figlet --version 2.2.5-1 --description "figlet 2.2.5 (banner generator)." \
    --stage st/figlet --output "$P/figlet-2.2.5-1.xpkg"
"$CREATE" create --name fortune --version 10.1-1 --description "NetBSD fortune 10.1 (random quotes generator)." \
    --stage st/fortune --output "$P/fortune-10.1-1.xpkg"
"$CREATE" create --name dzen2 --version 0.9.5-1 --description "dzen2 0.9.5 general purpose messaging utility." \
    --depends libXft,libX11,musl --stage st/dzen2 --output "$P/dzen2-0.9.5-1.xpkg"
"$CREATE" create --name lemonbar --version 1.4-1 --description "Lemonbar 1.4 (Xft) status bar." \
    --depends libxcb,libX11,libXft,musl,fonts --stage st/lemonbar --output "$P/lemonbar-1.4-1.xpkg"
