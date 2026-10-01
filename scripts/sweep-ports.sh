#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Copyright (c) 2026 FreeLinX OS Project.
#
# scripts/sweep-ports.sh - build and install every port named on stdin.
#
# Exists because scripts/check-port-coverage.sh reports which binaries in the
# rootfs no port has been installed for, and that list is long enough that
# running it by hand one port at a time is how it stays long.  This walks it,
# records the first line of the error for the ones that fail, and does not stop
# on the first failure -- a single broken port says nothing about the other two
# hundred.
#
# Ports are visited in dependency order: a port's DEPENDENCIES are emitted
# before it.  That matters because a port whose headers or .pc files are missing
# fails in a way that looks like a bug in the port rather than an ordering
# mistake, and those failures cost more to read than the ordering does to get.
#
# Usage:  sh scripts/sweep-ports.sh < portlist.txt
#         sh scripts/sweep-ports.sh base/banner www/w3m
#
# Environment:
#   RESULT      where the per-port results are written (default build/sweep.txt)
#   TIMEOUT     seconds per port (default 1800)
#   NOINSTALL   set to stage only, not to install into the rootfs

set -u

HERE=$(cd "$(dirname "$0")" && pwd)
PORTS=$(cd "$HERE/.." && pwd)

RESULT=${RESULT:-$PORTS/build/sweep.txt}
TIMEOUT=${TIMEOUT:-1800}
LOGDIR=${LOGDIR:-$PORTS/build/sweep-log}

if [ "$#" -gt 0 ]; then
	list=$*
else
	list=cat
fi

mkdir -p "$LOGDIR"
: >"$RESULT"

printf 'sweeping ports\n'
printf '  ports   %s\n' "$PORTS"
printf '  logs    %s\n' "$LOGDIR"
printf '  results %s\n\n' "$RESULT"

# --- dependency order --------------------------------------------------------

deps_of() {
	sed -n 's/^DEPENDENCIES[[:space:]]*[:?]\?= *//p' "$1/Makefile" 2>/dev/null |
		head -1 | tr ',' ' '
}

# Ports are named the way the tree names them (base/banner), and a dependency may
# be named either way (banner, or base/banner).  Resolve a bare name against the
# category directories, which is the same list scripts/install.sh uses.
find_port() {
	_n=$1
	case $_n in
	*/*) [ -d "$PORTS/$_n" ] && printf '%s' "$_n" && return 0 ;;
	esac
	for _c in base devel sysutils www x11 archivers editors shells security \
	    drivers fonts graphics libs multimedia net mail servers utils \
	    kernel xorg; do
		[ -d "$PORTS/$_c/$_n" ] && printf '%s/%s' "$_c" "$_n" && return 0
	done
	return 1
}

emit() {
	case " $SEEN " in
	*" $1 "*) return 0 ;;
	esac
	SEEN="$SEEN $1"
	if [ -f "$PORTS/$1/Makefile" ]; then
		for _d in $(deps_of "$1"); do
			[ -n "$_d" ] || continue
			_dp=$(find_port "$_d") || continue
			emit "$_dp"
		done
	fi
	printf '%s\n' "$1"
}

# Recursion in POSIX sh needs no `local`, and a global stack is what avoids the
# 1000-deep argv limit on a 200-port list.
SEEN=' '

# --- the walk ----------------------------------------------------------------

ok=0
failed=0
skipped=0
n=0

# printf '%s\n' $list unquoted, deliberately: the argument list is one string by
# the time it is in a variable, and "$list | tr" then asks the shell to run the
# whole thing as a program name, which is
#
#   sweep-ports.sh: line 105: base/apply: Is a directory
#
# printf '%s\n' $list word-splits it first, which is what is wanted here and
# nowhere else in this script.
printf '%s\n' $list | tr ' ' '\n' | sed '/^$/d' | while read -r _p; do
	_p=$(find_port "$_p") || continue
	emit "$_p"
done >"$LOGDIR/.order"

total=$(wc -l <"$LOGDIR/.order")
printf '  %s ports, in dependency order\n\n' "$total"

while read -r p <&3; do
	[ -n "$p" ] || continue
	n=$((n + 1))
	log="$LOGDIR/${p%/*}-${p#*/}.log"
	if [ ! -f "$PORTS/$p/Makefile" ]; then
		printf '  [%3d/%3d] %-28s SKIP (no Makefile)\n' "$n" "$total" "$p"
		skipped=$((skipped + 1))
		continue
	fi
	printf '  [%3d/%3d] %-28s ' "$n" "$total" "$p"
	if [ "${NOINSTALL:-0}" = 1 ]; then
		if timeout "$TIMEOUT" "$PORTS/scripts/build.sh" "$p" >"$log" 2>&1; then
			printf 'ok\n'
			ok=$((ok + 1))
			printf 'ok\t%s\n' "$p" >>"$RESULT"
			continue
		fi
	else
		if timeout "$TIMEOUT" "$PORTS/scripts/install.sh" -r "$p" >"$log" 2>&1; then
			printf 'ok\n'
			ok=$((ok + 1))
			printf 'ok\t%s\n' "$p" >>"$RESULT"
			continue
		fi
	fi
	why=$(grep -m1 -E 'Error [0-9]+|error\]|No rule to make' "$log" |
		sed 's/^[[:space:]]*//' | cut -c1-90)
	printf 'FAIL\n'
	printf '         %s\n' "${why:-(no error line; see $log)}"
	failed=$((failed + 1))
	printf 'fail\t%s\t%s\n' "$p" "${why:-(no error line)}" >>"$RESULT"
done 3<"$LOGDIR/.order"

# Counted with awk, not `grep -c ... || echo 0`: grep -c prints 0 *and* exits 1
# when it finds nothing, so the `||` appends a second 0 and the variable holds
# "0\n0", which printf then rejects as not a number and `[ -ne ]` rejects as not
# an integer.  That is two errors reported for a sweep that worked.
ok=$(awk -F'\t' '$1 == "ok" { n++ } END { print n + 0 }' "$RESULT")
failed=$(awk -F'\t' '$1 == "fail" { n++ } END { print n + 0 }' "$RESULT")

printf '\n%d ok, %d failed of %d\n' "$ok" "$failed" "$total"
if [ "$failed" -ne 0 ]; then
	printf 'the failures are in %s, one log per port\n' "$RESULT"
	exit 1
fi
exit 0