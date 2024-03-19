/* paging.h - Defines for various paging descriptors, descriptor tables,
 * and selectors
 * vim:ts=4 noexpandtab
 */


#ifndef PAGING_H
#define PAGING_H

#include "types.h"


/* Calling the setup paging logic defined in setup_paging.c */
extern void setup_paging();

/* Calling the initialize paging function defined in init_paging.S */
extern void init_paging(unsigned int*);

/* Define the data structure for Page Directory Entry (4 KB) */
typedef union page_directory_entry_4KB_t {
    uint32_t val;
    struct { // filled in from OSDev
        uint32_t present                :1;
        uint32_t read_write             :1;
        uint32_t user_supervisor        :1;
        uint32_t write_through          :1;
        uint32_t cache_disable          :1;
        uint32_t accessed               :1;
        uint32_t available_1            :1;
        uint32_t page_size              :1;
        uint32_t available_2            :4;
        uint32_t address_bits           :20;
    } __attribute__ ((packed));
} page_directory_entry_4KB_t;

/* Define the data structure for Page Directory Entry (4 MB) */
typedef union page_directory_entry_4MB_t {
    uint32_t val;
    struct { // filled in from OSDev
        uint32_t present                :1;
        uint32_t read_write             :1;
        uint32_t user_supervisor        :1;
        uint32_t write_through          :1;
        uint32_t cache_disable          :1;
        uint32_t accessed               :1;
        uint32_t dirty                  :1;
        uint32_t page_size              :1;
        uint32_t global                 :1;
        uint32_t available              :3;
        uint32_t page_attribute_table   :1;
        uint32_t upper_bits             :8;
        uint32_t reserved               :1;
        uint32_t lower_bits             :10;
    } __attribute__ ((packed));
} page_directory_entry_4MB_t;

/* Combine 4KB and 4MB for full PDE */
typedef union page_directory_entry_t {
    page_directory_entry_4KB_t page_directory_entry_4KB;
    page_directory_entry_4MB_t page_directory_entry_4MB;
} page_directory_entry_t;

/* Define the data structure for Page Table Entry */
typedef union page_table_entry_t {
    uint32_t val;
    struct { // filled in from OSDev
        uint32_t present                :1;
        uint32_t read_write             :1;
        uint32_t user_supervisor        :1;
        uint32_t write_through          :1;
        uint32_t cache_disable          :1;
        uint32_t accessed               :1;
        uint32_t dirty                  :1;
        uint32_t page_attribute_table   :1;
        uint32_t global                 :1;
        uint32_t available              :3;
        uint32_t address_bits           :20;
    } __attribute__ ((packed));
} page_table_entry_t;

/* We initialize 1 page directory */
page_directory_entry_t page_directory_single[1024]  __attribute__ ((aligned (4096)));
// 4096 comes from the fact that a page directory is made up of 4096 bytes
// both tables contain 1024 4-byte entries
page_table_entry_t page_directory_entry_single[1024] __attribute__ ((aligned (4096)));
// page table entry size is also 4096 bytes
// both tables contain 1024 4-byte entries

#endif /* PAGING_H */
