/* paging.c - Functions for various paging descriptors, descriptor tables,
 * and selectors
 * vim:ts=4 noexpandtab
 */

#include "paging.h"

/* void setup_paging()
 *  Functionality: Sets up paging, configures page directory entries
 *  Arguments: None
 *  Return: None
 *  References: OSDev
 ***********************************************************************************
 *  
 */

/* Calling the initialize paging function defined in init_paging.S */
extern void init_paging(uint32_t directory);

void setup_paging() {
    uint32_t i;

    page_directory_single[TABLE_INDEX].present         = 1;
    page_directory_single[TABLE_INDEX].read_write      = 1;
    page_directory_single[TABLE_INDEX].address_bits    = ((uint32_t)page_directory_entry_single) >> 12;

    page_directory_single[KERNEL_INDEX].present         = 1;
    page_directory_single[KERNEL_INDEX].read_write      = 1;
    page_directory_single[KERNEL_INDEX].page_size       = 1;  
    page_directory_single[KERNEL_INDEX].address_bits    = KERNEL_MEMORY >> 12;

    page_directory_single[USERMEM_INDEX].present         = 1;
    page_directory_single[USERMEM_INDEX].user_supervisor = 1;
    page_directory_single[USERMEM_INDEX].read_write      = 1;
    page_directory_single[USERMEM_INDEX].page_size       = 1; 
    page_directory_single[USERMEM_INDEX].address_bits    = USER_MEMORY >> 12;

    for(i = 0; i < 1024; i++){
        /*Initializing table entry*/
        if(i  == VIDEO_MEMORY >> 12){
            page_directory_entry_single[i].present = 1;
        }
        page_directory_entry_single[i].read_write           = 1;
        page_directory_entry_single[i].address_bits         = i;

        /*Initializing vidmem table entry*/
        page_directory_entry_vidmem[i].read_write           = 1; 
        page_directory_entry_vidmem[i].cache_disable        = 1;  
        page_directory_entry_vidmem[i].address_bits         = i;
    }

    init_paging((uint32_t)page_directory_single);
}
