#!/usr/bin/env python3
"""Check that every port's patches actually apply to a fresh extract.

Why this exists, alongside check-patches.py
--------------------------------------------

check-patches.py answers "is this patch file well formed?".  It cannot answer
"does this patch apply?", and that is the question that actually decides
whether a port builds.  The two failures look identical from inside the build -
the port's FreeLinX change silently does not exist - but they are found in
completely different ways, and neither the build nor the encoding check finds
this one.

The specific defect
-------------------

GNU patch 2.7.6, run with --fuzz=0 as mk/base-port.mk runs it, refuses a hunk
whose framing it did not produce itself, even when the hunk header's counts and
the hunk body agree with each other and the body matches the source byte for
byte.  The refusal is silent as to its reason: "Hunk #1 FAILED at 61." and
nothing else.

base/chown is the clean example.  Two nearly identical hunks, same file, same
insert, differing only in how much context each carries:

    hand written   @@ -61,3 +61,9 @@   2 lines of leading context
    diff -u / difflib   @@ -60,6 +60,12 @@   3 lines of leading context

Against the same pristine chown.c, the first is rejected with --fuzz=0 and the
second applies.  The bytes are all there in both; what differs is the framing.
A hunk that has been trimmed on the left is indistinguishable, to patch, from a
hunk that was matched with fuzz - and --fuzz=0 exists precisely to refuse those.

So the rule this tool exists to enforce is not "write valid diffs", it is
"produce hunks with diff -u or difflib, and let the tool frame them".  Hand
authored hunk headers are not reliably applicable, however self-consistent they
look, and the failure mode is a port that builds without the change it was
patched for.

What it does
------------

For each port with a Makefile, exactly mirroring the do-prepare recipe in
mk/base-port.mk:

  1. extract NETBSD_MEMBERS from DISTINFO_ARCHIVE with the same tar options
     (--strip-components=2, --exclude=CVS..., --no-same-owner)
  2. run each patches/patch-* in order, -p1 --fuzz=0, --dry-run
  3. report

--dry-run is what makes this safe to run over the whole tree: no port's build
directory is touched, so this can be run while a build is in progress.

A port with no patches/ directory, or with no NETBSD_MEMBERS, is reported as
having nothing to check rather than being passed silently - a patch file that
lives somewhere this tool does not look is a patch that is never applied.

Exit status is 0 only when every port with patches applies cleanly.

Usage
-----
    scripts/apply-patches.py                 # check every port
    scripts/apply-patches.py -v              # also name each port that is fine
    scripts/apply-patches.py chown expr      # check just these
"""

import argparse
import os
import pathlib
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent.parent
CATEGORIES = ("base", "shells", "net", "firmware")

# Archive handles and member lists, kept open for the whole run - see
# _archive_names() for why.
_OPEN = {}
_NAMES = {}

# Mirrors the tar exclusions in mk/base-port.mk's do-prepare recipe.
TAR_EXCLUDE = ("CVS", "CVS/Root", "CVS/Entries")

# A make variable assignment, possibly \ continued over several lines.  Only the
# first assignment of a name is taken, which is what make does too.
_ASSIGN = re.compile(r"^[ \t]*(?P<name>[A-Za-z_][A-Za-z0-9_]*)[ \t]*[:+?]?=[ \t]*(?P<val>.*)$")


def read_make_vars(portdir, makefile):
    """Return the simple variable assignments that make would see for a port.

    Two files matter, in make's own order of precedence:

      * the port's `distinfo`, which mk/port.mk does `-include` - this is where
        DISTINFO_ARCHIVE usually lives.  base/diff is the case that made this
        matter: it takes OpenBSD's diff from dist/openbsd-diff.tgz, and its
        NETBSD_MEMBERS deliberately name usr/src/usr.bin/diff/*, which is not
        in src.tgz at all.  Reading only the Makefile made the default of
        src.tgz apply and the port was reported as broken when it is fine.
      * the Makefile, which wins, because a later assignment in an included
        file overrides an earlier one.

    Deliberately not a make implementation: it joins the backslash-continued
    lines and takes the first assignment of each name, which is all the four
    variables this tool needs.  Anything cleverer would start disagreeing with
    make in ways that are hard to see.
    """
    out = {}
    for path in (portdir / "distinfo", makefile):
        if not path.is_file():
            continue
        lines = path.read_text(errors="replace").splitlines()
        i = 0
        while i < len(lines):
            m = _ASSIGN.match(lines[i])
            if not m:
                i += 1
                continue
            val = m.group("val")
            while val.rstrip().endswith("\\") and i + 1 < len(lines):
                val = val.rstrip()[:-1] + " " + lines[i + 1].strip()
                i += 1
            # distinfo first, Makefile second, so only fill in what the
            # Makefile does not itself assign.
            if m.group("name") not in out or path is makefile:
                out.setdefault(m.group("name"), val.strip())
            i += 1
    return out


def pipeline_of(portdir, makefile):
    """Which patch pipeline this port uses, or None if it is not the standard one.

    The defect this tool looks for belongs to one specific invocation:

        patch -d "$(SRC_DIR)" -p1 --fuzz=0 < "$p"

    which is mk/base-port.mk's do-prepare, run over NETBSD_MEMBERS out of
    DISTINFO_ARCHIVE.  Ports outside that pipeline cannot be checked the same
    way, and reporting them as failures would be reporting something untrue:

      * a project port (lynx, dhcpcd, fastfetch) patches an upstream tree that
        was fetched and extracted by mk/project-port.mk, not a NetBSD member
        list, and a patch there may legitimately be written against a different
        revision of a different project.
      * shells/netbsd-sh extracts the whole src set and applies its patches
        with plain `patch -p1` and no --fuzz=0, so --fuzz=0's refusal of a
        hand-framed hunk - the whole subject of this tool - cannot arise.

    Both are reported as out of scope, with the reason, rather than skipped in
    silence.
    """
    text = makefile.read_text(errors="replace")
    if "project-port.mk" in text:
        return "project port: patches an upstream tree, not NETBSD_MEMBERS"
    if "base-port.mk" not in text:
        return "hand-rolled port: no mk/base-port.mk patch pipeline"
    return None


def split_tokens(s):
    """Split a make value on whitespace, honouring the \ line continuation."""
    s = s.replace("\\\n", " ")
    return s.split()


def archive_for(vars_):
    """Resolve DISTINFO_ARCHIVE the way mk/base-port.mk does."""
    arch = vars_.get("DISTINFO_ARCHIVE", "src.tgz")
    return ROOT / "dist" / arch


def _archive_names(archive):
    """The member list of an archive, cached per archive.

    getnames() on a 234 MB gzip decompresses the whole stream, and this tool
    walks every port in the tree, so without the cache the same archive is
    decompressed once per port - hundreds of times, for a list that cannot
    have changed.  Building the ports' src.tgz is the expensive part of a
    ports run; this check must not multiply it.
    """
    if archive not in _NAMES:
        with tarfile.open(archive, "r:*") as tf:
            _NAMES[archive] = set(tf.getnames())
    return _NAMES[archive]


def _open_archive(archive):
    """Hold each archive open for the run.

    extractfile() on a gzip stream seeks, and tarfile re-decompresses from the
    start when it does.  Keeping the handle open and reusing it is what holds
    the whole-tree check to one pass per archive.
    """
    if archive not in _OPEN:
        _OPEN[archive] = tarfile.open(archive, "r:*")
    return _OPEN[archive]


def extract(archive, members, dest):
    """Extract members with mk/base-port.mk's tar semantics, or return why not.

    Python's tarfile is used rather than a tar subprocess so the test does not
    depend on the host tar accepting the same options; the semantics it needs
    are few: strip two leading path components, and drop CVS bookkeeping.
    """
    if not members:
        return "no NETBSD_MEMBERS: nothing extracted, so no patch can apply"
    if not archive.is_file():
        return "archive missing: %s" % archive
    tf = _open_archive(archive)
    available = _archive_names(archive)
    missing = [m for m in members if m not in available]
    if missing:
        return "archive %s has no member %s" % (archive.name, missing[0])
    for name in members:
        # --strip-components=2
        parts = name.split("/")
        rel = "/".join(parts[2:]) if len(parts) > 2 else parts[-1]
        if any(p in TAR_EXCLUDE for p in parts):
            continue
        src = tf.extractfile(name)
        if src is None:
            continue
        target = dest / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        with open(target, "wb") as fh:
            shutil.copyfileobj(src, fh)


def run_patch(workdir, patchfile):
    """Run one patch the way base-port.mk does.  Returns (ok, output)."""
    with open(patchfile, "rb") as fh:
        proc = subprocess.run(
            ["patch", "-d", str(workdir), "-p1", "--fuzz=0", "--dry-run"],
            stdin=fh, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
    return proc.returncode == 0, proc.stdout.decode(errors="replace")


def hunks_rejected(out):
    """The hunk numbers patch says it could not place."""
    return re.findall(r"Hunk #(\d+) FAILED", out)


def check_port(portdir, verbose=False):
    """Returns (state, detail).  state is ok / fail / out / nothing."""
    mk = portdir / "Makefile"
    if not mk.is_file():
        return "skip", "no Makefile; not a port"
    patchdir = portdir / "patches"
    patches = sorted(patchdir.glob("patch-*")) if patchdir.is_dir() else []
    if not patches:
        return "nothing", "no patches/ to check"

    scope = pipeline_of(portdir, mk)
    if scope:
        return "out", scope

    vars_ = read_make_vars(portdir, mk)
    members = split_tokens(vars_.get("NETBSD_MEMBERS", ""))
    archive = archive_for(vars_)

    with tempfile.TemporaryDirectory(prefix="flx-apply-") as tmp:
        work = pathlib.Path(tmp)
        why = extract(archive, members, work)
        if why:
            return "fail", why
        bad = []
        for p in patches:
            ok, out = run_patch(work, p)
            if not ok:
                bad.append((p.name, hunks_rejected(out)))
        if bad:
            return "fail", "; ".join(
                "%s (hunk %s)" % (n, ",".join(h) if h else "unknown")
                for n, h in bad)
        if verbose:
            return "ok", "%d patch(es) over %d member(s)" % (len(patches), len(members))
        return "ok", ""


def all_ports():
    seen = {}
    for cat in CATEGORIES:
        base = ROOT / cat
        if not base.is_dir():
            continue
        for d in sorted(base.iterdir()):
            if d.is_dir() and (d / "Makefile").is_file():
                seen.setdefault(d.name, d)
    return seen


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("ports", nargs="*", help="port names; default is every port")
    ap.add_argument("-v", "--verbose", action="store_true",
                    help="also report the ports that are fine")
    args = ap.parse_args()

    if args.ports:
        known = all_ports()
        selected = []
        for name in args.ports:
            if name not in known:
                print("no such port: %s" % name, file=sys.stderr)
                return 2
            selected.append((name, known[name]))
    else:
        selected = sorted(all_ports().items())

    n_ok = n_fail = n_nothing = n_out = 0
    failures = []
    out_of_scope = []
    for name, d in selected:
        state, detail = check_port(d, verbose=args.verbose)
        if state == "ok":
            n_ok += 1
            if args.verbose:
                print("ok   %-14s %s" % (name, detail))
        elif state == "fail":
            n_fail += 1
            failures.append((name, detail))
            print("FAIL %-14s %s" % (name, detail))
        elif state == "out":
            n_out += 1
            out_of_scope.append((name, detail))
            if args.verbose:
                print("out  %-14s %s" % (name, detail))
        elif state == "nothing":
            n_nothing += 1
            if args.verbose:
                print("---- %-14s %s" % (name, detail))
        # skip needs no reporting: it is not a port

    print()
    print("%d port(s) with patches, all apply" % n_ok if not n_fail else
          "%d apply, %d do not" % (n_ok, n_fail))
    if n_nothing:
        print("%d port(s) have no patches/ directory" % n_nothing)
    if out_of_scope:
        print("%d port(s) use a different patch pipeline and are not checked "
              "here:" % n_out)
        for name, why in out_of_scope:
            print("     %-14s %s" % (name, why))
    if failures:
        print()
        print("The patches above were hand framed.  Regenerate them against the")
        print("real source so diff writes the hunk headers - see")
        print("scripts/regen-patch.py - rather than raising the counts, which")
        print("patch --fuzz=0 will refuse just as firmly.")
    return 1 if n_fail else 0


if __name__ == "__main__":
    sys.exit(main())
