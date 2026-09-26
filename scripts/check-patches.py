#!/usr/bin/env python3
"""Validate - and optionally repair - the FreeLinX port patch files.

Why this exists
---------------
A patch whose *encoding* is damaged is not a build error you can see.  GNU
patch(1) trusts the hunk header's line counts and the leading character of each
body line.  Where either is wrong it stops reading the hunk early, applies only
the part it managed to read, and the port still links - so the change silently
never happened.  Nothing in the build reports it; it is only found by building
the port and noticing the port is still wrong.

The damage here has one dominant cause.  A context line whose source text is
empty must be written as a single space followed by the newline.  These files
were hand-edited in tools that trim trailing whitespace, and the space was
stripped, leaving a bare newline.  A bare newline is not a diff line at all, so
patch ends the hunk there and drops everything after it.

A related defect is a file whose last line has no newline, so the final hunk
line is incomplete and patch reports "patch unexpectedly ends in middle of
line".

What --fix does, and deliberately does not do
---------------------------------------------
--fix restores the missing space on empty context lines and adds a missing final
newline.  That is a pure encoding repair: the '+', '-' and ' ' lines are never
modified, so what a patch does to the source is unchanged.

--fix never rewrites a hunk header's line counts.  When a count disagrees with
the body it is the *body* that is damaged - a truncated final line, or context
lines lost to whitespace trimming - not the header.  The header was written
first and is normally right.  Regenerating it from the truncated body silently
changes which lines the patch claims to touch, and produces a patch that
matches nothing: base/msgs/patches/patch-msgs-dnamlen declares -284,7 +284,7
and needs the trailing empty context line; rewriting it to -284,6 +284,6 makes
patch reject it even with --fuzz=0.  Such files are reported for manual
attention instead - see the "needs the body completed" note in the output.

Usage
-----
    python3 scripts/check-patches.py          # report
    python3 scripts/check-patches.py --fix    # report and repair encoding
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
HUNK = re.compile(r'^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@')


def _body_end(lines, start):
    """Index just past the hunk body, plus whether a bare newline stopped it."""
    j = start + 1
    bare = False
    while j < len(lines):
        line = lines[j]
        if not line:
            bare = True
            break
        if HUNK.match(line) or line[:4] in ('--- ', '+++ ') or line[:5] == 'diff ':
            break
        if line[0] not in ' +-\\':
            break
        j += 1
    return j, bare


def _counts(lines, start):
    end, _ = _body_end(lines, start)
    old = new = 0
    for j in range(start + 1, end):
        tag = lines[j][0]
        if tag == ' ':
            old += 1
            new += 1
        elif tag == '-':
            old += 1
        elif tag == '+':
            new += 1
    return old, new


def check(path):
    problems = []
    raw = path.read_bytes()
    if not raw:
        return ['file is empty']
    if not raw.endswith(b'\n'):
        problems.append('no newline at end of file: the last hunk line is '
                        'incomplete and patch(1) drops that hunk')
    lines = raw.decode('utf-8', 'replace').splitlines()
    hunks = 0
    for i, line in enumerate(lines):
        m = HUNK.match(line)
        if not m:
            continue
        hunks += 1
        want_old = int(m.group(2)) if m.group(2) is not None else 1
        want_new = int(m.group(4)) if m.group(4) is not None else 1
        end, bare = _body_end(lines, i)
        if bare:
            problems.append(
                'hunk at line %d is cut short by a bare empty line at line '
                '%d: an empty *context* line must be a single space, '
                'otherwise patch(1) ends the hunk there and drops the rest'
                % (i + 1, end + 1))
        got_old, got_new = _counts(lines, i)
        if not bare and (got_old, got_new) != (want_old, want_new):
            if (got_old, got_new) > (want_old, want_new):
                problems.append(
                    'hunk at line %d declares -%d +%d but the body holds %d '
                    'old and %d new line(s): the header is too small, --fix '
                    'raises it' % (i + 1, want_old, want_new, got_old, got_new))
            else:
                problems.append(
                    'hunk at line %d declares -%d +%d but the body holds only '
                    '%d old and %d new line(s): the body is truncated, the '
                    'header is correct.  Complete the body from the real '
                    'source (or regenerate with diff -u) - do not lower the '
                    'header, patch(1) rejects the result even with --fuzz=0'
                    % (i + 1, want_old, want_new, got_old, got_new))
    if hunks == 0:
        problems.append('no unified-diff hunks found (is this a patch file?)')
    return problems


def repair(path):
    """Restore encoding only: empty context lines, the final newline, and
    hunk headers whose counts are smaller than the body they head.

    A header count that is *larger* than its body is left alone: that means the
    body is truncated and only the real source can say what the missing lines
    are.  See the module docstring for why the header is not authoritative.
    """
    text = path.read_text(encoding='utf-8')
    lines = text.splitlines()
    fixed = 0
    i = 0
    while i < len(lines):
        if not HUNK.match(lines[i]):
            i += 1
            continue
        j = i + 1
        while j < len(lines):
            line = lines[j]
            if not line:
                # An empty line inside a hunk body is a context line for an
                # empty source line, whose leading space was trimmed away.
                lines[j] = ' '
                fixed += 1
                j += 1
                continue
            if (line[0] in ' +-\\' and not HUNK.match(line)
                    and line[:4] not in ('--- ', '+++ ')):
                j += 1
                continue
            break
        m = HUNK.match(lines[i])
        want_old = int(m.group(2)) if m.group(2) is not None else 1
        want_new = int(m.group(4)) if m.group(4) is not None else 1
        got_old, got_new = _counts(lines, i)
        if (got_old, got_new) > (want_old, want_new):
            lines[i] = '@@ -%s,%d +%s,%d @@%s' % (
                m.group(1), got_old, m.group(3), got_new, lines[i][m.end():])
            fixed += 1
        i = j
    out = '\n'.join(lines) + '\n'
    if out != text:
        path.write_text(out, encoding='utf-8')
        fixed += 1
    return fixed


def main():
    fix = '--fix' in sys.argv[1:]
    bad = total = 0
    for path in sorted(ROOT.glob('**/patch-*')):
        if not path.is_file() or 'build' in path.parts or 'dist' in path.parts:
            continue
        if path.suffix in ('.xpkg', '.gz'):
            continue
        total += 1
        problems = check(path)
        if not problems:
            continue
        rel = path.relative_to(ROOT)
        if fix:
            repair(path)
            left = check(path)
            print('%-52s encoding repaired%s' % (rel, '' if not left else ''))
            for p in left:
                print('    %s' % p)
            bad += bool(left)
        else:
            bad += 1
            print(rel)
            for p in problems:
                print('    %s' % p)
    print('\n%d patch file(s) %s, %d still needing attention'
          % (total, 'repaired' if fix else 'checked', bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
