#!/bin/sh
set -e
make defconfig
for c in ROUTE GETTY PASSWD FDISK FSCK MODPROBE; do
	sed -i "s/^# CONFIG_$c is not set/CONFIG_$c=y/" .config
done