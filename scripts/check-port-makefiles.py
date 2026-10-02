#!/usr/bin/env python3
"""check-port-mkfiles.py [--fix]

Report (and optionally repair) four mistakes that keep recurring in this tree
and that make make fail in ways which name neither the file nor the variable:
  1. FREELINUX_ where FREELINX_ was meant.  Every FREELINX_* variable is
     undefined, so the line expands to nothing: CC= becomes empty and configure
     reports "C compiler cannot create executables"; -j"" makes ninja abort.

  2. PREFIX_<dep> used in a flag list but never assigned.  Reported
     only: the repair would have to guess the prefix, and see below.  There is no
     PREFIX_expat, so -I$(PREFIX_expat)/include becomes -I/include, which is
     silently wrong rather than an error -- the build then stops at the next
     "file not found" and blames the header.

  3. Two compiler flags sharing one element of a meson *_args list.  Meson hands
     each element to the compiler as a single argument, so
     '$(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS)' arrives as one
     argument with a space inside it, --sysroot is swallowed, and the build
     quietly compiles against the host's glibc headers.  The symptom is
     "fatal error: 'stdio.h' file not found" in a sysroot that has stdio.h.
     Repaired, because splitting on whitespace is exactly right here: both
     variables are already single flags.

  4. A port whose distinfo does not spell its tarball's root directory.  SRC_TREE
     defaults to work/$(NAME)/$(DISTINFO_NAME), and DISTINFO_NAME defaults to
     $(NAME), so a distinfo that spells the versioned tarball root -- which is
     what the framework needs -- while naming neither the port nor the version
     points every recipe at a directory that was never extracted, and the build
     stops at "cd: .../foo-1.2.3: No such file or directory".  Reported with the
     override that fixes it, never applied: only the tarball knows the answer.

  5. A host include or library directory on a compile or link line, whether
     spelled out (-L/usr/lib) or arrived at through a variable that expands
     there (-L$(FREELINX_STAGING_DIR)/usr/lib).  The sysroot puts musl's libc
     in the sysroot, but -L/usr/lib is searched first and /usr/lib/libc.a on a
     glibc build host is a real file, so the link picks glibc's static libc and
     stops on
       ld.lld: error: undefined symbol: __gcc_personality_v0
       >>> referenced by iofputs.o:(_IO_fputs.cold) in archive /usr/lib/libc.a
     or, on a host without it, quietly compiles glibc's headers.  Either way
     the port is no longer a musl port.  Install destinations are fine and are
     not matched: --prefix=/usr, --libdir=/usr/lib and --tccdir=/usr/lib/tcc
     name where to write, not where to look.

Run from the ports root.
"""

import glob
import os
import re
import shlex
import subprocess
import sys

BAD = "FREEL" + "INUX_"      # F R E E L I N U X _
GOOD = "FREEL" + "INX_"     # F R E E L I N X _

# A *_args = ['a', 'b c'] line: one element of the list holds two flags.
MESON_ARGS = re.compile(r"^(\s*)(\w*_args)(\s*=\s*\[)(.*)(\])")


def split_meson_args(match):
    """One list element per flag, so meson hands each flag to the compiler."""
    indent, name, eq, body, close = match.groups()
    elements = re.findall(r"'[^']*'", body)
    out = []
    for e in elements:
        out.extend("'%s'" % w for w in e[1:-1].split())
    return "%s%s%s%s%s" % (indent, name, eq, ", ".join(out), close)


def any_assigns_prefix(text, portdir):
    """True if an included mk assigns PREFIX_ names with $(eval) from a list.

    mk/xorg-lib.mk does exactly that, one per name in XORG_LIB_DEPS.  The eval
    is invisible to a regex, so a port that includes such a file gets the
    benefit of the doubt rather than a dozen reports of names it does have.
    """
    for inc in re.findall(r"^\s*include\s+(\S+)", text, re.M):
        path = os.path.normpath(os.path.join(portdir, inc))
        if os.path.isfile(path) and "PREFIX_$(_d)" in open(path).read():
            return True
    return False


fix = "--fix" in sys.argv
problems = []

for pat in ("*/Makefile", "*/*/Makefile", "mk/*.mk"):
    for f in sorted(glob.glob(pat)):
        if "/build/" in f:
            continue
        text = open(f).read()
        lines = text.split("\n")

        if BAD in text:
            problems.append((f, f"{BAD} misspelled (should be {GOOD})"))
            if fix:
                open(f, "w").write(text.replace(BAD, GOOD))
                text = open(f).read()
                lines = text.split("\n")

        for i, l in enumerate(lines):
            m = MESON_ARGS.match(l)
            if not m:
                continue
            elements = re.findall(r"'[^']*'", m.group(4))
            if not any(" " in e[1:-1] for e in elements):
                continue
            problems.append((f, "line %d: two flags in one meson %s element" % (i + 1, m.group(2))))
            if fix:
                lines[i] = MESON_ARGS.sub(split_meson_args, l)
        if fix:
            open(f, "w").write("\n".join(lines))
            text = open(f).read()
            lines = text.split("\n")

        used = set(re.findall(r"\bPREFIX_(\w+)", text))
        defined = {
            l.split("=", 1)[0].strip()[len("PREFIX_"):]
            for l in lines
            if l.startswith("PREFIX_") and "=" in l
        }
        # A port may get its PREFIX_ lines from an included mk that assigns them
        # with $(eval) from a list -- mk/xorg-lib.mk does exactly that for each
        # name in XORG_LIB_DEPS.  The eval is not visible here, so take the
        # includes at their word rather than report a dozen phantom gaps.
        if not any_assigns_prefix(text, os.path.dirname(f) or "."):
                for d in sorted(used - defined):
                    problems.append((f, f"PREFIX_{d} used but never assigned"))
                    if fix:
                        at = [i for i, l in enumerate(lines) if l.startswith("PREFIX_")]
                        # Nothing to anchor to -- put them above the include,
                        # which is where every other declaration lives.
                        if not at:
                            at = [next(i for i, l in enumerate(lines)
                                       if l.startswith("include "))]
                        lines.insert(max(at) + 1, f"PREFIX_{d} = $(FREELINUX_BUILD_DIR)/deps/{d}")
                        open(f, "w").write("\n".join(lines))
                        lines = open(f).read().split("\n")
                        # The value just written may itself carry the misspelling.
                        s = "\n".join(lines)
                        if BAD in s:
                            open(f, "w").write(s.replace(BAD, GOOD))

        # Does the port's own SRC_TREE agree with what the tarball unpacks to?
        if re.search(r"^SRC_TREE\s*[:?+]?=", text, re.M):
            continue
        # SRC_TREE defaults to $(SRC_DIR)/$(DISTINFO_NAME), and DISTINFO_NAME
        # defaults to $(NAME).  Either the distinfo or the Makefile may set it.
        distinfo = os.path.join(os.path.dirname(f), "distinfo")
        if not os.path.exists(distinfo):
            continue
        dinfo = open(distinfo).read()
        d = re.search(r"^DISTINFO_ARCHIVE\s*[:?+]?=\s*(\S+)", dinfo, re.M)
        n = re.search(r"^DISTINFO_NAME\s*[:?+]?=\s*(\S+)", dinfo, re.M) \
            or re.search(r"^DISTINFO_NAME\s*[:?+]?=\s*(\S+)", text, re.M)
        if not (d and n):
            continue
        tarball = os.path.join("dist", d.group(1))
        if not os.path.exists(tarball):
            continue
        # Only ports that let the framework do the extracting.  Several roll
        # their own with a different variable -- runit unpacks to admin/runit/,
        # the NetBSD ports to usr/ -- and are then their own authority.
        if "SRC_TREE" not in text:
            continue
        # Only the first entries are read.  One is enough to name the archive's
        # top directory, which is the case that actually bites, and a full
        # listing of a 1 GB tarball makes this lint slower than the builds.  The
        # head is in the pipeline so tar stops reading, not just stops storing.
        head = subprocess.run(
            "tar -tf %s 2>/dev/null | head -64" % shlex.quote(tarball),
            shell=True, stdout=subprocess.PIPE, text=True,
        ).stdout.split("\n")
        roots = sorted({l.split("/")[0] for l in head if "/" in l})
        if n.group(1) in roots:
            continue
        problems.append((f, "tarball unpacks to %s, not %r: set DISTINFO_NAME=%s "
                            "or SRC_TREE = $(SRC_DIR)/%s"
                         % (", ".join(roots[:3]) or "nothing", n.group(1),
                            n.group(1), roots[0] if roots else "?")))

# --- does a linked archive exist? -------------------------------------------
# A port names its dependency archives by path.  When the producing port stages
# them somewhere else, the link fails with a message that says nothing about the
# port: base/bdes linked
#
#   FLX_LDADD += $(OPENSSL_PREFIX)/lib64/libcrypto.a
#
# and devel/openssl stages
#
#   $(INSTALL) -m 644 "$(PROJECT_LIB)" "$(OPENSSL_PREFIX)/lib/libcrypto.a"
#
# so the build died on
#
#   clang: error: no such file or directory:
#       '.../build/deps/openssl/lib64/libcrypto.a'
#
# Five ports had it.  The check is only as good as its ability to resolve the
# variable, so it handles the one shape the tree uses -- NAME = $(...)/deps/NAME
# -- and stays quiet about anything else rather than guessing.
LIB_REF = re.compile(r"\$\(([A-Z_][A-Z0-9_]*)\)(/[\w./+-]+/)([\w.+-]+\.a)\b")
PREFIX_DEF = re.compile(r"^([A-Z_][A-Z0-9_]*)\s*[:?+]?=\s*\$\([^)]*\)/deps/([\w.+-]+)/?\s*$", re.M)

for pat in ("*/Makefile", "*/*/Makefile"):
    for f in sorted(glob.glob(pat)):
        if "/build/" in f:
            continue
        text = open(f).read()
        prefixes = dict(PREFIX_DEF.findall(text))
        if not prefixes:
            continue
        for i, l in enumerate(text.split("\n")):
            if l.lstrip().startswith("#"):
                continue
            for var, dirs, lib in LIB_REF.findall(l):
                if var not in prefixes:
                    continue
                dep = os.path.join("build", "deps", prefixes[var])
                if not os.path.isdir(dep):
                    # The dependency has not been built, so there is nothing to
                    # check against and saying so would be noise.
                    continue
                # dirs starts with "/", and os.path.join(dep, "/lib/", lib)
                # throws dep away -- which made the report read "links
                # /lib/libssl.a but openssl provides lib/libssl.a", naming the
                # very path that exists.
                want = os.path.join(dep, dirs.lstrip("/"), lib)
                if os.path.exists(want):
                    continue
                have = sorted(
                    os.path.relpath(os.path.join(r, n), dep)
                    for r, _d, ns in os.walk(dep) for n in ns if n == lib
                )
                if not have:
                    continue
                problems.append((f, "line %d: links %s but %s provides %s"
                                 % (i + 1, want, prefixes[var],
                                    ", ".join(have))))

# --- is a host directory on a search path? -----------------------------------
# -L/usr/lib finds the build host's libc.a ahead of the sysroot's, which turns a
# musl port into a glibc one or fails the link outright.  -I/usr/include puts
# glibc's headers ahead of musl's for the same reason.  A variable that expands
# to either is the same mistake, so it is resolved before matching.
HOST_ROOTS = r"/usr(?:/(?:local/)?(?:include|lib\d*|share/pkgconfig))?"
# The variables that hold the rootfs or the staging tree.  A dependency staged
# into one of those is installed at /usr inside it, so $(X)/usr/include is the
# path of the copy -- and on this host /usr/include is also glibc's.  Use the
# dependency's own build/deps prefix instead; audio/tinyalsa publishes one.
#
# FREELINX_SYSROOT is not one of these: it is the toolchain's musl sysroot, and
# naming it is the whole point.
STAGING_VARS = ("FREELINX_STAGING_DIR", "FREELINX_ROOTFS", "ROOTFS_DIR",
                "STAGE_ROOT", "STAGING_DIR")
HOST_SEARCH = re.compile(r"(?<![-\w])-(L|I|isystem|idirafter|B|LIBPATH)\s*=?\s*("
                         + HOST_ROOTS + r")\b")
HOST_EXPANDING = re.compile(
    r"-(L|I|isystem|idirafter|B|LIBPATH)\s*=?\s*\$\((" + "|".join(STAGING_VARS)
    + r")\)(/[^\s\"'|)]*)")

for pat in ("*/Makefile", "*/*/Makefile"):
    for f in sorted(glob.glob(pat)):
        if "/build/" in f:
            continue
        for i, l in enumerate(open(f).read().split("\n")):
            # A recipe comment starts with # after the optional @ that keeps
            # make from echoing the line, and these Makefiles explain their own
            # mistakes in exactly this place -- devel/expat says why expat.pc
            # must not say /usr/local, and quotes the offending Cflags doing it.
            if l.lstrip().lstrip("@").lstrip().startswith("#"):
                continue
            m = HOST_SEARCH.search(l)
            if m:
                problems.append((f, "line %d: host directory on the search path: "
                                    "-%s %s; the sysroot is the only place a musl "
                                    "build may look" % (i + 1, m.group(1), m.group(2))))
            for m2 in HOST_EXPANDING.finditer(l):
                problems.append((f, "line %d: host directory on the search path: "
                                    "-%s $(%s)%s; use the dependency's own "
                                    "build/deps prefix, not the rootfs"
                                 % (i + 1, m2.group(1), m2.group(2), m2.group(3))))

for f, what in problems:
    print(f"  {f}: {what}")
print(f"  {len(problems)} problem(s)" + ("  (repaired)" if fix and problems else ""))
sys.exit(1 if problems and not fix else 0)
