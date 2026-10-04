
include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/w3m-0.5.3-git20230121
PROJECT_BIN = $(SRC_TREE)/w3m


# Three steps.
#
# 1. A host libgc and a host mktable, in $(NATIVE).  The four sources are the
#    four Makefile.in names for mktable's objects, minus dummy.o, which is the
#    empty library stub upstream links in and has nothing to compile.  config.h
#    has to exist for them, which is why this runs after the configure above.
#
# 2. The tables, with make's own recipes, run with -o mktable.
#
#    -o is what makes this possible.  Makefile.in's rule is
#
#      mktable$(EXT): mktable.o dummy.o Str.o hash.o myctype.o
#
#    and dummy.o is unbuildable: there is no dummy.c in the tree, no rule for it
#    in the generated Makefile, and nothing else produces it.  It is a name
#    automake reserves for the object of an empty convenience library, left in
#    the rule after the library went away.  So asking make for functable.c
#    without -o makes it try to build mktable, which cannot succeed, and fails
#    with
#
#      make[1]: *** [Makefile:182: mktable] Error 1
#
#    -o mktable tells make the target is up to date, which is true: it was just
#    built, by the one compiler whose output this machine can run.
#
#    The Makefile's funcname.tab rule calls awk, sort and cmp by bare name
#    through PATH rather than through $(AWK), so there is nothing to override
#    there either.  Those are host programs on this host and were on every host
#    that has built w3m; none of them produces an object file.
#
# 3. w3m itself, cross.
BUILD_CMDS = \
