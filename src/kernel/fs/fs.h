/*
 * Copyright(C) 2011-2014 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file 
 * 
 * @brief Private file system interface.
 */

#ifndef _FS_H_
#define _FS_H_

	#include <nanvix/const.h>
	#include <nanvix/fs.h>
	#include <stdint.h>
	#include <limits.h>	
	
	/*
	 * Initializes inodes.
	 */
	EXTERN void inode_init(void);
	
/*============================================================================*
 *                               Inode Library                                *
 *============================================================================*/
 
	/*
	 * Disk inode.
	 */
	struct d_inode 
	{
		uint16_t i_mode;            /* Acess permissions.                    */
		uint16_t i_uid;             /* User id of the file's owner           */
		uint32_t i_size;            /* File size (in bytes).                 */
		uint32_t i_time;            /* Time when the file was last accessed. */
		uint8_t i_gid;              /* Group number of owner user.           */
		uint8_t i_nlinks;           /* Number of links to the file.          */
		uint16_t i_zones[NR_ZONES]; /* Zone numbers.                         */
	};
	
	/*
	 * Initializes inodes.
	 */
	EXTERN void inode_init(void);

/*============================================================================*
 *                            Super Block Library                             *
 *============================================================================*/
 	
 	/**
 	 * @defgroup Superblock
 	 */
 	/**@{*/
 	
 	/**
 	 * @brief Superblock magic number.
 	 */
 	#define SUPER_MAGIC 0x137f
 	
	/**
	 * @brief In-disk superblock.
	 */
	struct d_superblock
	{
		uint16_t s_ninodes;          /**< Number of inodes.           */
		uint16_t s_nblocks;          /**< Number of blocks.           */
		uint16_t s_imap_nblocks;     /**< Number of inode map blocks. */
		uint16_t s_bmap_nblocks;     /**< Number of zone map blocks.  */
		uint16_t s_first_data_block; /**< Unused.                     */
		uint16_t unused1;            /**< Unused.                     */
		uint32_t s_max_size;         /**< Maximum file size.          */
		uint16_t s_magic;            /**< Magic number.               */
	};
	
	/**
	 * @brief Maximum inode map size (in bytes).
	 */
	#define IMAP_SIZE (HDD_SIZE/(BLOCK_SIZE*BLOCK_SIZE*8))
	
	/**
	 * @brief Maximum zone map size (in bytes).
	 */
	#define ZMAP_SIZE (HDD_SIZE/(BLOCK_SIZE*BLOCK_SIZE*8))

	/**
	 * @brief Superblock flags.
	 */
	enum superblock_flags
	{
		SUPERBLOCK_RDONLY = 1, /**< Read only?        */
		SUPERBLOCK_LOCKED = 2, /**< Locked?           */
		SUPERBLOCK_DIRTY  = 4, /**< Dirty?            */
		SUPERBLOCK_VALID  = 8  /**< Valid superblock? */
	};
	
	/**
	 * @brief In-core superblock.
	 */
	struct superblock
	{
		unsigned count;                 /**< Reference count.              */
		struct buffer *buf;             /**< Buffer disk superblock.       */
		ino_t ninodes;                  /**< Number of inodes.             */
		struct buffer *imap[IMAP_SIZE]; /**< Inode map.                    */
		block_t imap_blocks;            /**< Number of inode map blocks.   */
		struct buffer *zmap[ZMAP_SIZE]; /**< Zone map.                     */
		block_t zmap_blocks;            /**< Number of zone map blocks.    */
		block_t first_data_block;       /**< First data block.             */
		off_t max_size;                 /**< Maximum file size.            */
		block_t zones;                  /**< Number of zones.              */
		struct inode *root;             /**< Inode for root directory.     */
		struct inode *mp;               /**< Inode mounted on.             */
		dev_t dev;                      /**< Underlying device.            */
		enum superblock_flags flags;    /**< Flags (see above).            */
		ino_t isearch;		            /**< Inodes below this are in use. */
		block_t zsearch;		        /**< Zones below this are in use.  */
		struct process *chain;          /**< Waiting chain.                */
	};
	
	/**@}*/

#endif /* _FS_H_ */
