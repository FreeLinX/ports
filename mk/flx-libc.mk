# -*- makefile -*-
# FreeLinX/ports - mk/flx-libc.mk : bundles of NetBSD libc sources that musl
# lacks, hosted in base/compat (+ base/compat/db).  A base port that needs
# dbopen(3), cgetnext(3), strvis(3) etc. appends the matching _SRCS group to
# its COMPAT_SRCS; every source is compiled with the shared FLX_CPPFLAGS and
# linked statically into that port's binary.

FLX_LIBC   := $(FREELINX_PORTS_ROOT)/base/compat

FLX_DB_SRCS := \
  $(FLX_LIBC)/db/db/db.c \
  $(FLX_LIBC)/db/db/dbfile.c \
  $(FLX_LIBC)/db/btree/bt_close.c \
  $(FLX_LIBC)/db/btree/bt_conv.c \
  $(FLX_LIBC)/db/btree/bt_debug.c \
  $(FLX_LIBC)/db/btree/bt_delete.c \
  $(FLX_LIBC)/db/btree/bt_get.c \
  $(FLX_LIBC)/db/btree/bt_open.c \
  $(FLX_LIBC)/db/btree/bt_overflow.c \
  $(FLX_LIBC)/db/btree/bt_page.c \
  $(FLX_LIBC)/db/btree/bt_put.c \
  $(FLX_LIBC)/db/btree/bt_search.c \
  $(FLX_LIBC)/db/btree/bt_seq.c \
  $(FLX_LIBC)/db/btree/bt_split.c \
  $(FLX_LIBC)/db/btree/bt_utils.c \
  $(FLX_LIBC)/db/hash/hash.c \
  $(FLX_LIBC)/db/hash/hash_bigkey.c \
  $(FLX_LIBC)/db/hash/hash_buf.c \
  $(FLX_LIBC)/db/hash/hash_func.c \
  $(FLX_LIBC)/db/hash/hash_log2.c \
  $(FLX_LIBC)/db/hash/hash_page.c \
  $(FLX_LIBC)/db/mpool/mpool.c \
  $(FLX_LIBC)/db/recno/rec_close.c \
  $(FLX_LIBC)/db/recno/rec_delete.c \
  $(FLX_LIBC)/db/recno/rec_get.c \
  $(FLX_LIBC)/db/recno/rec_open.c \
  $(FLX_LIBC)/db/recno/rec_put.c \
  $(FLX_LIBC)/db/recno/rec_search.c \
  $(FLX_LIBC)/db/recno/rec_seq.c \
  $(FLX_LIBC)/db/recno/rec_utils.c

FLX_VIS_SRCS         := $(FLX_LIBC)/vis.c $(FLX_LIBC)/unvis.c
FLX_GETCAP_SRCS      := $(FLX_LIBC)/getcap.c
FLX_FTS_SRCS         := $(FLX_LIBC)/fts.c
FLX_STRTOI_SRCS      := $(FLX_LIBC)/strtoi.c
FLX_SIGSTUB_SRCS     := $(FLX_LIBC)/sigstub.c
FLX_REALLOCARR_SRCS  := $(FLX_LIBC)/reallocarr.c
FLX_UTMPENTRY_SRCS   := $(FLX_LIBC)/utmpentry.c
FLX_STRCPYTRICK_SRCS := $(FLX_LIBC)/strsuftoll.c
FLX_FPARSE_SRCS      := $(FLX_LIBC)/fparseln.c
# libterminfo: NetBSD terminfo/termcap library vendored from src.tgz.
# Used by tic/infocmp/tput/tset/ul.  Sources define term.h + term_private.h
# (private header used by the toolchain ports), so per-port FLX_CPPFLAGS must
# carry FLX_TERMINFO_CPPFLAGS before any ncurses include path.
FLX_TERMINFO     := $(FLX_LIBC)/terminfo/usr/src/lib/libterminfo
FLX_TERMINFO_CPPFLAGS = -I$(FLX_TERMINFO) -DTERMINFO_COMPILE -DTERMINFO_DB -DTERMINFO_COMPAT
FLX_LTERMINFO_SRCS := \
  $(FLX_TERMINFO)/compile.c \
  $(FLX_TERMINFO)/curterm.c \
  $(FLX_TERMINFO)/setupterm.c \
  $(FLX_TERMINFO)/term.c \
  $(FLX_TERMINFO)/termcap.c \
  $(FLX_TERMINFO)/hash.c \
  $(FLX_TERMINFO)/ti.c \
  $(FLX_TERMINFO)/tparm.c \
  $(FLX_TERMINFO)/tputs.c \
  $(FLX_TERMINFO)/cdbr.c \
  $(FLX_TERMINFO)/cdbw.c \
  $(FLX_LIBC)/mi_vector_hash.c \
  $(FLX_LIBC)/fast_divide.c
