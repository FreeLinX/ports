# FreeLinX/ports - mk/stage-pc.mk : stage a generated pkg-config file.
#
# $(call flx-stage-pc,PORT_DEPS_DIR,PC_BASENAME) installs <name>.pc into
# $(FREELINX_BUILD_DIR)/deps/<port>/lib/pkgconfig/ and fails the install if it
# cannot find it.
#
# It exists because almost every X.Org port built a .pc, staged nothing, and
# looked finished: the port compiles, links its own test program, and the
# failure lands on the *consumer* instead, as
#
#     Package 'xau' not found
#     Package requirements (ice >= 1.1.0 xproto xtrans) were not met
#     Package requirements (freetype2 >= 21.0.15) were not met
#
# All three name the consumer's own requirement, not the file that is missing,
# and the last two are indistinguishable from a version problem.  Twelve of
# these ports were in that state.
#
# The module name is rarely the library's name, so the caller passes it:
# libXau generates xau.pc, libICE ice.pc, libXt xt.pc, libXaw xaw7.pc, and
# libxcvt -- the other way round -- libxcvt.pc.
#
# Where the file lands depends on the build system, and a port that guessed
# wrong failed only at the consumer's configure, so the whole source tree is
# searched rather than a list of expected directories.  Four levels is enough
# for every layout in this tree: an in-tree autotools build writes it beside
# the sources or in src/, cmake writes it in its build directory, and meson
# writes it in meson-private/ under whatever build directory it was given.
# scripts/check-deps-pkgconfig.sh checks the whole tree for the result.
define flx-stage-pc
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/$(1)/lib/pkgconfig"
	@_pc=$$(find "$(SRC_TREE)" -maxdepth 4 -name '$(2).pc' | head -1); \
	if [ -z "$$_pc" ]; then \
		printf '[FreeLinX/ports][error] %s: no $(2).pc was generated under %s\n' "$(NAME)" "$(SRC_TREE)" >&2; \
		exit 1; \
	fi; \
	$(INSTALL) -m 644 "$$_pc" "$(FREELINX_BUILD_DIR)/deps/$(1)/lib/pkgconfig/$(or $(3),$(2)).pc"
endef
