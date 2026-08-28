# -*- makefile -*-
# FreeLinX/ports - shared template for the simple base utilities.
#
# A single-source-file NetBSD utility (cat, echo, mkdir, cp, mv, rm) only sets
# metadata + SRC_DIR/BUILD_BIN and includes this file. It avoids redefining
# do-build, so there is no target-override noise and one source of logic.
#
# The build recipe is intentionally conservative: it only proceeds if the
# toolchain gate passed AND the source file is actually staged; otherwise it
# reports the blocker instead of pretending.

# This file lives at <root>/mk/base-port.mk. MAKEFILE_LIST's last entry is this
# file when the line below is read, so the repo root is one level up from its
# own directory.
FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

include $(FREELINX_PORTS_ROOT)/mk/port.mk

# The .c file to compile (relative to SRC_DIR; override if layout differs).
SRC_FILE?=$(SRC_DIR)/$(NAME).c

.PHONY: do-build
do-build:
	@printf '[FreeLinX/ports] Building $(NAME)...\n'
	@if [ -f "$(SRC_FILE)" ]; then \
	    if $(CC) $(FREELINX_CFLAGS) -o $(BUILD_BIN) "$(SRC_FILE)" $(FREELINX_LDFLAGS); then \
	        printf '[FreeLinX/ports] $(NAME): built $(BUILD_BIN)\n'; \
	    else \
	        printf '[FreeLinX/ports][error] $(NAME): compile/link failed\n'; \
	        exit 1; \
	    fi; \
	else \
	    printf '[FreeLinX/ports][error] $(NAME): source not staged at $(SRC_FILE)\n'; \
	    printf '[FreeLinX/ports][error] fetch the NetBSD source and unpack it there first\n'; \
	    exit 1; \
	fi
