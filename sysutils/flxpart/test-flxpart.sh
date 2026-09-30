#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Copyright (c) 2026 FreeLinX OS Project.
#
# test-flxpart.sh - check the partition table flxpart writes.
#
# Works on a regular file, never a real disk: flxpart accepts a plain file
# and treats it exactly like a block device above the size, so the bytes
# written are the bytes a disk would get.  Nothing here can destroy a disk.
#
# The table is checked three ways: by flxpart's own --show, by fdisk and
# parted as independent readers, and by a Python checker that validates the
# CRCs and the geometry from the spec.  The independent readers matter most:
# flxpart reading its own output agrees with itself whether or not it is
# right, and an earlier version of this wrote a table that fdisk accepted and
# parted called corrupt.
set -u

HERE=$(cd "$(dirname "$0")" && pwd)
FLXPART=${FLXPART:-/tmp/fp}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

pass=0
fail=0

ok() {
	if [ "$2" = "$3" ]; then
		pass=$((pass + 1)); printf '  ok   %s\n' "$1"
	else
		fail=$((fail + 1))
		printf '  FAIL %s\n        want [%s]\n        got  [%s]\n' "$1" "$3" "$2"
	fi
}

# make_disk SIZE_MB -> path
make_disk() {
	_d=$TMP/disk-$1.img
	truncate -s "$1"M "$_d"
	printf '%s' "$_d"
}

echo '== a 4 GiB disk, default layout =='
d=$(make_disk 4096)
"$FLXPART" -q --create-standard "$d" >"$TMP/out.txt" 2>"$TMP/err.txt"
ok 'exit status' "$?" '0'
ok 'no complaints' "$(cat "$TMP/err.txt")" ''
ok 'three partitions' "$(grep -c '^FLX_PART[0-9]*_NAME=' "$TMP/out.txt")" '3'

echo '== the three keys the installer reads =='
ok 'part 1 first'  "$(sed -n 's/^FLX_PART1_FIRST=//p' "$TMP/out.txt")" '2048'
ok 'part 1 size'   "$(sed -n 's/^FLX_PART1_SIZE_BYTES=//p' "$TMP/out.txt")" '268435456'
ok 'part 1 is the ESP' "$(sed -n 's/^FLX_PART1_TYPE=//p' "$TMP/out.txt")" \
	'28732AC1-1F81-D211-4BBA-A0A0C93EC93B'
ok 'part 2 is BIOS boot' "$(sed -n 's/^FLX_PART2_TYPE=//p' "$TMP/out.txt")" \
	'94CE8649-9964-6E6F-744E-65ED45464964'
ok 'part 2 is 1 MiB' "$(sed -n 's/^FLX_PART2_SIZE_BYTES=//p' "$TMP/out.txt")" '1048576'

echo '== alignment: every partition starts on a 1 MiB boundary =='
bad=0
for k in 1 2 3; do
	f=$(sed -n "s/^FLX_PART${k}_FIRST=//p" "$TMP/out.txt")
	[ $((f % 2048)) -eq 0 ] || bad=$((bad + 1))
done
ok 'all three aligned' "$bad" '0'

echo '== the last partition stops before the backup array =='
last=$(sed -n 's/^FLX_PART3_LAST=//p' "$TMP/out.txt")
lu=$(sed -n 's/^FLX_DISK_LAST_USABLE=//p' "$TMP/out.txt")
ok 'root ends at last usable' "$last" "$lu"
ok 'ends at, not past, the last usable' "$([ "$last" -le "$lu" ] && echo yes || echo no)" 'yes'

echo '== fdisk agrees (independent reader) =='
f=$(fdisk -l "$d" 2>/dev/null)
ok 'says GPT' "$(printf '%s' "$f" | grep -c 'Disklabel type: gpt')" '1'
ok 'finds three' "$(printf '%s' "$f" | grep -cE 'img[0-9]+ +[0-9]+ +[0-9]+')" '3'
ok 'part 1 starts at 2048' "$(printf '%s' "$f" | awk '/img1 /{print $2}')" '2048'

echo '== parted agrees, and does not call it corrupt (independent reader) =='
p=$(parted -s "$d" unit MiB print 2>&1)
ok 'no corruption warning' "$(printf '%s' "$p" | grep -ci 'corrupt')" '0'
ok 'says gpt' "$(printf '%s' "$p" | grep -c 'Partition Table: gpt')" '1'
ok 'names the first partition' "$(printf '%s' "$p" | grep -c 'EFI system')" '1'

echo '== the spec checker: CRCs, geometry, unused entries =='
python3 - "$d" <<'PY'
import struct, sys, zlib

path = sys.argv[1]
SEC = 512
disk = open(path, 'rb').read()
total = len(disk) // SEC

def sec(lba):
    return disk[lba*SEC:(lba+1)*SEC]

fails = []
def check(name, cond, detail=""):
    if cond:
        print("  ok   %s" % name)
    else:
        print("  FAIL %s %s" % (name, detail))
        fails.append(name)

# --- protective MBR
mbr = sec(0)
check("MBR signature 0x55AA", mbr[510] == 0x55 and mbr[511] == 0xAA,
      "got %02x%02x" % (mbr[510], mbr[511]))
check("MBR partition 1 type 0xEE at offset 446", mbr[446+4] == 0xEE,
      "got %02x at %d" % (mbr[446+4], 446+4))
check("MBR partition 1 starts at LBA 1",
      struct.unpack_from("<I", mbr, 446+8)[0] == 1)

# --- primary header
h = sec(1)
check("primary header signature", h[:8] == b"EFI PART")
check("header revision 1.0", struct.unpack_from("<I", h, 8)[0] == 0x00010000)
check("header size is 92", struct.unpack_from("<I", h, 12)[0] == 92)
stored = struct.unpack_from("<I", h, 16)[0]
zeroed = h[:16] + b"\0\0\0\0" + h[20:]
check("primary header CRC", zlib.crc32(zeroed[:92]) & 0xffffffff == stored,
      "stored %08x computed %08x" % (stored, zlib.crc32(zeroed[:92]) & 0xffffffff))
check("primary array LBA is 2", struct.unpack_from("<Q", h, 72)[0] == 2,
      "got %d" % struct.unpack_from("<Q", h, 72)[0])
check("entry count 128", struct.unpack_from("<I", h, 80)[0] == 128)
check("entry size 128", struct.unpack_from("<I", h, 84)[0] == 128)

first_usable = struct.unpack_from("<Q", h, 40)[0]
last_usable = struct.unpack_from("<Q", h, 48)[0]
check("first usable LBA is 34", first_usable == 34, "got %d" % first_usable)
check("last usable is 33 before the last",
      last_usable == total - 1 - 33, "got %d, total %d" % (last_usable, total))

# --- primary array, at LBA 2..33
arr = b"".join(sec(2 + i) for i in range(32))
check("array length", len(arr) == 128*128)
stored_crc = struct.unpack_from("<I", h, 88)[0]
check("array CRC", zlib.crc32(arr) & 0xffffffff == stored_crc,
      "stored %08x computed %08x" % (stored_crc, zlib.crc32(arr) & 0xffffffff))

# --- entries
parts = []
for i in range(128):
    e = arr[i*128:(i+1)*128]
    if e[:16] == b"\0"*16:
        continue
    t = e[:16]
    first = struct.unpack_from("<Q", e, 32)[0]
    last = struct.unpack_from("<Q", e, 40)[0]
    name = e[56:56+72].decode("utf-16-le").split("\0")[0]
    parts.append((i, t, first, last, name))

check("exactly three live entries", len(parts) == 3, "got %d" % len(parts))
check("unused entries are all zero",
      all(arr[i*128:(i+1)*128] == b"\0"*128 for i in range(len(parts), 128)))
for i, t, first, last, name in parts:
    check("part %d (%s) is in range" % (i+1, name),
          first_usable <= first <= last <= last_usable,
          "first %d last %d, usable %d..%d" % (first, last, first_usable, last_usable))
    check("part %d (%s) is 1 MiB aligned" % (i+1, name), first % 2048 == 0)
# no overlap
ordered = sorted(parts, key=lambda p: p[2])
check("partitions do not overlap",
      all(ordered[k][3] < ordered[k+1][2] for k in range(len(ordered)-1)))
# nothing before first usable, nothing into the tail
check("nothing lands in the primary array",
      all(p[2] >= FIRST for p in parts) if (FIRST := 34) else False)

# --- backup header
bh = sec(total - 1)
check("backup header signature", bh[:8] == b"EFI PART")
bstored = struct.unpack_from("<I", bh, 16)[0]
bzeroed = bh[:16] + b"\0\0\0\0" + bh[20:]
check("backup header CRC", zlib.crc32(bzeroed[:92]) & 0xffffffff == bstored)
check("backup header current LBA is last", struct.unpack_from("<Q", bh, 24)[0] == total - 1)
check("backup header points at primary", struct.unpack_from("<Q", bh, 32)[0] == 1)
check("backup array LBA", struct.unpack_from("<Q", bh, 72)[0] == total - 33)
check("same disk GUID in both headers", bh[56:72] == h[56:72])
# the backup array is a copy
barr = b"".join(sec(total - 33 + i) for i in range(32))
check("backup array matches the primary", barr == arr)
check("backup array CRC", zlib.crc32(barr) & 0xffffffff == bstored and False or
      zlib.crc32(barr) & 0xffffffff == struct.unpack_from("<I", bh, 88)[0])

# --- the ESP type GUID, byte for byte
ESP = bytes([0x28,0x73,0x2a,0xc1,0x1f,0x81,0xd2,0x11,0x4b,0xba,0xa0,0xa0,0xc9,0x3e,0xc9,0x3b])
check("part 1 type is the ESP GUID", parts[0][1] == ESP if parts else False)

print("  --- %s ---" % ("all spec checks passed" if not fails else "FAILURES: %s" % fails))
sys.exit(1 if fails else 0)
PY
ok 'spec checker agrees' "$?" '0'

echo '== --show reads back what was written =='
s=$("$FLXPART" --show "$d" 2>&1)
ok 'reports GPT' "$(printf '%s' "$s" | grep -c 'GPT')" '1'
ok 'lists three' "$(printf '%s' "$s" | grep -cE '^  [0-9]+:')" '3'
ok 'names the ESP' "$(printf '%s' "$s" | grep -c 'EFI system')" '1'

echo '== an unpartitioned file is reported, not invented =='
blank=$(make_disk 64)
s=$("$FLXPART" --show "$blank" 2>&1)
ok 'says not partitioned' "$(printf '%s' "$s" | grep -c 'not partitioned GPT')" '1'

echo '== a small disk is refused before anything is written =='
tiny=$(make_disk 8)
"$FLXPART" --create-standard "$tiny" >/dev/null 2>"$TMP/e.txt"
ok 'exits non-zero' "$?" '1'
ok 'says why' "$(grep -cE 'cannot hold|too small|no room' "$TMP/e.txt")" '1'
# cmp against /dev/zero would compare 8 MB to 1 byte and always differ; what
# matters is that not one byte of the disk was written.
ok 'wrote nothing' "$(tr -d '\0' <"$tiny" | wc -c | tr -d ' ')" '0'

echo '== bad arguments =='
d2=$(make_disk 512)
"$FLXPART" >/dev/null 2>&1; ok 'no args fails' "$?" '1'
"$FLXPART" --create-standard >/dev/null 2>&1; ok 'no device fails' "$?" '1'
"$FLXPART" --bogus "$d2" >/dev/null 2>&1; ok 'unknown option fails' "$?" '1'
"$FLXPART" --create-standard --show "$d2" >/dev/null 2>&1
ok 'two jobs at once fails' "$?" '1'
"$FLXPART" --create-standard --esp-size 0 "$d2" >/dev/null 2>&1
ok 'esp-size 0 fails' "$?" '1'
"$FLXPART" --version >/dev/null 2>&1; ok '--version works' "$?" '0'
"$FLXPART" --help >/dev/null 2>&1; ok '--help works' "$?" '0'

echo '== a small ESP, and a 3 TiB disk (64-bit LBAs) =='
d3=$(make_disk 2048)
"$FLXPART" -q --create-standard --esp-size 64 "$d3" >"$TMP/o3.txt" 2>&1
ok 'esp-size 64 honoured' "$(sed -n 's/^FLX_PART1_SIZE_BYTES=//p' "$TMP/o3.txt")" '67108864'
p3=$(parted -s "$d3" unit MiB print 2>&1)
ok 'parted accepts the small ESP too' "$(printf '%s' "$p3" | grep -ci corrupt)" '0'

big=$TMP/big.img
truncate -s 3T "$big"
"$FLXPART" -q --create-standard "$big" >"$TMP/o4.txt" 2>&1
ok '3 TiB works' "$?" '0'
p4=$(parted -s "$big" unit GiB print 2>&1)
ok 'parted accepts 3 TiB' "$(printf '%s' "$p4" | grep -ci corrupt)" '0'
ok 'the root partition is huge' \
	"$([ "$(sed -n 's/^FLX_PART3_SIZE_BYTES=//p' "$TMP/o4.txt")" -gt 3000000000000 ] && echo yes || echo no)" 'yes'

echo '== re-running on the same disk is the same result =='
d4=$(make_disk 1024)
"$FLXPART" -q --create-standard "$d4" >"$TMP/o5.txt" 2>&1
a=$(grep -vE 'GUID=' "$TMP/o5.txt")
"$FLXPART" -q --create-standard "$d4" >"$TMP/o6.txt" 2>&1
b=$(grep -vE 'GUID=' "$TMP/o6.txt")
ok 'the layout is identical' "$a" "$b"

echo '== GUIDs differ between two runs (they are random, not fixed) =='
e1=$(make_disk 1024); e2=$(make_disk 1024)
"$FLXPART" -q --create-standard "$e1" | grep '^FLX_PART1_GUID=' >"$TMP/g1"
"$FLXPART" -q --create-standard "$e2" | grep '^FLX_PART1_GUID=' >"$TMP/g2"
ok 'not the same GUID' "$(cmp -s "$TMP/g1" "$TMP/g2" && echo same || echo different)" 'different'

printf '\n%s passed, %s failed\n' "$pass" "$fail"
[ "$fail" -eq 0 ]
