# FreeLinX/ports - textproc/yq : yq 4.44.3 (command-line YAML processor).
#
# MIT-licensed portable command-line YAML, JSON, XML, CSV, TOML processor.
# This is a Go project: like security/age, it requires a Go toolchain on
# the build host.  FreeLinX does not yet ship its own Go compiler, so this
# port is currently a host-built bootstrap port.  Once a FreeLinX Go
# toolchain is available this port will be updated to cross-build properly.
#
# BUILD_DEPENDENCIES notes the host Go toolchain requirement explicitly.

NAME    := yq
VERSION := 4.44.3
CATEGORY:= textproc
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= Go toolchain (host/upstream)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= yq
INSTALL_RELPATH:= usr/bin/yq

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/yq-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/yq
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	make build
