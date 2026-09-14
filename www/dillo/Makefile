# FreeLinX/ports - www/dillo : Dillo lightweight graphical web browser (3.1.1).
#
# Ultra-fast, lightweight graphical web browser with FLTK GUI, tabs, cookies,
# SSL/TLS (OpenSSL), and image support (PNG/JPEG/GIF). 100% non-GNU static port.

NAME    := dillo
VERSION := 3.1.1
CATEGORY:= www
LICENSE := GPLv3
DEPENDENCIES:= fltk openssl libpng jpeg zlib
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dillo
INSTALL_RELPATH:= usr/bin

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/dillo-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/dillo

PROJECT_CFG_CMDS = \
	echo "Dillo already configured"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	make -j"$(FREELINX_JOBS)"
