/*-
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 FreeLinX OS Project.
 *
 * flxpart - partition a disk for FreeLinX.
 *
 * Writes a GPT with a protective MBR, so the result boots on both BIOS and
 * UEFI from the same image.  There is a second reason it exists: the portable
 * tools for this (sgdisk, parted, fdisk) are GPL, and partitioning a disk is
 * the one step where reaching for a GPL tool feels most reasonable and is
 * most likely to be quietly relied on.  This is FreeLinX code, built with
 * clang and musl like everything else.
 *
 * The standard layout, all 1 MiB aligned:
 *
 *   0            protective MBR
 *   1            primary GPT header
 *   2-33         primary partition array
 *   34           partition 1, EFI system partition, FAT32
 *   ...          partition 2, BIOS boot, 1 MiB, for a BIOS bootloader
 *   ...          partition 3, FreeLinX root, the rest of the disk
 *   ...          backup array
 *   last-1       backup GPT header
 *
 * The BIOS boot partition is there because a BIOS bootloader has to be
 * somewhere the firmware will not overwrite and the GPT will not manage, and
 * the ESP is not it.  Limine installs into it.
 *
 * Nothing here reads or writes a filesystem.  It draws a partition table and
 * stops; mkfs and the system copy are separate steps, so a failure after this
 * point leaves a disk that is partitioned and empty rather than half
 * overwritten.
 *
 * Destructive: --create-standard writes a fresh table over whatever was there.
 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __linux__
#include <linux/fs.h>		/* BLKRRPART */
#endif

#define	SECTOR		512
#define	ALIGN		(1024 * 1024)	/* 1 MiB */
#define	MB		(1024 * 1024)

#define	GPT_ENTRIES	128
#define	GPT_ENTRY_SIZE	128
#define	ARRAY_BYTES	(GPT_ENTRIES * GPT_ENTRY_SIZE)
#define	ARRAY_SECTORS	(ARRAY_BYTES / SECTOR)

/* The primary array sits at LBA 2 and runs for 32 sectors, so it ends at LBA
 * 33 and the first partition may start at 34.  These are different numbers
 * for different things and keeping them apart is the whole point of naming
 * both. */
#define	PRIMARY_ARRAY_LBA	2

/* The first LBA a partition may use: past the protective MBR at 0, the
 * primary header at 1, and the primary array at 2-33. */
#define	FIRST_USABLE	34

/* Per UEFI 2.10: the backup array occupies the last 33 sectors, so the last
 * usable LBA is the last LBA minus 33.  Getting this wrong is not cosmetic:
 * a partition that runs into the backup array makes the whole table read as
 * corrupt, and a disk with an unreadable partition table is a disk nobody
 * can boot. */
#define	RESERVED_TAIL	33

/* Partition type GUIDs, in the mixed-endian order they are stored in on disk:
 * Data1 and Data2 and Data3 are little-endian, Data4 and Data5 big-endian.
 *
 * Every one of these three had at least one wrong byte when they were written
 * out by hand as byte arrays, which is not a thing to do twice.  The values now
 * read back, through guid_text and through a real GPT reader, as:
 *
 *   ESP        C12A7328-F81F-11D2-BA4B-00A0C93EC93B
 *   BIOS boot  21686148-6449-6E6F-744E-656564454649
 *   root       0FC63DAF-8483-4772-8E79-3D69D8477DE4
 *
 * and test-flxpart.sh checks all three, byte for byte, because nothing else
 * notices.  A wrong ESP type is invisible until firmware declines to mount the
 * partition it was told was an ESP; a wrong BIOS boot type stops limine at:
 *
 *   error: Chosen partition for BIOS boot code is not of BIOS boot partition type.
 */

/* EFI System Partition: C12A7328-F81F-11D2-BA4B-00A0C93EC93B */
static const unsigned char GUID_ESP[16] = {
	0x28,0x73,0x2a,0xc1, 0x1f,0xf8, 0xd2,0x11,
	0xba,0x4b, 0x00,0xa0, 0xc9,0x3e,0xc9,0x3b
};
/* BIOS Boot Partition: 21686148-6449-6E6F-744E-656564454649.  Limine
 * bios-install reads this type and refuses to install without it, which it can
 * tell from the type alone - so a wrong type here means the BIOS stages are
 * never written even though the partition exists, is 1 MiB, and is named "BIOS
 * boot". */
static const unsigned char GUID_BIOSBOOT[16] = {
	0x48,0x61,0x68,0x21, 0x49,0x64, 0x6f,0x6e,
	0x74,0x4e, 0x65,0x65, 0x64,0x45,0x46,0x49
};
/* Linux filesystem data: 0FC63DAF-8483-4772-8E79-3D69D8477DE4.  The bytes
 * here used to be 4F68EE06-F53D-D74B-1193-47F89EF89EF8, an unregistered value
 * that matched nothing including this file's own header comment. */
static const unsigned char GUID_LINUX_ROOT[16] = {
	0xaf,0x3d,0xc6,0x0f, 0x83,0x84, 0x72,0x47,
	0x8e,0x79, 0x3d,0x69, 0xd8,0x47,0x7d,0xe4
};

static const char *prog;
static int quiet;

/* --- reporting ------------------------------------------------------------ */

static void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s: ", prog);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
	exit(1);
}

static void
say(const char *fmt, ...)
{
	va_list ap;

	if (quiet)
		return;
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	fputc('\n', stdout);
}

/* read_le64 - a little-endian 64-bit read, for the header fields. */
static uint64_t
read_le64(const unsigned char *p)
{
	uint64_t v = 0;
	int i;

	for (i = 7; i >= 0; i--)
		v = (v << 8) | p[i];
	return v;
}

/* random_guid - a version 4, RFC 4122 variant GUID, so a reader that
 * validates one does not reject the table it just wrote. */
static void
random_guid(unsigned char out[16])
{
	unsigned char b[16];
	size_t got = 0;
	int fd = open("/dev/urandom", O_RDONLY);

	if (fd < 0)
		die("cannot open /dev/urandom: %s", strerror(errno));
	while (got < sizeof b) {
		ssize_t r = read(fd, b + got, sizeof b - got);

		if (r < 0) {
			if (errno == EINTR)
				continue;
			die("cannot read /dev/urandom: %s", strerror(errno));
		}
		if (r == 0)
			die("/dev/urandom ended early");
		got += (size_t)r;
	}
	close(fd);
	memcpy(out, b, 16);
	out[7] = (unsigned char)((out[7] & 0x0f) | 0x40);
	out[8] = (unsigned char)((out[8] & 0x3f) | 0x80);
}

/* --- byte writers --------------------------------------------------------- */

static void
put_le32(unsigned char *p, uint32_t v)
{
	p[0] = (unsigned char)v;
	p[1] = (unsigned char)(v >> 8);
	p[2] = (unsigned char)(v >> 16);
	p[3] = (unsigned char)(v >> 24);
}

static void
put_le64(unsigned char *p, uint64_t v)
{
	int i;

	for (i = 0; i < 8; i++)
		p[i] = (unsigned char)(v >> (8 * i));
}

/* guid_text - the canonical 8-4-4-4-12 form, for the output keys.  An earlier
 * version of this printed six groups, which is not a GUID in any notation
 * anybody uses. */
static void
guid_text(const unsigned char g[16], char out[37])
{
	snprintf(out, 37,
	    "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
	    g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7],
	    g[8], g[9], g[10], g[11], g[12], g[13], g[14], g[15]);
}

/* --- CRC-32 (IEEE) -------------------------------------------------------- */

static uint32_t
crc32_ieee(const unsigned char *buf, size_t len)
{
	static uint32_t table[256];
	static int ready;
	uint32_t crc;
	size_t i;
	int j;

	if (!ready) {
		for (i = 0; i < 256; i++) {
			crc = (uint32_t)i;
			for (j = 0; j < 8; j++)
				crc = (crc & 1) ?
				    (crc >> 1) ^ 0xEDB88320u : crc >> 1;
			table[i] = crc;
		}
		ready = 1;
	}

	crc = 0xFFFFFFFFu;
	for (i = 0; i < len; i++)
		crc = table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFFu;
}

/* --- the partition table -------------------------------------------------- */

#define	MAX_PARTS	16

struct part {
	const unsigned char *type;
	uint64_t		first;
	uint64_t		last;		/* inclusive */
	unsigned char		guid[16];
	const char		*name;
};

struct ptable {
	struct part	part[MAX_PARTS];
	int		npart;
	uint64_t	total_sectors;
	uint64_t	first_usable;
	uint64_t	last_usable;
	unsigned char	disk_guid[16];
};

static int
add_part(struct ptable *t, const unsigned char *type, uint64_t first,
    uint64_t last, const char *name)
{
	struct part *p;

	if (t->npart >= MAX_PARTS)
		die("more than %d partitions", MAX_PARTS);
	p = &t->part[t->npart++];
	p->type = type;
	p->first = first;
	p->last = last;
	p->name = name;
	/* A unique per-partition GUID, distinct from the disk GUID.  GPT
	 * requires one and some tools complain when it is zero. */
	(void)random_guid(p->guid);
	return t->npart - 1;
}

/* build_array - the 128 partition entries.
 *
 * memset first, every time.  The unused entries have to read as zero, and an
 * uninitialised buffer invents ~125 garbage partitions that a reader will
 * happily report as real ones. */
static void
build_array(const struct ptable *t, unsigned char *arr)
{
	int i, j;

	memset(arr, 0, ARRAY_BYTES);
	for (i = 0; i < t->npart; i++) {
		unsigned char *e = arr + i * GPT_ENTRY_SIZE;

		memcpy(e, t->part[i].type, 16);
		memcpy(e + 16, t->part[i].guid, 16);
		put_le64(e + 32, t->part[i].first);
		put_le64(e + 40, t->part[i].last);
		put_le64(e + 48, 0);			/* attributes */
		/* The name, UTF-16LE, as GPT stores it. */
		for (j = 0; t->part[i].name[j] != '\0' && j < 36; j++) {
			e[56 + j * 2] = (unsigned char)t->part[i].name[j];
			e[57 + j * 2] = 0;
		}
	}
}

static void
build_header(unsigned char *h, const struct ptable *t, uint64_t current,
    uint64_t backup, uint64_t array_lba, uint32_t array_crc)
{
	memset(h, 0, SECTOR);
	memcpy(h, "EFI PART", 8);
	put_le32(h + 8, 0x00010000u);		/* revision 1.0 */
	put_le32(h + 12, 92);			/* header size */
	put_le32(h + 16, 0);			/* CRC, filled in below */
	put_le32(h + 20, 0);			/* reserved */
	put_le64(h + 24, current);
	put_le64(h + 32, backup);
	put_le64(h + 40, t->first_usable);
	put_le64(h + 48, t->last_usable);
	memcpy(h + 56, t->disk_guid, 16);
	put_le64(h + 72, array_lba);
	put_le32(h + 80, GPT_ENTRIES);
	put_le32(h + 84, GPT_ENTRY_SIZE);
	put_le32(h + 88, array_crc);
	/* The header CRC covers the first 92 bytes with the CRC field zero,
	 * which it is at this point. */
	put_le32(h + 16, crc32_ieee(h, 92));
}

/* build_mbr - the protective MBR.
 *
 * The partition entry starts at byte 446, not at 0x1a: 446 is the offset of
 * the first partition record within the MBR, and writing the type byte at
 * 0x1a instead puts a 0xEE in the middle of the disk signature area, where no
 * reader looks for it and no tool will tell you it is wrong. */
static void
build_mbr(unsigned char *mbr, uint64_t total)
{
	uint64_t sectors = total - 1;

	memset(mbr, 0, SECTOR);
	/* A disk with more sectors than a 32-bit count can hold reports
	 * 0xFFFFFFFF, which is what the spec says to do; the GPT is
	 * authoritative anyway. */
	if (sectors > 0xFFFFFFFFu)
		sectors = 0xFFFFFFFFu;
	mbr[446 + 0] = 0x00;			/* not bootable */
	mbr[446 + 1] = 0x00;			/* starting head */
	mbr[446 + 2] = 0x02;			/* starting sector */
	mbr[446 + 3] = 0x00;			/* starting cylinder */
	mbr[446 + 4] = 0xEE;			/* GPT protective */
	mbr[446 + 5] = 0xFF;			/* ending head */
	mbr[446 + 6] = 0xFF;			/* ending sector */
	mbr[446 + 7] = 0xFF;			/* ending cylinder */
	put_le32(mbr + 446 + 8, 1);		/* first LBA */
	put_le32(mbr + 446 + 12, (uint32_t)sectors);
	mbr[510] = 0x55;
	mbr[511] = 0xAA;
}

/* --- writing -------------------------------------------------------------- */

struct dev {
	int	fd;
	uint64_t	size;		/* in bytes */
	const char *path;
	int	is_file;		/* a regular file, not a block device */
};

static void
dev_open(struct dev *d, const char *path, int write)
{
	struct stat st;
	int flags = write ? O_RDWR : O_RDONLY;

	memset(d, 0, sizeof *d);
	d->path = path;
	d->fd = open(path, flags);
	if (d->fd < 0)
		die("cannot open %s: %s", path, strerror(errno));
	if (fstat(d->fd, &st) < 0)
		die("cannot stat %s: %s", path, strerror(errno));

	if (S_ISBLK(st.st_mode)) {
		uint64_t bytes = 0;
		int i;

		d->is_file = 0;
#ifdef BLKGETSIZE64
		if (ioctl(d->fd, BLKGETSIZE64, &bytes) < 0)
			die("cannot get the size of %s: %s", path,
			    strerror(errno));
		d->size = bytes;
#else
		die("BLKGETSIZE64 is not available; cannot size a block device");
#endif
		(void)i;
	} else if (S_ISREG(st.st_mode)) {
		/* A plain file, which is how the tests exercise this without
		 * a disk: everything above the block size is the same. */
		d->is_file = 1;
		d->size = (uint64_t)st.st_size;
	} else {
		die("%s is neither a block device nor a regular file", path);
	}
}

static void
dev_close(struct dev *d)
{
	if (d->fd >= 0)
		close(d->fd);
	d->fd = -1;
}

/* dev_write - write one sector's worth at a sector offset. */
static void
dev_write_sector(struct dev *d, uint64_t lba, const unsigned char *buf)
{
	off_t off = (off_t)(lba * SECTOR);
	size_t done = 0;

	if (lba * SECTOR >= d->size)
		die("refusing to write sector %" PRIu64 ": past the end of %s",
		    lba, d->path);

	while (done < SECTOR) {
		ssize_t w = pwrite(d->fd, buf + done, SECTOR - done,
		    off + (off_t)done);

		if (w < 0) {
			if (errno == EINTR)
				continue;
			die("write at sector %" PRIu64 " of %s: %s", lba,
			    d->path, strerror(errno));
		}
		done += (size_t)w;
	}
}

static void
dev_read_sector(struct dev *d, uint64_t lba, unsigned char *buf)
{
	off_t off = (off_t)(lba * SECTOR);
	size_t done = 0;

	if (lba * SECTOR + SECTOR > d->size) {
		/* Short read past the end reads as zeroes rather than being
		 * an error: a --show on a fresh image of the wrong size
		 * should say "no table", not crash. */
		memset(buf, 0, SECTOR);
		return;
	}
	while (done < SECTOR) {
		ssize_t r = pread(d->fd, buf + done, SECTOR - done,
		    off + (off_t)done);

		if (r < 0) {
			if (errno == EINTR)
				continue;
			die("read at sector %" PRIu64 " of %s: %s", lba,
			    d->path, strerror(errno));
		}
		if (r == 0) {
			memset(buf + done, 0, SECTOR - done);
			return;
		}
		done += (size_t)r;
	}
}

/* reread - tell the kernel the table changed.  Without this a partitioner
 * writes the table and the kernel keeps using the one it had, so the new
 * partitions do not appear until the machine is rebooted.  Not applicable to
 * a regular file. */
static void
dev_reread(struct dev *d)
{
#ifdef BLKRRPART
	if (d->is_file)
		return;
	if (ioctl(d->fd, BLKRRPART) < 0) {
		/* EBUSY is normal when the disk is mounted or in use, and it
		 * is not fatal: the table is written either way. */
		if (errno == EBUSY) {
			say("%s is in use; the new table takes effect on reboot",
			    d->path);
			return;
		}
		die("cannot ask the kernel to reread %s: %s", d->path,
		    strerror(errno));
	}
#else
	(void)d;
#endif
}

/* --- the operations ------------------------------------------------------- */

/* write_table - put the GPT on the device, or compute the arrays and write
 * nothing when dry_run is set.  report_table - print the result in a form a
 * caller can parse.  create_standard - the layout for a disk that boots.
 * create_data - the layout for a disk that only holds state.
 *
 * Declared here because create_standard calls the first two and sits above
 * them: they are its tail, factored out so that a second layout cannot come to
 * disagree with the first about where the array goes. */
static void write_table(struct dev *d, struct ptable *t, int dry_run);
static void report_table(struct ptable *t, uint64_t total, int dry_run);
static int create_standard(struct dev *d, unsigned long esp_mb, int dry_run);
static int create_data(struct dev *d, int dry_run);

/* create_standard - the FreeLinX layout on an already-sized device.
 *
 * dry_run computes and reports the layout without writing a byte, which is
 * what an installer needs in order to show a plan on a machine where the
 * answer is not yet a partitioned disk.  The geometry is the real one: the
 * same code and the same arithmetic, only the writes are skipped. */
static int
create_standard(struct dev *d, unsigned long esp_mb, int dry_run)
{
	struct ptable t;
	uint64_t total, first_free;
	uint64_t esp_sectors, bios_sectors;
	int n;

	memset(&t, 0, sizeof t);
	total = d->size / SECTOR;
	if (total < FIRST_USABLE + RESERVED_TAIL + 64)
		die("%s is only %" PRIu64 " sectors; too small for a GPT with any "
		    "room in it", d->path, total);

	/* Per UEFI 2.10 the backup array and header take the last 33
	 * sectors, so the last usable LBA is 33 before the last one. */
	t.total_sectors = total;
	t.first_usable = FIRST_USABLE;
	t.last_usable = total - 1 - RESERVED_TAIL;

	if (t.last_usable <= t.first_usable)
		die("%s is too small to hold a partition", d->path);

	esp_sectors = (uint64_t)esp_mb * MB / SECTOR;
	/* The BIOS boot partition is 1 MiB, the traditional size, and is
	 * where a BIOS bootloader goes. */
	bios_sectors = MB / SECTOR;

	/* Work out where every partition ends before writing anything.
	 *
	 * This has to happen first.  An earlier version checked only that the
	 * disk was big enough to hold a GPT, then laid the partitions out and
	 * wrote as it went, so a disk too small for the requested ESP was
	 * written up to the point of running out of room and left with an
	 * MBR and a partition table describing two partitions and no third.
	 * That is worse than leaving the disk alone: a table that is
	 * half-written and internally consistent reads as a disk with a
	 * small root, and the next thing that touches it may "fix" it. */
	first_free = (t.first_usable + ALIGN / SECTOR - 1) &
	    ~((uint64_t)(ALIGN / SECTOR) - 1);
	if (first_free + esp_sectors > t.last_usable)
		die("%s is %" PRIu64 " MiB, which cannot hold a %lu MiB EFI "
		    "system partition plus a BIOS boot partition and a root "
		    "partition. Use a smaller --esp-size, or a larger disk.",
		    d->path, total * SECTOR / MB, esp_mb);
	if (first_free + esp_sectors + bios_sectors >= t.last_usable)
		die("%s has no room left for a root partition after the %lu MiB "
		    "EFI system partition and the BIOS boot partition",
		    d->path, esp_mb);

	/* Past this point the layout is known to fit. */
	random_guid(t.disk_guid);

	/* 1. ESP */
	n = add_part(&t, GUID_ESP, first_free, first_free + esp_sectors - 1,
	    "EFI system");
	say("partition %d: EFI system, %lu MiB, LBA %" PRIu64 "-%" PRIu64,
	    n + 1, esp_mb, t.part[n].first, t.part[n].last);

	/* 2. BIOS boot */
	first_free += esp_sectors;
	n = add_part(&t, GUID_BIOSBOOT, first_free,
	    first_free + bios_sectors - 1, "BIOS boot");
	say("partition %d: BIOS boot, 1 MiB, LBA %" PRIu64 "-%" PRIu64,
	    n + 1, t.part[n].first, t.part[n].last);

	/* 3. root, to the end.  The tail is already excluded by
	 * last_usable, so this cannot run into the backup array. */
	first_free += bios_sectors;
	if (first_free > t.last_usable)
		die("%s has no room left for a root partition after the ESP "
		    "and the BIOS boot partition", d->path);
	n = add_part(&t, GUID_LINUX_ROOT, first_free, t.last_usable, "FreeLinX");
	say("partition %d: FreeLinX root, %" PRIu64 " MiB, LBA %" PRIu64
	    "-%" PRIu64, n + 1,
	    (uint64_t)((t.part[n].last - t.part[n].first + 1) * SECTOR) / MB,
	    t.part[n].first, t.part[n].last);

	write_table(d, &t, dry_run);
	report_table(&t, total, dry_run);
	return 0;
}

/* write_table - put the GPT on the device, or compute nothing but the arrays
 * when dry_run is set.
 *
 * Shared by every layout so that a second layout cannot come to disagree with
 * the first about where the array goes, which is the mistake the LBA 2 comment
 * in the body of this function is about. */
static void
write_table(struct dev *d, struct ptable *t, int dry_run)
{
	unsigned char arr[ARRAY_BYTES];
	unsigned char sector[SECTOR];
	uint32_t array_crc;
	uint64_t total = t->total_sectors;

	build_array(t, arr);
	array_crc = crc32_ieee(arr, ARRAY_BYTES);

	if (!dry_run) {
		/* The MBR. */
		build_mbr(sector, total);
		dev_write_sector(d, 0, sector);

		/* Primary header and array.
		 *
		 * The array goes at LBA 2, not at FIRST_USABLE.  FIRST_USABLE
		 * is 34, which is where the first *partition* may start, and
		 * confusing the two puts the first sector at LBA 2 and the
		 * remaining 31 at LBA 35-65, leaving 3-33 empty.  fdisk still
		 * reads that, because it walks the partition entries and the
		 * header CRC covers only the header; parted compares the
		 * array against the CRC in the header and reports the primary
		 * table as corrupt, then falls back to the backup.  The disk
		 * works on fdisk and not on parted, which is the worst kind of
		 * wrong. */
		build_header(sector, t, 1, total - 1, PRIMARY_ARRAY_LBA,
		    array_crc);
		dev_write_sector(d, 1, sector);
		{
			int i;

			for (i = 0; i < ARRAY_SECTORS; i++) {
				memcpy(sector, arr + i * SECTOR, SECTOR);
				dev_write_sector(d,
				    (uint64_t)PRIMARY_ARRAY_LBA + i, sector);
			}
		}

		/* Backup array, then backup header. */
		{
			uint64_t arr_lba = total - 1 - ARRAY_SECTORS;
			int i;

			for (i = 0; i < ARRAY_SECTORS; i++) {
				memcpy(sector, arr + i * SECTOR, SECTOR);
				dev_write_sector(d, arr_lba + (uint64_t)i,
				    sector);
			}
			build_header(sector, t, total - 1, 1, arr_lba,
			    array_crc);
			dev_write_sector(d, total - 1, sector);
		}

		dev_reread(d);
	}
}

/* report_table - print the layout in a form a caller can parse, so an installer
 * does not have to read this program's human output. */
static void
report_table(struct ptable *t, uint64_t total, int dry_run)
{
	int n;

	for (n = 0; n < t->npart; n++) {
		char g[37];

		guid_text(t->part[n].type, g);
		printf("FLX_PART%d_TYPE=%s\n", n + 1, g);
		guid_text(t->part[n].guid, g);
		printf("FLX_PART%d_GUID=%s\n", n + 1, g);
		printf("FLX_PART%d_FIRST=%" PRIu64 "\n", n + 1, t->part[n].first);
		printf("FLX_PART%d_LAST=%" PRIu64 "\n", n + 1, t->part[n].last);
		printf("FLX_PART%d_SIZE_BYTES=%" PRIu64 "\n", n + 1,
		    (t->part[n].last - t->part[n].first + 1) * SECTOR);
		printf("FLX_PART%d_NAME=%s\n", n + 1, t->part[n].name);
	}
	printf("FLX_DISK_SECTORS=%" PRIu64 "\n", total);
	printf("FLX_DISK_FIRST_USABLE=%" PRIu64 "\n", t->first_usable);
	printf("FLX_DISK_LAST_USABLE=%" PRIu64 "\n", t->last_usable);
	if (dry_run)
		printf("FLX_DRY_RUN=1\n");
}

/* create_data - the layout for a system that runs from RAM.
 *
 * One partition, the whole disk, and no boot chain at all.
 *
 * There is no EFI system partition and no BIOS boot partition here on purpose,
 * not as an omission.  This layout is for a disk that holds state, not a
 * system: the kernel and the boot chain live on the medium the machine booted
 * from, every boot, and what this disk is for is surviving a reboot.  Laying
 * out 256 MiB of ESP and a megabyte of BIOS boot on it would be 257 MiB of
 * disk given to a bootloader that is never installed and never run.
 *
 * The partition carries the ordinary Linux root type GUID rather than one of
 * its own, because that is what it is: a Linux filesystem partition that
 * happens to be mounted at /var.  What makes it the data partition is the
 * filesystem label the installer writes on it, and the boot code looks for that
 * label and not for the type GUID - a GUID cannot say "this is /var" without
 * every other Linux partition on the machine claiming the same thing.
 */
static int
create_data(struct dev *d, int dry_run)
{
	struct ptable t;
	uint64_t total;
	int n;

	memset(&t, 0, sizeof t);
	total = d->size / SECTOR;
	if (total < FIRST_USABLE + RESERVED_TAIL + 64)
		die("%s is only %" PRIu64 " sectors; too small for a GPT with any "
		    "room in it", d->path, total);

	t.total_sectors = total;
	t.first_usable = FIRST_USABLE;
	t.last_usable = total - 1 - RESERVED_TAIL;

	if (t.last_usable <= t.first_usable)
		die("%s is too small to hold a partition", d->path);

	random_guid(t.disk_guid);

	n = add_part(&t, GUID_LINUX_ROOT, t.first_usable, t.last_usable,
	    "FreeLinX data");
	say("partition %d: FreeLinX data, %" PRIu64 " MiB, LBA %" PRIu64
	    "-%" PRIu64, n + 1,
	    (t.part[n].last - t.part[n].first + 1) * SECTOR / MB,
	    t.part[n].first, t.part[n].last);

	write_table(d, &t, dry_run);
	report_table(&t, total, dry_run);
	return 0;
}

/* show - read the table back and print it.  Reads, never writes. */
static void
show(struct dev *d)
{
	unsigned char sector[SECTOR];
	unsigned char arr[ARRAY_BYTES];
	uint64_t array_lba, total;
	int i, n = 0;

	total = d->size / SECTOR;

	/* A GPT disk always starts with a protective MBR; if it does not,
	 * saying so is more useful than printing a header full of zeroes. */
	dev_read_sector(d, 0, sector);
	if (sector[510] != 0x55 || sector[511] != 0xAA) {
		printf("no MBR signature: %s is not partitioned GPT\n", d->path);
		return;
	}
	if (sector[446 + 4] != 0xEE) {
		printf("%s has an MBR but no protective partition; not a GPT disk\n",
		    d->path);
		return;
	}

	dev_read_sector(d, 1, sector);
	if (memcmp(sector, "EFI PART", 8) != 0) {
		printf("%s: no GPT header at LBA 1\n", d->path);
		return;
	}

	array_lba = 0;
	for (i = 0; i < 8; i++)
		array_lba |= (uint64_t)sector[72 + i] << (8 * i);

	printf("%s: %" PRIu64 " sectors, GPT\n", d->path, total);
	printf("  first usable LBA %" PRIu64 "\n", read_le64(sector + 40));
	printf("  last usable LBA  %" PRIu64 "\n", read_le64(sector + 48));
	printf("  array at LBA %" PRIu64 "\n", array_lba);

	if (array_lba == 0 || array_lba + ARRAY_SECTORS > total) {
		printf("  array is outside the disk; the table is unreadable\n");
		return;
	}

	for (i = 0; i < ARRAY_SECTORS; i++)
		dev_read_sector(d, array_lba + (uint64_t)i, arr + i * SECTOR);

	for (i = 0; i < GPT_ENTRIES; i++) {
		const unsigned char *e = arr + i * GPT_ENTRY_SIZE;
		uint64_t first, last;
		int j, empty = 1;
		char type[37], name[64];

		/* An all-zero entry is unused.  A GUID of all zeroes in a
		 * live entry is the "unknown type" GUID, which is not
		 * empty, so this is the right test. */
		for (j = 0; j < 16; j++)
			if (e[j] != 0)
				empty = 0;
		if (empty)
			continue;

		first = read_le64(e + 32);
		last = read_le64(e + 40);
		guid_text(e, type);

		/* UTF-16LE, up to 36 characters. */
		for (j = 0; j < 36 && e[56 + j * 2] != 0; j++)
			name[j] = (char)e[56 + j * 2];
		name[j] = '\0';

		printf("  %d: %-12s first %-10" PRIu64 " last %-10" PRIu64
		    " (%8.1f MiB)  %s\n", ++n, name, first, last,
		    (double)((last - first + 1) * SECTOR) / (1024.0 * 1024.0),
		    type);
	}
	if (n == 0)
		printf("  the table has no partitions in it\n");
}

static void
usage_text(FILE *f)
{
	fprintf(f,
	    "usage: %s --create-standard [--esp-size MB] [--dry-run] DEVICE\n"
	    "       %s --create-data [--dry-run] DEVICE\n"
	    "       %s --show DEVICE\n"
	    "       %s --help | --version\n"
	    "\n"
	    "Writes a GPT with a protective MBR.\n"
	    "\n"
	    "--create-standard lays out a disk that boots the system:\n"
	    "  1  EFI system   FAT32, 256 MiB by default, the UEFI boot target\n"
	    "  2  BIOS boot    1 MiB, where a BIOS bootloader goes\n"
	    "  3  FreeLinX     the rest of the disk, and the system on it\n"
	    "\n"
	    "--create-data lays out a disk that only holds state:\n"
	    "  1  FreeLinX data  the whole disk\n"
	    "\n"
	    "  No EFI system partition and no BIOS boot partition, because\n"
	    "  nothing boots from this disk: the kernel and the initramfs are on\n"
	    "  the medium the machine booted from, every boot.  The partition is\n"
	    "  meant to be formatted and labelled FREELINX_VAR, and the boot code\n"
	    "  finds it by that label and mounts it at /var.\n"
	    "\n"
	    "  --dry-run      report the layout without writing anything\n"
	    "\n"
	    "Both create modes erase any existing partition table.  With --dry-run\n"
	    "nothing is written, so it is safe to run to see what a disk would be\n"
	    "given.\n",
	    prog, prog, prog, prog);
}

static void
usage(void)
{
	usage_text(stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	int create_standard_mode = 0, create_data_mode = 0;
	int do_show = 0, help = 0, dry_run = 0;
	unsigned long esp_mb = 256;
	const char *devpath = NULL;
	struct dev d;
	int i;

	prog = argv[0];
	if (prog == NULL)
		prog = "flxpart";

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--create-standard") == 0)
			create_standard_mode = 1;
		else if (strcmp(argv[i], "--create-data") == 0)
			create_data_mode = 1;
		else if (strcmp(argv[i], "--show") == 0)
			do_show = 1;
		else if (strcmp(argv[i], "--esp-size") == 0 && i + 1 < argc)
			esp_mb = strtoul(argv[++i], NULL, 10);
		else if (strcmp(argv[i], "--dry-run") == 0)
			dry_run = 1;
		else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
			quiet = 1;
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			help = 1;
		else if (strcmp(argv[i], "--version") == 0) {
			printf("flxpart 1.0\n");
			return 0;
		} else if (argv[i][0] == '-') {
			fprintf(stderr, "%s: unknown option: %s\n", prog, argv[i]);
			usage();
		} else if (devpath == NULL) {
			devpath = argv[i];
		} else {
			fprintf(stderr, "%s: only one device can be given\n", prog);
			usage();
		}
	}

	/* --help first: asking for help is not an error and must not need a
	 * device to be named. */
	if (help) {
		usage_text(stdout);
		return 0;
	}

	if (devpath == NULL)
		usage();
	if (create_standard_mode && create_data_mode) {
		/* Both would write a table, and they write different ones.
		 * Picking the one that was named first would be a coin toss
		 * on the back of a working disk, so this is refused rather
		 * than resolved. */
		fprintf(stderr, "%s: --create-standard and --create-data are "
		    "different layouts and cannot both be asked for\n", prog);
		usage();
	}
	if ((create_standard_mode || create_data_mode) && do_show) {
		fprintf(stderr, "%s: writing a layout and --show are "
		    "different jobs\n", prog);
		usage();
	}
	if (!create_standard_mode && !create_data_mode && !do_show) {
		fprintf(stderr, "%s: nothing to do; pass --create-standard, "
		    "--create-data or --show\n", prog);
		usage();
	}
	if (esp_mb == 0)
		die("--esp-size must be at least 1");

	if (do_show) {
		dev_open(&d, devpath, 0);
		show(&d);
		dev_close(&d);
		return 0;
	}

	/* Before anything is written, check it is big enough.  Discovering
	 * that after a partial write means a disk with a half-written table,
	 * which is worse than a disk with none. */
	{
		struct stat st;

		if (stat(devpath, &st) == 0 && S_ISBLK(st.st_mode) == 0 &&
		    S_ISREG(st.st_mode) == 0)
			die("%s is neither a block device nor a regular file",
			    devpath);
	}

	dev_open(&d, devpath, dry_run ? 0 : 1);
	say("%s %s (%" PRIu64 " bytes)", dry_run ? "planning" : "partitioning",
	    devpath, d.size);
	if (create_standard_mode) {
		if (create_standard(&d, esp_mb, dry_run) != 0)
			return 1;
	} else {
		if (create_data(&d, dry_run) != 0)
			return 1;
	}
	dev_close(&d);
	return 0;
}
