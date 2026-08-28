# FreeLinX/ports

A clean, minimal **BSD-style ports framework** for FreeLinX — a Linux-based
operating system. It is used to fetch, patch, build and stage **NetBSD-derived
userspace** programs against the FreeLinX musl toolchain.

This repository is **not** a package manager, not a dependency solver, not a
binary package repository, and not the OS itself. It is a thin, reproducible
harness: POSIX `sh` + `make`/`bmake` + simple metadata + patches.

---

## What this is, and why it exists

FreeLinX wants a BSD-flavoured userland, but it runs on a Linux kernel and a
**musl** libc, built with **Clang + LLD**. NetBSD's userland is written against
NetBSD's libc and headers, so we cannot just drop it in. Instead we port it
program-by-program using a small `pkgsrc`-inspired framework:

```
NetBSD upstream source
        │
        ▼
FreeLinX port definition   (metadata + distinfo + recipe)
        │
        ▼
FreeLinX patches           (small, documented, musl-specific)
        │
        ▼
Clang + LLD + musl sysroot (the FreeLinX toolchain)
        │
        ▼
static/dynamic x86_64 ELF binary
        │
        ▼
FreeLinX/src/rootfs/bin/   (staged, later baked into the ISO)
```

The framework is intentionally modelled on the BSD ports / pkgsrc approach:
each port lives in its own directory and owns its metadata, source reference,
patches and build recipe. It must work for **any** developer or CI machine by
using configurable `FREELINX_*` variables — **never** hard-coded paths.

---

## Repository layout

```
ports/
├── README.md              this file
├── Makefile               top-level interface (help/list/build/install/...)
├── config/
│   └── default.conf       single source of truth for FREELINX_* defaults
├── mk/                    make framework shared by every port
│   ├── common.mk          common make defaults (mirror of config/default.conf)
│   ├── compiler.mk        clang/lld/sysroot flag construction
│   ├── install.mk         staging + rootfs install rules
│   ├── port.mk            generic per-port harness (toolchain gate, all/install)
│   └── base-port.mk       template for the single-file base utilities
├── base/                  minimal NetBSD utilities (scaffolding ports)
│   ├── cat/  echo/  ls/  mkdir/  cp/  mv/  rm/
│   │   ├── Makefile
│   │   ├── distinfo       upstream source reference + checksum
│   │   └── patches/       (empty for now; FreeLinX patches go here)
├── shells/
│   └── netbsd-sh/         the FIRST real port
│       ├── Makefile
│       ├── distinfo
│       └── patches/
│           └── README     patch conventions
└── scripts/               top-level or port operations (POSIX /bin/sh)
    ├── common.sh          shared helpers + config loading + toolchain detect
    ├── fetch.sh           download + verify upstream source
    ├── build.sh           build a port (delegates to the port's Makefile)
    ├── install.sh         stage a built port (+ optionally into src rootfs)
    ├── check.sh           read-only diagnostics (config/toolchain/ports)
    ├── list.sh            list ports + metadata
    └── clean.sh           remove generated artifacts
```

Generated output lives under `build/`, `staging/` and `dist/` — all ignored by
git. Nothing generated is ever committed.

---

## Toolchain integration (important)

FreeLinX uses a **configurable external toolchain** produced by the
`FreeLinX/toolchain` repository. This repository never assumes a specific
install path. Everything is derived from `config/default.conf`, which uses the
`${VAR:-default}` idiom so environment variables win:

| Variable             | Default                                    | Meaning                              |
|----------------------|--------------------------------------------|--------------------------------------|
| `FREELINX_TRIPLE`    | `x86_64-linux-musl`                        | Target triple                       |
| `FREELINX_TOOLCHAIN_DIR` | `$(FREELINX_ROOT)/../toolchain`         | Toolchain repo output location      |
| `FREELINX_TOOLCHAIN_BIN` | `$(FREELINX_TOOLCHAIN_DIR)/bin`          | Where clang/ld.lld live             |
| `FREELINX_CC`        | `$(FREELINX_TOOLCHAIN_BIN)/clang`          | C compiler driver                   |
| `FREELINX_LD`        | `$(FREELINX_TOOLCHAIN_BIN)/ld.lld`         | Linker                              |
| `FREELINX_SYSROOT`   | `$(FREELINX_TOOLCHAIN_DIR)/x86_64-linux-musl` | musl sysroot                    |
| `FREELINX_PREFIX`    | `staging/$(FREELINX_TRIPLE)`               | Staging destination                 |
| `FREELINX_LINK_MODE` | `static`                                   | `static` or `dynamic`               |
| `FREELINX_ROOTFS_BIN`| `../src/rootfs/bin`                        | Where install -r copies binaries    |

The canonical compile invocation is:

```
clang \
  --target=$(FREELINX_TRIPLE) \
  --sysroot=$(FREELINX_SYSROOT) \
  -fuse-ld=lld \
  [-static] \
  ...sources... -o <out>
```

**Static linking is not forced forever.** It is the default only because it is
extremely useful during the FreeLinX bootstrap. Set `FREELINX_LINK_MODE=dynamic`
(or pass `FREELINX_LINK_MODE=dynamic` to make) to link dynamically instead.

The `mk/compiler.mk` builds the flag sets (`FREELINX_CFLAGS`, `FREELINX_LDFLAGS`)
from these variables, and `FREELINX_TOOLCHAIN_READY` reports whether the
configured compiler + sysroot actually exist. If the toolchain is unavailable,
the framework refuses to build and reports the blocker explicitly — it never
claims a build happened.

---

## Relationship with the sibling repositories

- **`FreeLinX/toolchain`** — builds Clang/LLD + musl and produces the sysroot this
  framework compiles against. `FREELINX_CC`/`FREELINX_LD`/`FREELINX_SYSROOT` point
  at its output by default.
- **`FreeLinX/src`** — owns the root filesystem template (`rootfs/`) and the full
  system integration build. This ports framework **stages** binaries under
  `staging/`, and `FreeLinX/src` later consumes them into `rootfs/bin/` (e.g.
  `sh -> rootfs/bin/sh`). `install -r` can also copy a staged binary directly
  into `FreeLinX/src/rootfs/bin/`.
- **`FreeLinX/kernel`** — Linux 6.1; untouched here.
- **`FreeLinX/iso`** — consumes `FreeLinX/src`'s output; untouched here.

---

## Build interface

```
make help                 # list targets
make list                 # list ports + metadata
make check                # read-only: config, toolchain, port inventory
make build [PORT=netbsd-sh]   # build a port (default: all)
make build PORT=base/cat      # category-qualified
make fetch [PORT=netbsd-sh]   # download + verify upstream source
make install [PORT=netbsd-sh] # stage a built port
make install PORT=... INSTALL_FLAGS=-r   # also copy into src rootfs
make clean                # remove build/, staging/, dist/
# note: make clean is a full clean; a single port's artifacts are removed by
#       make -C <portdir> clean (e.g. make -C base/cat clean)
```

The scripts can be run directly (`./scripts/build.sh netbsd-sh`). Output uses a
clear `[FreeLinX/ports]` prefix and never exaggerates success.

---

## How a port works

1. A port is a directory `category/name/` with a `Makefile`, a `distinfo`, and a
   `patches/` directory.
2. `distinfo` names the upstream archive and its URL. `DISTINFO_SHA256` pins the
   checksum (**TODO until computed from the real download** — we never invent a
   hash).
3. `scripts/fetch.sh` downloads the archive into `dist/` and verifies its
   checksum.
4. The port's `Makefile` includes `../../mk/port.mk` (which pulls in
   `compiler.mk` and `install.mk`). It declares metadata and a `do-build`
   recipe that compiles/link the sources with the FreeLinX toolchain.
5. `mk/port.mk`'s `all` target first runs the **toolchain gate**; if the
   toolchain is absent it fails with a clear message.
6. The build emits `BUILD_BIN`; `install` stages it to
   `staging/$(FREELINX_TRIPLE)/category/name/bin/<INSTALL_BIN>`.
7. `install -r` copies the staged binary into `FreeLinX/src/rootfs/bin/`.

---

## Adding a new port

1. Pick the NetBSD source it comes from and confirm the exact upstream location
   and release (prefer a fixed NetBSD release src set for reproducibility).
2. Create `category/name/{Makefile,distinfo,patches/}`.
3. In `distinfo`, set `DISTINFO_NAME`, `DISTINFO_ARCHIVE`, `DISTINFO_URL` and
   leave `DISTINFO_SHA256=TODO`; run `scripts/fetch.sh name` and pin the printed
   checksum after review.
4. Write the `Makefile` using `include ../../mk/port.mk`, set metadata
   (`NAME`, `VERSION`, `CATEGORY`, `LICENSE`, `DEPENDENCIES`,
   `BUILD_DEPENDENCIES`, `TARGET`, `PREFIX`), and a `do-build` recipe.
5. For every missing libc API, investigate in this order: **is it POSIX? → does
   musl provide it? → does the program actually need it?** Only then add a
   minimal patch under `patches/` (see `patches/README`).
6. `make build PORT=name`, then `make install PORT=name`.

---

## Patches

Patches live in each port's `patches/` directory. They are the **FreeLinX
patches** layer — applied on top of the pristine upstream source, kept small
and isolated. See `shells/netbsd-sh/patches/README` for the full conventions.

---

## Staging

- `dist/`      — downloaded upstream archives (gitignored).
- `build/work/`— unpacked source and per-port build trees (gitignored).
- `staging/`   — staged binaries, keyed by target triple and port (gitignored).

A successful build of `sh` ends up conceptually as:

```
staging/x86_64-linux-musl/shells/netbsd-sh/bin/sh
```

which `FreeLinX/src` later copies to `rootfs/bin/sh`. Nothing here writes to the
live system `/`.

---

## First port: NetBSD sh

Status: **framework complete; the compile itself is BLOCKED by the not-yet-built
FreeLinX toolchain** (no clang/ld.lld/musl sysroot exists in `FreeLinX/toolchain`
yet). This is expected and intentionally reported, not faked.

Please keep these four layers distinct when working on it:

| Layer             | What it is                                                        |
|-------------------|-------------------------------------------------------------------|
| UPSTREAM SOURCE   | NetBSD 10.1, `external/bsd/bin/sh`. Fetched, never edited here.   |
| FREELINX PATCHES  | `shells/netbsd-sh/patches/` — small musl compatibility patches.   |
| BUILD FRAMEWORK   | This repo: `mk/`, `scripts/`, `config/`.                          |
| GENERATED ARTIFACTS| `build/`, `staging/`, `dist/` — gitignored.                      |

**Upstream source:** NetBSD 10.1 `/bin/sh` (Almquist shell / ash), in the NetBSD
source tree at `external/bsd/bin/sh`. Obtained from the immutable NetBSD 10.1
release source set `src.tgz`:
`https://cdn.netbsd.org/pub/NetBSD/NetBSD-10.1/source/sets/src.tgz`
(release checksums published by the NetBSD project at that directory).

**Compatibility work expected** (verify against musl before patching):
`strlcpy()/strlcat()`, BSD `err()`/`warn()`, `<sys/queue.h>`, BSD attribute
macros, and compiling the `.c` sources directly (NetBSD itself uses bmake).

**To build once the toolchain is ready:** set `FREELINX_CC`, `FREELINX_LD`,
`FREELINX_SYSROOT` (defaults already point at `FreeLinX/toolchain` output),
then `make build PORT=netbsd-sh` and `make install PORT=netbsd-sh`.

---

## Bootstrap strategy

1. `FreeLinX/toolchain` builds clang + lld + musl and produces a sysroot.
2. Set `FREELINX_CC`/`FREELINX_LD`/`FREELINX_SYSROOT` (or let the defaults find
   the sibling repo output).
3. Start with trivial ports (base): compile single-file NetBSD utilities.
4. Bring up `netbsd-sh` next, adding minimal musl-compat patches.
5. Expand to more NetBSD userland, staging binaries for `FreeLinX/src`.

## Current limitations

- The FreeLinX toolchain is still being finalized; the first real port
  (`netbsd-sh`) cannot be compiled yet and reports that blocker honestly.
- No package manager / dependency solver yet (not needed at this stage).
- Base utilities are scaffolding metadata ports; their compile recipes are
  pending source fetch + toolchain availability.
- Archive checksums are intentionally unverified (`DISTINFO_SHA256=TODO`) until
  the real downloads are hashed and reviewed.
```
