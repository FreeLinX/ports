# FreeLinX/ports

A clean, minimal **BSD-style ports framework** for FreeLinX — a Linux-based
operating system. It is used to fetch, patch, build and stage **NetBSD-derived
userspace** programs (and, where FreeLinX needs it, small non-NetBSD
components like runit) against the FreeLinX musl toolchain.

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
├── sysutils/
│   └── runit/             service supervision + init (see below)
│       ├── Makefile
│       └── distinfo
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
| `FREELINX_PREFIX`    | `staging/$(FREELINX_TRIPLE)`               | Install prefix (kept configurable)  |
| `FREELINX_LINK_MODE` | `static`                                   | `static` or `dynamic`               |
| `FREELINX_STAGING_ROOT` | `../staging`                            | Rootfs-overlay staging tree         |
| `FREELINX_SRC_DIR`   | `../src`                                   | Sibling FreeLinX/src repo           |
| `FREELINX_ROOTFS_DIR`| `$(FREELINX_SRC_DIR)/rootfs`               | Rootfs template that consumes output|

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
- **`FreeLinX/kernel`** — Linux 6.6.21; untouched here.
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

**Multi-binary ports** (see `sysutils/runit` below) don't fully work through
the top-level `make install PORT=...` / `scripts/install.sh` path — invoke
their installs directly (`make -C sysutils/runit install`) instead.

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
6. The build emits `BUILD_BIN`; the port's `INSTALL_RELPATH` says where in the
   rootfs that binary belongs (default `bin/$(NAME)`, `sh` uses `bin/sh`).
7. `install` stages it into the overlay tree at
   `staging/$(INSTALL_RELPATH)` (e.g. `staging/bin/sh`).
8. `install -r` copies the staged file into the FreeLinX/src rootfs template:
   `src/rootfs/bin/sh`.

**Rootfs destination safety:** rootfs installs (the `install-rootfs` make target
and `scripts/install.sh -r`) always require an explicit, **absolute**, non-`/`
destination. The destination is validated before anything is copied: empty,
relative, or `/` (and redirects that resolve to `/`) are refused with a clear
error. `FREELINX_ROOTFS_DIR` defaults to `<FreeLinX/src>/rootfs` — never `/` —
so `/` can never be an implicit/default install target.

---

## Adding a new port

1. Pick the NetBSD source it comes from and confirm the exact upstream location
   and release (prefer a fixed NetBSD release src set for reproducibility).
   (For a non-NetBSD component, like `sysutils/runit`, pick a pinned upstream
   release/tarball instead and document why it's not NetBSD-derived.)
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
7. If the port produces more than one binary FreeLinX needs, see
   **"Multi-binary port pattern"** under `sysutils/runit` below.

---

## Patches

Patches live in each port's `patches/` directory. They are the **FreeLinX
patches** layer — applied on top of the pristine upstream source, kept small
and isolated. See `shells/netbsd-sh/patches/README` for the full conventions.

---

## Staging

- `dist/`      — downloaded upstream archives (gitignored).
- `build/work/`— unpacked source and per-port build trees (gitignored).
- `staging/`   — rootfs-compatible overlay of staged binaries (gitignored).

A successful build of `sh` ends up as:

```
staging/bin/sh
```

which the `install` step copies into `FreeLinX/src` as `rootfs/bin/sh` (both the
staging root and the rootfs template are configurable via
`FREELINX_STAGING_ROOT` and `FREELINX_ROOTFS_DIR`). Nothing here writes to the
live system `/`.

---

## First port: NetBSD sh

Status: **complete.** `shells/netbsd-sh` builds against the FreeLinX
toolchain and is staged as `/bin/sh` in `FreeLinX/src`'s rootfs.

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

**Compatibility work done** (verified against musl): `strlcpy()/strlcat()`,
BSD `err()`/`warn()`, `<sys/queue.h>`, BSD attribute macros, and compiling
the `.c` sources directly (NetBSD itself uses bmake).

**To build:** set `FREELINX_CC`, `FREELINX_LD`, `FREELINX_SYSROOT` (defaults
already point at `FreeLinX/toolchain` output), then
`make build PORT=netbsd-sh` and `make install PORT=netbsd-sh`.

---

## Second real port: runit (sysutils/runit)

Status: **built, staged, and installed into FreeLinX/src rootfs. Verified
booting in QEMU as PID 1 — kernel → /init → runsvdir → runsv supervising a
test service — 2026-08-30.**

**Upstream source:** runit 2.2.0 (Gerrit Pape, smarden.org), BSD-2-Clause.
Unlike every other port here, this is **not NetBSD-derived** — runit is its
own independent upstream, included because it is FreeLinX's intended init
and service supervisor (see `FreeLinX/src/rootfs/init`).

**Build model (deliberately different from shells/netbsd-sh):** runit ships
its own build system (`src/compile`, `src/load`, `src/choose`), which does
*runtime* feature-probing — it compiles a small test program, links it,
executes it, and picks a header variant based on what actually happened on
this system. Because FreeLinX targets `x86_64-linux-musl` and builds run on
`x86_64` Linux hosts, these probes execute correctly. Rather than manually
enumerating translation units the way `shells/netbsd-sh` does, this port
writes runit's own `conf-cc`/`conf-ld` to point at the FreeLinX toolchain
(`-rtlib=compiler-rt -unwindlib=none`, since the musl sysroot has no
libgcc/crtbegin/crtend) and lets `package/compile` run unmodified.

**Multi-binary port pattern:** runit produces four binaries FreeLinX needs
(`runsvdir`, `runsv`, `sv`, `chpst`), not one. `mk/port.mk` + `mk/install.mk`
only natively support one binary per port
(`INSTALL_BIN`/`INSTALL_RELPATH`). For any future suite-style port:

1. Set `INSTALL_BIN`/`INSTALL_RELPATH` to just *one* of the binaries — the
   generic framework rules stage/install that one automatically.
2. Add an `install:` target in the port's own Makefile with extra
   prerequisites/recipes staging the remaining binaries to
   `$(STAGE_ROOT)/...` (`make` accumulates multiple `install:` rule bodies
   in one Makefile as long as none conflict).
3. Add a matching `install-rootfs:` target doing the same for
   `$(ROOTFS_DIR)/...`, depending on `check-install-rootfs`.

**Important:** the top-level `scripts/install.sh` (used by
`make install PORT=...` from the repo root) only understands the single
`INSTALL_BIN`/`INSTALL_RELPATH` pair — it has no awareness of a port's extra
install rules. For a multi-binary port, always invoke installs directly
against the port directory:

    make -C sysutils/runit install
    make -C sysutils/runit install-rootfs

rather than `make install PORT=sysutils/runit` from the repo root, which
would only stage/install `runsvdir` and silently skip the other three.

**Boot-critical binaries:**

| Binary      | Rootfs path       | Role                                          |
|-------------|--------------------|------------------------------------------------|
| `runsvdir`  | `/sbin/runsvdir`   | Checked directly by `src/rootfs/init`          |
| `runsv`     | `/sbin/runsv`      | Exec'd by `runsvdir` per service, via `$PATH`  |
| `sv`        | `/usr/bin/sv`      | Admin tool (start/stop/status a service)       |
| `chpst`     | `/usr/bin/chpst`   | Admin tool (run a command under changed state) |

Other binaries the suite builds (`runit`, `runit-init`, `svlogd`,
`runsvctrl`, `runsvstat`, `runsvchdir`, `svwaitup`, `svwaitdown`,
`utmpset`) are left in the build tree for now; extend the port's
`install`/`install-rootfs` targets when FreeLinX needs one.

---

## Networking & wireless stack (net/ + firmware/)

FreeLinX is a GNU-free, BSD-flavoured system. Networking is provided by a mix
of upstream (dhcpcd, wpa_supplicant, libnl) and NetBSD-derived tools, all
built as static musl binaries with clang + LLD (no GNU tool in the system or
in the build). Kernel side is upstream Linux 6.6.21 with `CONFIG_CFG80211` and
`CONFIG_MAC80211` built-in plus each chip driver built as a module.

| Port                 | Role                                 | Category | Status        |
|----------------------|--------------------------------------|----------|---------------|
| `net/libnl`          | netlink socket library (for wpa_supplicant) | net  | **built** (build/deps/libnl) |
| `net/wpa_supplicant` | WPA2/WPA3 supplicant (nl80211)       | net      | **built**, staged to sbin/ (+ `wpa_cli`, `wpa_passphrase`) |
| `net/dhcpcd`         | DHCP/IP configuration client         | net      | **built**, staged to sbin/dhcpcd |
| `net/netbsd-ping`    | NetBSD 10.1 ICMP echo                | net      | scaffolded (needs patches) |
| `net/netbsd-ifconfig`| NetBSD 10.1 interface mgmt (AF_ROUTE)| net      | scaffolded (needs netlink rewrite) |
| `net/netbsd-route`   | NetBSD 10.1 route mgmt (AF_ROUTE)    | net      | scaffolded (kept for reference only) |
| `net/freelinx-ifconfig` | BSD-styled ifconfig over libnl/netlink | net  | **built**, staged to sbin/flx-ifconfig |
| `net/freelinx-route` | BSD-styled route over libnl/netlink  | net      | **built**, staged to sbin/flx-route |
| `firmware/linux-firmware` | redistributable device blobs    | firmware | **built + installed** (1.2G, flat under /lib/firmware) |

### Kernel config (kernel-repo/kernel.config)
`CONFIG_CFG80211=y`, `CONFIG_MAC80211=y`, `CONFIG_RFKILL=y`, `CONFIG_FW_LOADER=y`
and, as modules: `CONFIG_ATH9K=m`, `CONFIG_ATH9K_HTC=m`, `CONFIG_ATH10K=m`,
`CONFIG_IWLWIFI=m`, `CONFIG_RTW88=m`, `CONFIG_RTW89=m`, `CONFIG_BRCMSMAC=m`,
`CONFIG_BRCMFMAC=m`. Sync to the in-tree `.config` at kernel build time with
`cp ../kernel.config .config`.

### The linux-firmware exception (documented)
WiFi/Ethernet/GPU chips need closed, redistributable firmware blobs that
cannot be compiled from Free Software. `firmware/linux-firmware` downloads
the upstream release tarball and copies `firmware/*` into
`/lib/firmware` of the rootfs. This is the one unavoidable non-Free package
FreeLinX ships. Fetch with `make fetch PORT=linux-firmware`, stage/install with
`make install PORT=linux-firmware` (large ~580MB download).

### NetBSD ifconfig/route: resolved with FreeLinX's own tools
NetBSD's `ifconfig` and `route` are built on **BSD routing sockets**
(`AF_ROUTE`, `struct rt_msghdr`), a kernel API the Linux kernel does not
provide (Linux routes/interfaces are managed over `AF_NETLINK` with RTM_*
messages). Simply compiling them against musl is not enough. Rather than
rewrite NetBSD's non-trivial hostops layers, FreeLinX ships **its own**
BSD-styled `flx-ifconfig` and `flx-route`, written from scratch, compiled
only with the FreeLinX clang + LLD + musl toolchain, statically linked
against libnl-3 (which FreeLinX builds itself). They talk AF_NETLINK via
libnl and provide the small BSD command surface a bring-up needs (`up`/
`down`/address/mtu, route `add`/`delete`/`show`). Zero GNU tools in the
system or the build. The `netbsd-ifconfig`/`netbsd-route` scaffolds remain
as correct references to the genuine NetBSD 10.1 sources.

`netbsd-ping` is closer (raw ICMP sockets are portable) but still needs
NetBSD `<netinet/in_systm.h>` / `<netinet/ip_var.h>` /
`<netipsec/ipsec.h>` shims.




### `flx-wifi` — distro-style wifi wrapper
`flx-wifi` wraps `wpa_supplicant` + `wpa_cli` + `wpa_passphrase` + `dhcpcd`
into one BSD-command flow, installed to `/sbin/flx-wifi`:

    flx-wifi scan                                        # scan + list APs
    flx-wifi connect "MyNetwork"                         # open network
    flx-wifi connect "MyNetwork" "mypassword"            # WPA/WPA2
    flx-wifi disconnect
    flx-wifi status
    flx-wifi off

It starts the supplicant with a generated config (creating
`/var/run/flx-wifi.conf` if absent), drives the association over `wpa_cli`,
then hands off to `dhcpcd`. It is a plain POSIX `sh` script staged as
`sbin/flx-wifi`. (Real 802.11 scan/associate needs physical hardware with a
supported NIC + firmware; it cannot be exercised inside QEMU.)

### Verified (2026-09-03, booted FreeLinX image)
- `flx-wifi status` runs cleanly; `flx-wifi scan` starts the supplicant.
- `flx-ifconfig eth0 inet 10.0.2.15/24` **applies the /24 prefix** — this
  fixed a libnl gotcha: `rtnl_addr_set_local()` overwrites the prefixlen
  with the parsed address's own prefix (0), so the prefix is now set on the
  parsed `nl_addr` first. A connected route (`10.0.2.0/24`) now appears and
  the default route adds successfully.
- `flx-route add default 10.0.2.2` then `ping -c 3 10.0.2.2` round-trips
  3/3 through the wired NIC.
- Wifi kernel modules load live in the running system (`iwlwifi`, `ath9k`,
  `ath10k_pci`, `brcmfmac`, `brcmsmac`) with full dependency chains
  (`ath9k_htc` -> `ath9k_common` -> `ath9k_hw` -> `ath`, `brcmfmac` ->
  `brcmutil`, ...).

## Bootstrap strategy

1. `FreeLinX/toolchain` builds clang + lld + musl and produces a sysroot.
2. Set `FREELINX_CC`/`FREELINX_LD`/`FREELINX_SYSROOT` (or let the defaults find
   the sibling repo output).
3. Start with trivial ports (base): compile single-file NetBSD utilities.
4. Bring up `netbsd-sh` next, adding minimal musl-compat patches.
5. Expand to more NetBSD userland, staging binaries for `FreeLinX/src`.
6. Bring up `sysutils/runit` for service supervision/init (see above).

## Current limitations

- No package manager / dependency solver yet (not needed at this stage).
- Some base utilities remain scaffolding metadata ports pending source
  fetch + build recipes.
- Archive checksums are intentionally unverified (`DISTINFO_SHA256=TODO`)
  for any port that hasn't yet had its real download hashed and reviewed.
- The top-level `make install PORT=...` / `scripts/install.sh` path only
  supports single-binary ports; multi-binary ports (`sysutils/runit`) need
  their installs invoked directly (see above).
