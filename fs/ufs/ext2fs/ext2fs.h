/*
 * Copyright (c) 1997 Manuel Bouyer.
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)fs.h	8.10 (Berkeley) 10/27/94
 *  Modified for ext2fs by Manuel Bouyer.
 */

#ifndef _FS_UFS_EXT2FS_EXT2FS_H
#define _FS_UFS_EXT2FS_EXT2FS_H

#include <sys/types.h>
#include <sys/endian.h>
#include <sys/param.h>

/*
 * Each disk drive contains some number of file systems.
 * A file system consists of a number of cylinder groups.
 * Each cylinder group has inodes and data.
 *
 * A file system is described by its super-block, which in turn
 * describes the cylinder groups.  The super-block is critical
 * data and is replicated in each cylinder group to protect against
 * catastrophic loss.  This is done at `newfs' time and the critical
 * super-block data does not change, so the copies need not be
 * referenced further unless disaster strikes.
 *
 * The first boot and super blocks are given in absolute disk addresses.
 * The byte-offset forms are preferred, as they don't imply a sector size.
 */
#define BBSIZE		1024
#define SBSIZE		1024
#define	BBOFF		((off_t)(0))
#define	SBOFF		((off_t)(BBOFF + BBSIZE))
#define	BBLOCK		((daddr_t)(0))
#define	SBLOCK		((daddr_t)(BBLOCK + BBSIZE / DEV_BSIZE))

/*
 * Inodes are, like in UFS, 32-bit unsigned integers and therefore ufsino_t.
 * Disk blocks are 32-bit, if the filesystem isn't operating in 64-bit mode
 * (the incompatible ext4 64BIT flag).  More work is needed to properly use
 * daddr_t as the disk block data type on both BE and LE architectures.
 * XXX disk blocks are simply u_int32_t for now.
 */

/*
 * MINBSIZE is the smallest allowable block size.
 * MINBSIZE must be big enough to hold a cylinder group block,
 * thus changes to (struct cg) must keep its size within MINBSIZE.
 * Note that super blocks are always of size SBSIZE,
 * and that both SBSIZE and MAXBSIZE must be >= MINBSIZE.
 * FSIZE means fragment size.
 */
#define LOG_MINBSIZE	10
#define MINBSIZE	(1 << LOG_MINBSIZE)
#define LOG_MINFSIZE	10
#define MINFSIZE	(1 << LOG_MINFSIZE)

/*
 * The path name on which the file system is mounted is maintained
 * in fs_fsmnt. MAXMNTLEN defines the amount of space allocated in
 * the super block for this name.
 */
#define MAXMNTLEN	512

/*
 * Super block for an ext2fs file system.
 * NOTE: not all of them are used in AIMv6.
 */
struct ext2fs {
	uint32_t  e2fs_icount;		/* Inode count */
	uint32_t  e2fs_bcount;		/* blocks count */
	uint32_t  e2fs_rbcount;		/* reserved blocks count */
	uint32_t  e2fs_fbcount;		/* free blocks count */
	uint32_t  e2fs_ficount;		/* free inodes count */
	uint32_t  e2fs_first_dblock;	/* first data block */
	uint32_t  e2fs_log_bsize;	/* block size = 1024*(2^e2fs_log_bsize) */
	uint32_t  e2fs_log_fsize;	/* fragment size log2 */
	uint32_t  e2fs_bpg;		/* blocks per group */
	uint32_t  e2fs_fpg;		/* frags per group */
	uint32_t  e2fs_ipg;		/* inodes per group */
	uint32_t  e2fs_mtime;		/* mount time */
	uint32_t  e2fs_wtime;		/* write time */
	uint16_t  e2fs_mnt_count;	/* mount count */
	uint16_t  e2fs_max_mnt_count;	/* max mount count */
	uint16_t  e2fs_magic;		/* magic number */
#define	E2FS_MAGIC	0xef53		/* the ext2fs magic number */
	uint16_t  e2fs_state;		/* file system state */
#define	E2FS_ISCLEAN	0x01
#define	E2FS_ERRORS	0x02
	uint16_t  e2fs_beh;		/* behavior on errors */
#define E2FS_BEH_CONTINUE	1	/* continue operation */
#define E2FS_BEH_READONLY	2	/* remount fs read only */
#define E2FS_BEH_PANIC		3	/* cause panic */
#define E2FS_BEH_DEFAULT	E2FS_BEH_CONTINUE
	uint16_t  e2fs_minrev;		/* minor revision level */
	uint32_t  e2fs_lastfsck;	/* time of last fsck */
	uint32_t  e2fs_fsckintv;	/* max time between fscks */
	uint32_t  e2fs_creator;		/* creator OS */
#define E2FS_OS_LINUX 0
#define E2FS_OS_HURD  1
#define E2FS_OS_MASIX 2
	uint32_t  e2fs_rev;		/* revision level */
#define E2FS_REV0	0		/* revision levels */
#define E2FS_REV1	1		/* revision levels */
	uint16_t  e2fs_ruid;		/* default uid for reserved blocks */
	uint16_t  e2fs_rgid;		/* default gid for reserved blocks */
	/* EXT2_DYNAMIC_REV superblocks */
	uint32_t  e2fs_first_ino;	/* first non-reserved inode */
	uint16_t  e2fs_inode_size;	/* size of inode structure */
	uint16_t  e2fs_block_group_nr;	/* block grp number of this sblk*/
	/* We do not use features in AIMv6 */
	uint32_t  e2fs_features_compat; /*  compatible feature set */
	uint32_t  e2fs_features_incompat; /* incompatible feature set */
	uint32_t  e2fs_features_rocompat; /* RO-compatible feature set */
	uint8_t   e2fs_uuid[16];	/* 128-bit uuid for volume */
	char      e2fs_vname[16];	/* volume name */
	char      e2fs_fsmnt[64]; 	/* name mounted on */
	uint32_t  e2fs_algo;		/* For compression */
	uint8_t   e2fs_prealloc;	/* # of blocks to preallocate */
	uint8_t   e2fs_dir_prealloc;	/* # of blocks to preallocate for dir */
	uint16_t  e2fs_reserved_ngdb;	/* # of reserved gd blocks for resize */
	/* Ext3 JBD2 journaling.  Unused in AIMv6  */
	uint8_t   e2fs_journal_uuid[16];
	uint32_t  e2fs_journal_ino;
	uint32_t  e2fs_journal_dev;
	uint32_t  e2fs_last_orphan;	/* start of list of inodes to delete */
	uint32_t  e2fs_hash_seed[4];	/* htree hash seed */
	uint8_t   e2fs_def_hash_version;
	uint8_t   e2fs_journal_backup_type;
	uint16_t  e2fs_gdesc_size;
	uint32_t  e2fs_default_mount_opts;
	uint32_t  e2fs_first_meta_bg;
	uint32_t  e2fs_mkfs_time;
	uint32_t  e2fs_journal_backup[17];
	uint32_t  reserved2[76];
};

/* in-memory data for ext2fs */
struct m_ext2fs {
	struct ext2fs;
	uint8_t	e2fs_fsmnt_mem[MAXMNTLEN];	/* name mounted on */
	int8_t	e2fs_ronly;		/* mounted read-only flag */
	int8_t	e2fs_fmod;		/* super block modified flag */
	int32_t e2fs_fsize;		/* fragment size */
	int32_t	e2fs_bsize;		/* block size */
	int32_t e2fs_bshift;		/* ``lblkno'' calc of logical blkno */
	int32_t e2fs_bmask;		/* ``blkoff'' calc of blk offsets */
	int64_t e2fs_qbmask;		/* ~fs_bmask - for use with quad size */
	int32_t	e2fs_fsbtodb;		/* fsbtodb and dbtofsb shift constant */
	int32_t	e2fs_ncg;		/* number of cylinder groups */
	int32_t	e2fs_ngdb;		/* number of group descriptor block */
	int32_t	e2fs_ipb;		/* number of inodes per block */
	int32_t	e2fs_itpg;		/* number of inode table per group */
	off_t	e2fs_maxfilesize;	/* depends on LARGE/HUGE flags */
	struct	ext2_gd *e2fs_gd;	/* group descriptors */
};

struct ext2_gd {
	uint32_t ext2bgd_b_bitmap;	/* blocks bitmap block */
	uint32_t ext2bgd_i_bitmap;	/* inodes bitmap block */
	uint32_t ext2bgd_i_tables;	/* inodes table block  */
	uint16_t ext2bgd_nbfree;	/* number of free blocks */
	uint16_t ext2bgd_nifree;	/* number of free inodes */
	uint16_t ext2bgd_ndirs;	/* number of directories */
	uint16_t reserved;
	uint32_t reserved2[3];
};

#endif

