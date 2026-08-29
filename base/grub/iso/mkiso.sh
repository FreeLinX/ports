#!/bin/sh
# base/grub/iso/mkiso.sh - FreeLinX bootable ISO builder (infrastructure).
#
# NOTE: This is build infrastructure, NOT a fabricated ISO.  It is invoked only
# once a FreeLinX kernel and a built GRUB (grub-mkrescue + xorriso) are
# available.  It is intentionally NOT run as part of Phase 4: the FreeLinX
# kernel does not exist yet and GRUB cannot be built with the FreeLinX
# clang+musl toolchain (see base/grub/Makefile).  Running it would require a
# GCC+binutils cross toolchain and functional objcopy/nm.
#
# Usage (when prerequisites exist):
#   ./mkiso.sh <kernel-binary> <output-iso>
#
# It builds:
#   - x86_64 BIOS image   (el-torito, grub boot.img + core.img)
#   - x86_64 UEFI image   (FAT ESP with BOOTX64.EFI) when grub-mkimage is able
# and combines them into a single hybrid ISO via xorriso -as mkisofs.

set -eu
KERNEL="${1:?usage: mkiso.sh <kernel-binary> <output-iso>}"
OUT="${2:?usage: mkiso.sh <kernel-binary> <output-iso>}"

GRUB_MKRESCUE="${GRUB_MKRESCUE:-grub-mkrescue}"
XORRISO="${XORRISO:-xorriso}"

[ -x "$(command -v "$GRUB_MKRESCUE")" ] || { echo "grub-mkrescue not found" >&2; exit 1; }
[ -x "$(command -v "$XORRISO")" ]      || { echo "xorriso not found" >&2; exit 1; }
[ -f "$KERNEL" ]                       || { echo "kernel not found: $KERNEL" >&2; exit 1; }

WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

mkdir -p "$WORK/boot/grub"
cp "$KERNEL" "$WORK/boot/freelinxf"
cat > "$WORK/boot/grub/grub.cfg" <<'EOF'
set timeout=3
set default=0
menuentry "FreeLinX" {
    linux /boot/freelinxf
}
EOF

# Hybrid BIOS+UEFI ISO.
"$GRUB_MKRESCUE" -o "$OUT" "$WORK" --xorriso="$XORRISO"
echo "Built: $OUT"
