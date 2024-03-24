#include <stdio.h>
#include <string.h>
/* filesys.c - Functions headers and structs for supporting operations on the file system image
 * vim:ts=4 noexpandtab
 */

#ifndef FILESYS_H
#define FILESYS_H

/* Defining File System Structure from Week 9 Discussion Slide 13 */
/* Check if unsigned or signed for data block */
/* File system structure, bottom middle */
typedef struct directory_entry_t{
    char filename[32]; // 32B size comes from the maximum possible length of a line
    uint32_t file_type; // 4B size
    uint32_t inode_number; // inode #, 4B size
    uint8_t reserve[24]; // 24B reserved
} directory_entry_t;

/* Boot block structure, left */
typedef struct boot_block_t{
    uint32_t dir_entries_count; // # dir. entries, 4B size
    uint32_t inodes; // # inodes (N), 4B size
    uint32_t data_blocks; // # data blocks (D), 4B size
    uint8_t reserved[52]; // 52B reserved
    directory_entry_t dir_entries[63]; // dir. entries, 63B size (first 64 bytes is not for directory entries)
} boot_block_t;

/* inode structure, right */
typedef struct inode_t{
    uint32_t length; // length in B, 4B size
    uint32_t data_blocks[1023]; // length of all data blocks, 1023B size
    /* 1023 comes from the ((4096B total size / 4kB per block) - 1 to be inclusive) */
} inode_t;

/* File function headers */
uint32_t open_file(const uint8_t* filename);
uint32_t close_file(int32_t file_directory);
uint32_t write_file(int32_t file_directory, const void* buffer, int32_t size);
// INSERT READ_FILE HERE

/* Directory function headers */
uint32_t open_directory(const uint8_t* filename);
uint32_t close_directory(int32_t file_directory);
uint32_t write_directory(int32_t file_directory, const void* buffer, int32_t size);
// INSERT READ_DIRECTORY HERE

/* read  data functions */
int32_t read_dentry_by_name(const uint8_t* fname, directory_entry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, directory_entry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

directory_entry_t* dentry_block;
boot_block_t* boot_block;
inode_t* inode;


#endif //FILESYS_H
