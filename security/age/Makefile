# FreeLinX/ports - security/age : age 1.2.0 (modern file encryption).
#
# BSD-3-Clause licensed simple, modern, and secure file encryption tool.
# This is a Go project: the upstream build uses `go build` and requires
# a Go toolchain on the build host.  FreeLinX does not yet ship its own
# Go toolchain, so this port is currently a host-built bootstrap port.
# Once a FreeLinX Go compiler is available, this port will be updated to
# cross-build properly.
#
# BUILD_DEPENDENCIES notes the host Go toolchain requirement explicitly.

NAME    := age
VERSION := 1.2.0
CATEGORY:= security
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= Go toolchain (host/upstream)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= age
INSTALL_RELPATH:= usr/bin/age

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/age-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/age
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	make build
