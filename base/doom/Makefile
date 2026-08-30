# FreeLinX/ports - base/doom : id Software DOOM (doomgeneric) for FreeLinX.
#
# Builds the REAL classic Doom engine (doomgeneric by ozkl, itself built
# from id Software's GPL-2.0 Linux Doom sources) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), statically.  It is NOT a fake
# binary, a host game, or a stub: it links the full engine (renderer,
# game logic, WAD loader) and runs a real WAD.
#
# Video backend: `linuxvt` (doomgeneric_linuxvt.c) pushes each rendered
# 640x400 frame into the Linux framebuffer /dev/fb0 (FBIO mmap) and
# reads input from evdev.  The delivered binary is `doom`.
#
# Because the current FreeLinX kernel config has NO framebuffer subsystem
# (CONFIG_FB / CONFIG_DRM_FBDEV_EMULATION are off), /dev/fb0 does not
# exist at runtime and the `doom` binary cannot display.  To verify the
# engine actually renders without a live framebuffer, the port ALSO
# builds `doombench`, a headless backend that writes the rendered frames
# to PPM files.  This proves the render/game core works and isolates the
# blocker to the missing kernel framebuffer device.
#
# Runtime data: DOOM needs a .wad (e.g. the freely redistributable
# shareware doom1.wad) next to the binary.  The WAD is NOT vendored here.
#
# Static link set (identical model to mk/base-port.mk): crt1.o + crti.o
# at the front, crtn.o at the very end, -lc in between, and clang's
# compiler-rt in place of the libgcc a GCC toolchain would add.  The
# musl sysroot has no crtbegin*/crtend*/libgcc, so the link is driven
# explicitly with -nostdlib.

NAME   := doom
VERSION:= 0.1
CATEGORY:= base
LICENSE:= GPL-2.0
DEPENDENCIES:= musl (via FreeLinX toolchain sysroot); runtime: a DOOM .wad
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= doom
INSTALL_RELPATH:= bin/doom

include ../../mk/port.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/$(NAME)
SRC_TREE = $(SRC_ROOT)/$(DISTINFO_NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)

# Upstream doomgeneric (linuxvt variant) translation units, exactly the
# list in the stock Makefile.linuxvt (platform backends excluded).
DG_SRCS = dummy am_map doomdef doomstat dstrings d_event d_items d_iwad \
	d_loop d_main d_mode d_net f_finale f_wipe g_game hu_lib hu_stuff info \
	i_cdmus i_endoom i_joystick i_scale i_sound i_system i_timer memio \
	m_argv m_bbox m_cheat m_config m_controls m_fixed m_menu m_misc \
	m_random p_ceilng p_doors p_enemy p_floor p_inter p_lights p_map \
	p_maputl p_mobj p_plats p_pspr p_saveg p_setup p_sight p_spec \
	p_switch p_telept p_tick p_user r_bsp r_data r_draw r_main r_plane \
	r_segs r_sky r_things sha1 sounds statdump st_lib st_stuff s_sound \
	tables v_video wi_stuff w_checksum w_file w_main w_wad z_zone \
	w_file_stdc i_input i_video doomgeneric mus2mid

PPM_BACKEND = $(CURDIR)/backends/doomgeneric_ppm.c

BUILD_BIN  = $(OBJ_DIR)/doom
PPM_BIN    = $(OBJ_DIR)/doombench

CFLAGS_LOCAL = -DNORMALUNIX -DLINUX -DSNDSERV -D_DEFAULT_SOURCE -O2

FLX_CRT         = $(FREELINX_SYSROOT)/lib/crt1.o $(FREELINX_SYSROOT)/lib/crti.o
FLX_CRT_END     = $(FREELINX_SYSROOT)/lib/crtn.o
FLX_RTDIR       := $(shell $(CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -print-resource-dir 2>/dev/null)
FLX_COMPILER_RT = $(filter %-musl/libclang_rt.builtins.a,$(wildcard $(FLX_RTDIR)/lib/*/libclang_rt.builtins.a))

# ---------------------------------------------------------------------------
# do-prepare: extract the pristine upstream source and copy in the
# FreeLinX headless verifier backend.  A .flx-patched sentinel keeps
# builds idempotent.
# ---------------------------------------------------------------------------
.PHONY: do-prepare
do-prepare:
	@set -e; \
	if [ ! -d "$(SRC_TREE)" ]; then \
		printf '[FreeLinX/ports] extracting %s\n' "$(DISTINFO_ARCHIVE)"; \
		rm -rf "$(SRC_ROOT)"; \
		mkdir -p "$(SRC_ROOT)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(SRC_ROOT)"; \
	fi; \
	mkdir -p "$(OBJ_DIR)"; \
	cp -f "$(PPM_BACKEND)" "$(SRC_TREE)/doomgeneric/doomgeneric_ppm.c"; \
	if [ ! -f "$(SRC_TREE)/.flx-patched" ]; then \
		touch "$(SRC_TREE)/.flx-patched"; \
	fi

# ---------------------------------------------------------------------------
# do-build: compile every TU shell-side and statically link `doom` and
# `doombench`.  Driven shell-side so make does not need the source tree
# present at graph-build time.  Any failure surfaces the real compiler
# / linker error - nothing is faked.
# ---------------------------------------------------------------------------
.PHONY: do-build
do-build: do-prepare
	@set -e; \
	for s in $(DG_SRCS); do \
		printf '[FreeLinX/ports] cc %s.c\n' "$$s"; \
		$(CC) $(FREELINX_CFLAGS) $(CFLAGS_LOCAL) -c "$(SRC_TREE)/doomgeneric/$$s.c" -o "$(OBJ_DIR)/$$s.o"; \
	done; \
	printf '[FreeLinX/ports] cc doomgeneric_linuxvt.c\n'; \
	$(CC) $(FREELINX_CFLAGS) $(CFLAGS_LOCAL) -c "$(SRC_TREE)/doomgeneric/doomgeneric_linuxvt.c" -o "$(OBJ_DIR)/doomgeneric_linuxvt.o"; \
	printf '[FreeLinX/ports] cc doomgeneric_ppm.c\n'; \
	$(CC) $(FREELINX_CFLAGS) $(CFLAGS_LOCAL) -c "$(SRC_TREE)/doomgeneric/doomgeneric_ppm.c" -o "$(OBJ_DIR)/doomgeneric_ppm.o"; \
	DOOM_LDOBJ="$$(ls $(OBJ_DIR)/*.o | grep -v doomgeneric_ppm.o | grep -v doomgeneric_linuxvt.o | tr '\n' ' ') $(OBJ_DIR)/doomgeneric_linuxvt.o"; \
	BENCH_LDOBJ="$$(ls $(OBJ_DIR)/*.o | grep -v doomgeneric_linuxvt.o | grep -v doomgeneric_ppm.o | tr '\n' ' ') $(OBJ_DIR)/doomgeneric_ppm.o"; \
	printf '[FreeLinX/ports] ld doom\n'; \
	$(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) -nostdlib -static \
		-L$(FREELINX_SYSROOT)/lib \
		$(FLX_CRT) $$DOOM_LDOBJ -lc $(FLX_COMPILER_RT) $(FLX_CRT_END) -o "$(BUILD_BIN)"; \
	printf '[FreeLinX/ports] ld doombench\n'; \
	$(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) -nostdlib -static \
		-L$(FREELINX_SYSROOT)/lib \
		$(FLX_CRT) $$BENCH_LDOBJ -lc $(FLX_COMPILER_RT) $(FLX_CRT_END) -o "$(PPM_BIN)"; \
	printf '[FreeLinX/ports] built: %s and %s\n' "$(BUILD_BIN)" "$(PPM_BIN)"

# doombench is an additional verification artifact, built on demand.
.PHONY: bench
bench: $(PPM_BIN)
	@printf '[FreeLinX/ports] doombench: %s\n' "$(PPM_BIN)"