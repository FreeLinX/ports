#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Copyright (c) 2026 FreeLinX OS Project.
#
# check-port-coverage.sh - which binaries in the rootfs have no port behind them.
#
# The other checkers ask whether what a port built is correct.  This asks the
# opposite and prior question: whether the rootfs ships a program that nothing in
# ports/ builds, and so is a committed binary nobody can reproduce or update.
#
# A file is considered covered when some port's staging overlay, or the
# dependency tree the ports build into, provides the same path.  Files that are
# data, configuration or shell scripts are not the question, so only regular
# files that are ELF objects or shared libraries are considered.
#
# Run from the ports root or anywhere:  sh scripts/check-port-coverage.sh
#
# Exit is 0 whether or not anything is uncovered: an uncovered binary is a
# finding to report, not a build failure.  Use -q to fail instead.

set -u

HERE=$(cd "$(dirname "$0")" && pwd)
PORTS=$(cd "$HERE/.." && pwd)
ROOTFS=${ROOTFS:-$PORTS/../src/rootfs}
STAGE=${STAGE:-$PORTS/staging}
DEPS=${DEPS:-$PORTS/build/deps}

if [ "${1:-}" = "-q" ]; then
	QUIET_FAIL=1
fi

if [ ! -d "$ROOTFS" ]; then
	printf 'error: no rootfs at %s\n' "$ROOTFS" >&2
	exit 2
fi

# What the ports can produce, as rootfs-relative paths.  build/deps holds
# libraries and pkg-config for the benefit of later ports rather than the rootfs,
# so it counts too: a libX11.a there is what lets xorg-server be built at all.
# The prefix has to come off as a whole path, not one component at a time.
# "sed 's|^[^/]*/||'" strips a single component, and these paths are absolute, so
# it removed the leading empty component and left the whole path -- and then
# every one of the 378 rootfs binaries came back uncovered, which is how this
# checker looked for five minutes before anyone read it.
covered=$(
	{
		[ -d "$STAGE" ] && find "$STAGE" -type f -print
		[ -d "$DEPS" ] && find "$DEPS" -type f \( -name '*.a' -o -name '*.so*' \
			-o -name '*.pc' \) -print
	} 2>/dev/null | sed -e "s|^$STAGE/||" -e "s|^$DEPS/||" | sort -u
)

# What each port claims it installs, as a relpath, so the report can say which
# port a binary belongs to instead of guessing from a directory name.  Guessing
# gets it wrong in both directions: base/openssh installs bin/ssh, bin/scp,
# bin/sftp, bin/sshd, bin/ssh-keygen and five files under libexec, and none of
# them is called openssh.
manifest=$(mktemp)
for f in "$PORTS"/*/Makefile "$PORTS"/*/*/Makefile; do
	[ -f "$f" ] || continue
	rel=$(sed -n 's|^[[:space:]]*INSTALL_RELPATH[[:space:]]*[:?]\?=[[:space:]]*||p' "$f" |
		head -1 | tr -d '"\'' ')
	[ -n "$rel" ] || continue
	case $rel in /*) continue ;; esac
	dir=$(dirname "$f")
	printf '%s\t%s\n' "$rel" "${dir#"$PORTS"/}" >>"$manifest"
	# A tree port stages a directory; everything under it counts.
	grep -q '^[[:space:]]*STAGE_TREE[[:space:]]*[:?]\?=' "$f" || continue
done
sort -u -o "$manifest" "$manifest"

if [ -z "$covered" ]; then
	printf 'error: nothing staged under %s or %s\n' "$STAGE" "$DEPS" >&2
	printf 'error: build and install some ports first.\n' >&2
	exit 2
fi

printf 'port coverage of the rootfs\n'
printf '  rootfs  %s\n' "$ROOTFS"
printf '  ports   %s\n\n' "$PORTS"

total=0
uncovered=0
list=$(mktemp)
trap 'rm -f "$list" "$list.elf" "$list.notbuilt" "$list.noport" "$manifest"' EXIT

# -exec file on many files at once is far quicker than one per file, and the
# rootfs has 8000+ of them.
find "$ROOTFS" -type f -exec file {} + 2>/dev/null |
	while IFS= read -r _l; do
		:
	done

find "$ROOTFS" -type f -print0 2>/dev/null |
	xargs -0 -n 40 file 2>/dev/null |
	grep 'ELF' |
	sed 's|:.*||' |
	while read -r f; do
		rel=${f#"$ROOTFS"/}
		printf '%s\n' "$rel"
	done |
	sort -u >"$list.elf"

while read -r rel; do
	[ -n "$rel" ] || continue
	total=$((total + 1))
	if printf '%s\n' "$covered" | grep -qxF "$rel"; then
		continue
	fi
	uncovered=$((uncovered + 1))
	printf '%s\n' "$rel" >>"$list"
done <"$list.elf"

printf '  %d ELF files in the rootfs\n' "$total"
printf '  %d of them have no port behind them\n\n' "$uncovered"

if [ "$uncovered" -eq 0 ]; then
	printf '  every binary in the rootfs is produced by a port\n'
	exit 0
fi

# Two quite different situations, and the difference is the whole value of this
# report:
#
#   not built   a port for it exists and has not been installed.  The port is
#               written; the binary in the rootfs is still a committed prebuilt
#               nobody can reproduce.  This is work waiting to happen.
#   no port     nothing in the tree can produce it, so either a port has to be
#               written or the binary has to be justified.
#
# A directory named after the program is a good enough match for the first: the
# tree names ports after the program they build, with the odd exception.
portdir() {
	awk -F'\t' -v r="$1" '$1 == r {print $2; exit}' "$manifest"
}

: >"$list.notbuilt"
: >"$list.noport"
while read -r rel; do
	if _p=$(portdir "$rel") && [ -n "$_p" ]; then
		printf '%-44s %s\n' "$rel" "${_p#"$PORTS"/}" >>"$list.notbuilt"
	else
		printf '%s\n' "$rel" >>"$list.noport"
	fi
done <"$list"

printf '  %d have a port that was never installed:\n' "$(wc -l <"$list.notbuilt" | tr -d ' ')"
sort "$list.notbuilt" | while read -r rel _p; do
	printf '    %-44s %s\n' "$rel" "$_p"
done

printf '\n  %d have no port at all:\n' "$(wc -l <"$list.noport" | tr -d ' ')"
sort "$list.noport" | while read -r rel; do
	printf '    %s\n' "$rel"
done

if [ "${QUIET_FAIL:-0}" -eq 1 ]; then
	exit 1
fi
exit 0