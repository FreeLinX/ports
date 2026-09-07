# FreeLinX/ports - shells/mksh : MirBSD Korn shell R59c.
#
# mksh is the MirBSD Korn shell (public domain / permissive MirBSD licence).
# A single "Build.sh" drives its own configure (mirtoconf) + build; the
# resulting shell is static musl.  We chose the .tgz distribution because the
# do-extract recipe in mk/project-port.mk handles tar.gz/tgz (the R60+
# .cpio.gz releases are not tar-extractable by this framework).
#
# Build strategy: run upstream's own Build.sh with the FreeLinX clang + LLD +
# musl toolchain exported via the project-port env (CC/CFLAGS/LDFLAGS carry
# --target/--sysroot/--rtlib=compiler-rt/-static/-fuse-ld=lld).  mirtoconf
# compiles its probes with CC and the F_LINX flags; the shell links against
# the musl sysroot only.  No C++ runtime is wanted, so FLX_PROJECT_LIBS is
# emptied before the framework is included.

NAME   := mksh
VERSION:= R59c
CATEGORY:= shells
LICENSE:= MirBSD (BSD-like)
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mksh
INSTALL_RELPATH:= bin/mksh

# mksh is pure C: do not drag the project-port C++ library set (libc++ etc.)
# into CFLAGS/LDFLAGS.
FLX_PROJECT_LIBS:=
FLX_PROJECT_LDFLAGS:=

include ../../mk/project-port.mk

# The mksh-R59c.tgz extracts to a top-level `mksh/` directory.
SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mksh
PROJECT_BIN = $(SRC_TREE)/mksh

# Build.sh performs configure + compile + link itself; no separate config step.
PROJECT_CFG_CMDS = :

BUILD_CMDS = sh ./Build.sh