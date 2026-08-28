# FreeLinX/ports - base/ls : NetBSD `ls`.
NAME   := ls
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:=
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

include ../../mk/port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# TODO(FRAMEWORK): see base/cat/Makefile. ls is multi-file (bing.c, print.c,
# util.c) and uses fts(); compiling it is a genuine porting task later.

.PHONY: do-build
do-build:
	@printf '[FreeLinX/ports] Building $(NAME)...\n'
	@if [ -d "$(SRC_DIR)/bin/ls" ]; then \
	    printf '[FreeLinX/ports][error] $(NAME): multi-file port; recipe not yet written\n'; \
	    exit 1; \
	else \
	    printf '[FreeLinX/ports][error] $(NAME): source not staged; fetch and unpack to $(SRC_DIR) first\n'; \
	    exit 1; \
	fi
