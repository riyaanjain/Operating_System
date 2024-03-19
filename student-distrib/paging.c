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
 void setup_paging()
 {
    int i; // init var for outer loop for PDEs (1 for 4KB and 1 for 4MB)
    for(i = 0; i < 2; i++)
    {
        if(i == 0) // check if we are at the first PDE
        {
            /* Info taken from table 4-5: PDE that references a page table */

            /* The way we set this is by accessing the single PDE defined in paging.h 
                we access the 4KB part of the union and then the specific bit position */
                page_directory_single[i].page_directory_entry_4KB.present = 1; // must be 1 to reference a page table
                page_directory_single[i].page_directory_entry_4KB.read_write = 1; // writes allowed to 4MB region
                page_directory_single[i].page_directory_entry_4KB.user_supervisor = 0;
                page_directory_single[i].page_directory_entry_4KB.write_through = 0;
                page_directory_single[i].page_directory_entry_4KB.cache_disable = 0;
                page_directory_single[i].page_directory_entry_4KB.accessed = 0;
                page_directory_single[i].page_directory_entry_4KB.available_1 = 0;
                page_directory_single[i].page_directory_entry_4KB.page_size = 0; 
                page_directory_single[i].page_directory_entry_4KB.available_2 = 0;
                page_directory_single[i].page_directory_entry_4KB.address_bits = ((int) page_directory_entry_single) >> 12; // ptindex under OSDev Manipulation for MSB
        }
        if(i == 1) // check if we are at the second PDE
        {
            /* Info taken from table 4-4: PDE that maps a 4MB Page */

            /* The way we set this is by accessing the single PDE defined in paging.h 
                we access the 4MB part of the union and then the specific bit position */
                page_directory_single[i].page_directory_entry_4MB.present = 1; // must be 1 to map a 4MB page
                page_directory_single[i].page_directory_entry_4MB.read_write = 1; // writes may be allowed to 4MB page 
                page_directory_single[i].page_directory_entry_4MB.user_supervisor = 0; // check this, supervisor bit set up 
                page_directory_single[i].page_directory_entry_4MB.write_through = 0;
                page_directory_single[i].page_directory_entry_4MB.cache_disable = 0; 
                page_directory_single[i].page_directory_entry_4MB.accessed = 0; 
                page_directory_single[i].page_directory_entry_4MB.dirty = 0; 
                page_directory_single[i].page_directory_entry_4MB.page_size = 1; // must be 1 
                page_directory_single[i].page_directory_entry_4MB.global = 1; // translation is global (access code from anywhere)
                page_directory_single[i].page_directory_entry_4MB.available = 0; 
                page_directory_single[i].page_directory_entry_4MB.page_attribute_table = 0; // reserved (must be 0)
                page_directory_single[i].page_directory_entry_4MB.upper_bits = 0; // may need to be swapped w/ lower bits, check
                page_directory_single[i].page_directory_entry_4MB.reserved = 0; // reserved (must be 0)
                page_directory_single[i].page_directory_entry_4MB.lower_bits = (0x400000) >> 22; // pdindex under OSDev Manipulation for MSB
                /* 0x400000 comes from kernel loaded to physical address for 4MB */
        }
    }
    int j; // init var for another loop to set video memory page
    for(j = 0; j < 1024; j++) // loop through 1024 4-byte entries
    {
        /* Filled in from Intel ISA Documentation, manipulating 4KB since 
           video memory page is within it */
        if(j == (0xB8000 >> 12)) // start of video mem shifted by offset from 4KB
        {
            page_directory_single[j].page_directory_entry_4KB.present = 1;
        }
        else
        {
            page_directory_single[j].page_directory_entry_4KB.present = 0;
        }
        page_directory_single[j].page_directory_entry_4KB.read_write = 1; // must be enabled
        page_directory_single[j].page_directory_entry_4KB.user_supervisor = 0;
        page_directory_single[j].page_directory_entry_4KB.write_through = 0;
        page_directory_single[j].page_directory_entry_4KB.cache_disable = 1; // must be enabled
        page_directory_single[j].page_directory_entry_4KB.accessed = 0;
        page_directory_single[j].page_directory_entry_4KB.available_1 = 0;
        page_directory_single[j].page_directory_entry_4KB.page_size = 0; 
        page_directory_single[j].page_directory_entry_4KB.available_2 = 0;
        page_directory_single[j].page_directory_entry_4KB.address_bits = j; // corresponding index
    }
    init_paging((uint32_t*)page_directory_single); // call ASM function with our page directory
 }
