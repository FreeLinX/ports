#!/usr/bin/env python3
"""Regenerate a damaged FreeLinX port patch against the real upstream source.

Why
---
The patch files in this tree were hand-written and hand-edited, and the edits
were done in tools that strip trailing whitespace.  That silently damaged the
encoding: an empty *context* line has to be a single space followed by the
newline, and when that space is stripped the line is no longer a diff line at
all.  patch(1) ends the hunk there, so the rest of the change is never applied
- and because the remaining lines still match, the port keeps building.  The
damage only shows up as a port that is quietly wrong.

scripts/check-patches.py repairs the encoding.  A handful of files cannot be
repaired that way, because their hunk body is genuinely shorter than the header
says: the last lines were lost, not just their leading space.  Those need the
real source to say what the missing lines were.  This tool does that: it takes
the surviving intent out of the patch, applies it to a pristine copy of the
upstream file, and lets diff(1) write a well-formed patch.

It never invents a change.  The new-side lines are exactly the '+' lines of the
existing patch, and the context is exactly the surviving context; only the
framing - line numbers, counts, context extent - is recomputed by diff.

Usage
-----
    python3 scripts/regen-patch.py <patch-file> <source-root> [out-file]

<source-root> is the directory the port's sources are extracted into, laid out
the way the port sees them (so a patch touching usr.bin/msgs/msgs.c needs a
root containing usr.bin/msgs/msgs.c).  out-file defaults to the patch file.
"""
import difflib
import pathlib
import re
import sys

HUNK = re.compile(r'^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@')


def split_file(text):
    """Real lines of a file.

    splitlines(), not split('\\n'): for text ending in a newline the latter
    yields a phantom final element, which difflib then treats as an extra
    context line that no such line in the file can ever match, and patch(1)
    rejects the result.
    """
    return text.splitlines()


def parse(patch_path):
    """Return [(target_path, [hunk, ...])] for a patch file.

    A hunk is (start_line, entries) where entries is [(tag, content), ...] and
    tag is ' ', '+' or '-'.  The tags are kept rather than being collapsed into
    an old-list and a new-list, because one repair below is only safe on context
    lines and needs to know which is which.
    """
    text = patch_path.read_text(encoding='utf-8', errors='replace')
    lines = text.splitlines()
    files = []
    cur = None
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith('--- ') and i + 1 < len(lines) and lines[i + 1].startswith('+++ '):
            target = line[4:].split('\t')[0].strip()
            if target.startswith(('a/', 'b/')):
                target = target[2:]
            cur = (target, [])
            files.append(cur)
            i += 2
            continue
        m = HUNK.match(line)
        if m and cur is not None:
            start = int(m.group(1))
            body = []
            j = i + 1
            while j < len(lines):
                bl = lines[j]
                if not bl or bl[0] not in ' +-\\':
                    break
                if HUNK.match(bl) or bl[:4] in ('--- ', '+++ '):
                    break
                body.append(bl)
                j += 1
            entries = []
            for bl in body:
                if bl.startswith('\\'):
                    continue          # "\ No newline at end of file"
                entries.append((bl[0], bl[1:]))
            cur[1].append((start, entries))
            i = j
            continue
        i += 1
    return files


def old_of(entries):
    return [c for t, c in entries if t in ' -']


def new_of(entries):
    return [c for t, c in entries if t in ' +']


def trim_trailing_context(entries):
    """Drop trailing context lines, and say how many went.

    A hunk that is hand framed routinely ends with one more context line than
    the file has, because a context line that is empty is written as a bare
    newline and then normalised to ' ' by whatever edited the file, so it looks
    like any other context line and is easy to add by accident.  base/find's
    patch-find-findh is the clean case: its hunk body ends

        N_AND = 1,  ...  /* must start > 0 */
        <empty context line>

    and find.h has no blank line after N_AND - it goes straight to N_AMIN.  The
    six lines above it do match, so the intent is unambiguous; the seventh line
    is simply not there.

    Only *context* is ever trimmed.  Dropping a trailing '-' line would discard
    part of the change itself, and a patch that has lost a removal is not a
    patch this tool can repair: it would quietly not do what it says.
    """
    n = 0
    while entries and entries[-1][0] == ' ':
        entries = entries[:-1]
        n += 1
    return entries, n


def locate(haystack, needle, hint):
    """Index of needle in haystack, preferring the position the header claims.

    patch(1) searches outwards from the header's line number, so the search
    here does too; a header that survived the damage intact is still the best
    available hint about where the hunk belongs.
    """
    n = len(needle)
    if n == 0 or n > len(haystack):
        return None
    for delta in range(0, len(haystack)):
        for cand in {hint - 1 + delta, hint - 1 - delta}:
            if 0 <= cand <= len(haystack) - n and haystack[cand:cand + n] == needle:
                return cand
    return None


def main(argv):
    if len(argv) < 3:
        print(__doc__.strip())
        return 2
    patch_path = pathlib.Path(argv[1])
    src_root = pathlib.Path(argv[2])
    out_path = pathlib.Path(argv[3]) if len(argv) > 3 else patch_path

    out = []
    for target, hunks in parse(patch_path):
        src_file = src_root / target
        if not src_file.is_file():
            print('no such source file: %s' % src_file, file=sys.stderr)
            return 1
        original = src_file.read_text(encoding='utf-8', errors='surrogateescape')
        lines = split_file(original)
        # Apply from the bottom up so earlier hunks keep their line numbers.
        for start, entries in sorted(hunks, key=lambda h: -h[0]):
            at = locate(lines, old_of(entries), start)
            if at is None:
                entries, dropped = trim_trailing_context(entries)
                at = locate(lines, old_of(entries), start) if entries else None
                if at is not None:
                    print('%s: line %d: the hunk named %d trailing context '
                          'line(s) the source does not have; dropped'
                          % (patch_path, start, dropped), file=sys.stderr)
            if at is None:
                print('%s: cannot locate the hunk that should start at line %d'
                      % (patch_path, start), file=sys.stderr)
                for l in old_of(entries):
                    print('    %r' % l, file=sys.stderr)
                return 1
            lines[at:at + len(old_of(entries))] = new_of(entries)
        trailing = original.endswith('\n')
        modified = '\n'.join(lines) + ('\n' if trailing else '')

        out.extend(difflib.unified_diff(
            split_file(original), split_file(modified),
            fromfile='a/' + target, tofile='b/' + target, n=3))
        print('%s: regenerated %d hunk(s) for %s' % (patch_path, len(hunks), target),
              file=sys.stderr)

    # difflib is inconsistent about line endings: the '---', '+++' and '@@'
    # header lines already carry the lineterm, the hunk body lines do not.  A
    # plain '\n'.join() therefore leaves a blank line after every header - and
    # a blank line inside a hunk is not a diff line, so patch(1) stops reading
    # there.  That is the very damage this tool exists to undo, so normalise
    # every line to exactly one terminating newline before writing.
    body = ''.join(line if line.endswith('\n') else line + '\n' for line in out)
    if body and not body.endswith('\n'):
        body += '\n'
    out_path.write_text(body, encoding='utf-8')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
