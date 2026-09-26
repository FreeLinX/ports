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
    """Return [(target_path, [hunk, ...])] for a patch file."""
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
            old, new = [], []
            for bl in body:
                if bl.startswith('\\'):
                    continue
                tag, content = bl[0], bl[1:]
                if tag in ' -':
                    old.append(content)
                if tag in ' +':
                    new.append(content)
            cur[1].append((start, old, new))
            i = j
            continue
        i += 1
    return files


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
        for start, old, new in sorted(hunks, key=lambda h: -h[0]):
            at = locate(lines, old, start)
            if at is None:
                print('%s: cannot locate the hunk that should start at line %d'
                      % (patch_path, start), file=sys.stderr)
                for l in old:
                    print('    %r' % l, file=sys.stderr)
                return 1
            lines[at:at + len(old)] = new
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
