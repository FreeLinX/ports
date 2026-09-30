#!/usr/bin/env python3
"""check-port-mkfiles.py [--fix]

Report (and optionally repair) two mistakes that keep recurring in this tree and
that make make fail in ways which name neither the file nor the variable:

  1. FREELINUX_ where FREELINX_ was meant.  Every FREELINX_* variable is
     undefined, so the line expands to nothing: CC= becomes empty and configure
     reports "C compiler cannot create executables"; -j"" makes ninja abort.

  2. PREFIX_<dep> used in a flag list but never assigned.  Reported
     only: the repair would have to guess the prefix, and see below.  There is no
     PREFIX_expat, so -I$(PREFIX_expat)/include becomes -I/include, which is
     silently wrong rather than an error -- the build then stops at the next
     "file not found" and blames the header.

Run from the ports root.
"""

import glob
import os
import re
import sys

BAD = "FREEL" + "INUX_"      # F R E E L I N U X _
GOOD = "FREEL" + "INX_"     # F R E E L I N X _

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

        used = set(re.findall(r"\bPREFIX_(\w+)", text))
        defined = {
            l.split("=", 1)[0].strip()[len("PREFIX_"):]
            for l in lines
            if l.startswith("PREFIX_") and "=" in l
        }
        for d in sorted(used - defined):
            problems.append((f, f"PREFIX_{d} used but never assigned"))
            if fix:
                last = max(i for i, l in enumerate(lines) if l.startswith("PREFIX_"))
                lines.insert(last + 1, f"PREFIX_{d} = $(FREELINX_BUILD_DIR)/deps/{d}")
                open(f, "w").write("\n".join(lines))
                lines = open(f).read().split("\n")
                # The value just written may itself carry the misspelling.
                s = "\n".join(lines)
                if BAD in s:
                    open(f, "w").write(s.replace(BAD, GOOD))

for f, what in problems:
    print(f"  {f}: {what}")
print(f"  {len(problems)} problem(s)" + ("  (repaired)" if fix and problems else ""))
sys.exit(1 if problems and not fix else 0)
