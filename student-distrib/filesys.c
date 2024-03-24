#include <stdio.h>
#include <string.h>
/* filesys.c - Functions for supporting operations on the file system image
 * vim:ts=4 noexpandtab
 */

#include "filesys.h"

/* General Driver Functions */

/* void init_file_system()
 *  Functionality: 
 *  Arguments: 
 *  Return: None
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
void init_file_sys(uint32_t filesys_ptr) {
    boot_block = (boot_block_t*)filesys_ptr;
    dentry_block = boot_block->dir_entries;
    inode = (inode_t *)(boot_block + 1);
}

/* File functions */

/* uint32_t open_file()
 *  Functionality: Initialize any temporary structures
 *  Arguments: const uint8_t* filename - file name that needs to be opened
 *  Return: Return 0, Return -1 for failure
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t open_file(const uint8_t* filename)
{
    /* We want to use read_dentry_by_name to establish that we are opening a valid directory */
    directory_entry_t temp; // temp var to hold directory_entry
    if(read_dentry_by_name(filename, &temp)) // check entry value, if != 0 then return failure
    {
        return -1;
    }
    return 0;
}

/* uint32_t close_file()
 *  Functionality: Undo what you did in the open function
 *  Arguments: int32_t file_directory - file directory that needs to be closed
 *  Return: Return 0
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t close_file(int32_t file_directory)
{
    return 0;
}

/* uint32_t write_file()
 *  Functionality: Should do nothing
 *  Arguments: int32_t file_directory - directory we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *  Return: Return -1
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t write_file(int32_t file_directory, const void* buffer, int32_t size)
{
    return -1;
}

/* uint32_t read_file()
 *  Functionality: reads count bytes of data from file into buf, uses read_data
 *  Arguments: 
 *  Return:
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t read_file(int32_t file_directory, const void* buffer, int32_t size)
{
    /* TODO */
}

/* Directory functions */

/* uint32_t open_directory()
 *  Functionality: Opens a directory file (note file types) uses read_dentry_by_name
 *  Arguments: int32_t file_directory - file directory that needs to be closed
 *  Return: Return 0, or -1 upon failure from read_dentry_by_name
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t open_directory(const uint8_t* filename)
{
    /* We want to use read_dentry_by_name to establish that we are opening a valid directory */
    directory_entry_t temp; // temp var to hold directory_entry
    if(!read_dentry_by_name(filename, &temp)) // check entry value, if == 0 then return failure
    {
        return -1;
    }
    return 0;
}

/* uint32_t close_directory()
 *  Functionality: Probably does nothing
 *  Arguments: int32_t file_directory - file directory that needs to be closed
 *  Return: Return 0
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t close_directory(int32_t file_directory)
{
    return 0;
}

/* uint32_t close_directory()
 *  Functionality: Should do nothing
 *  Arguments: int32_t file_directory - directory we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *  Return: Return -1
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t write_directory(int32_t file_directory, const void* buffer, int32_t size)
{
    return -1;
}

/* uint32_t read_directory()
 *  Functionality: read files filename by filename, including ".", uses read_dentry_by_index
 *  Arguments: int32_t file_directory - directory we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *             int32_t index - index in boot block
 *  Return: int length - size of filename
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t read_directory(int32_t file_directory, const void* buffer, int32_t size, int32_t index)
{
    /* We want to call read_dentry_by_index with index in boot_block_t */
    /* The index in boot entries block is dir_entires (read directory entry
       at a particular position within the boot block)*/
    directory_entry_t temp; // temp var to hold directory_entry indexes
    read_dentry_by_index(index, &temp); // call funct with passed index
    int length = strlen((char*)temp.filename); // get length of filename
    if(length > 32) // 32 bytes should be max for filename
    {
        length = 32; // if not max size for filename, only read what fits
    }
    memcpy(buffer, temp.filename, length); // use memcpy to copy filename to buffer
    return length; // return length of filename
}

int32_t read_dentry_by_name(const uint8_t* fname, directory_entry_t* dentry) {
    // if non-existent file, return -1

    int i;
    for (i = 0; i < len(fname); i++) {
        dentry.filename[i] = fname[i];
    }
    // if rtc, file type = 0
    // if directory, file type = 1
    // if regular file, file type = 2
    // inode number only for regular files

    return 0;
}


int32_t read_dentry_by_index(uint32_t index, directory_entry_t* dentry) {
    if (index > 62 || index < 0) {
        return -1;
    }

    //
    // if rtc, file type = 0
    // if directory, file type = 1
    // if regular file, file type = 2
    // inode number only for regular files
    
    return 0;
}


int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    // if invalid inode num, return -1
    if (inode < 0 || inode > boot_block.inodes - 1) {
        return -1;
    }

    buf = -1;

    int i;
    for (i = offset; i < offset + length; i++) {
        // read from file with above inode
        // place number of bytes read in buf
    }

    return buf;
}
