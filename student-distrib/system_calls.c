#include "system_calls.h"

static int num_pcb = 0;
extern void flush_tlb();
extern void parent_return(uint32_t ebp, uint32_t esp, uint8_t status);
extern void context_switch(uint32_t d, uint32_t c, uint32_t b, uint32_t a);

int32_t halt(uint8_t status){
    int i;

    //Closing FDs
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*(num_pcb)));
    for (i = 0; i < 8; i++) {
        pcb->fd_table[i].flags = 0;
    }

    //Restoring parent data
    num_pcb--;
    uint32_t ebp = pcb->ebp;
    uint32_t esp = pcb->esp;
    tss.ss0 = KERNEL_DS;
    tss.esp0 = (MB_8-(KB_8*(pcb->parent_pid))) - 4;
    pcb_t* parent_pcb = (pcb_t*)(MB_8 - (KB_8*(pcb->parent_pid + 1)));

    //Restoring parent paging
    page_directory_single[USERMEM_INDEX].address_bits = (MB_8 + MB_4*(parent_pcb->pid)) >> 12;
    flush_tlb();

    parent_return(ebp, esp, status);
    return -1; //fails if it reaches here
}


int32_t execute(const uint8_t* command) {
    // return -1 if program does not exist or filename is not an executable
    // return 256 if program dies by an exception
    // if call halt, then return what halt returns
    directory_entry_t dentry;
    uint8_t fname[MAX_FILENAME_LENGTH];
    uint8_t arg1[MAX_FILENAME_LENGTH];
    uint8_t arg2[MAX_FILENAME_LENGTH];
    uint8_t arg3[MAX_FILENAME_LENGTH];
    split(command, fname, arg1, arg2, arg3); // Parse the arguments passed in
    uint8_t copyhelp_buffer[4];
    uint32_t i; //loop variable

    //Checking file validity
    if (read_dentry_by_name(fname, &dentry) == -1) {
        return -1;
    } else if(read_data(dentry.inode_number, 0, copyhelp_buffer, 4) == -1) { // Check if the filename from the argument is valid and also an executable file
        return -1;
    } else if(!(copyhelp_buffer[0] == MAGICNUM1 && copyhelp_buffer[1] == MAGICNUM2 && copyhelp_buffer[2] == MAGICNUM3 && copyhelp_buffer[3] == MAGICNUM4)) {
        return -1;
    } else if(num_pcb == MAX_PCBs) { // Check if we reached the maximum supported PCB’s (Max programs).
        return -1;
    }
    num_pcb++;
    //Setting up paging
    uint32_t pa = MB_8 + ((num_pcb-1) * MB_4);
    page_directory_single[USERMEM_INDEX].address_bits = pa >> 12;
    flush_tlb();
    
    //Loading file into memory
    if(read_data(dentry.inode_number, 0, (uint8_t*)(USER_MEMORY+USERMEM_OFFSET), ((inode_t*)inode_block + dentry.inode_number)->length) == -1) {
        return -1;
    } 

    //Creating pcb
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    pcb->pid = num_pcb-1;
    if(num_pcb == 1) {
        pcb->parent_pid = num_pcb-1;
    } else {
        pcb->parent_pid = num_pcb-2;
    }
    pcb->args[0] = arg1;
    pcb->args[1] = arg2;
    pcb->args[2] = arg3;
    if (read_data(dentry.inode_number, 24, copyhelp_buffer, 4) == -1) {
        return -1;
    }

    //Opening FDs
    /*stdin*/
    pcb->fd_table[0].file_operations.read = terminal_read;
    pcb->fd_table[0].file_operations.write = terminal_write;
    pcb->fd_table[0].file_operations.open = terminal_open;
    pcb->fd_table[0].file_operations.close = terminal_close;
    pcb->fd_table[0].flags = 1; 

    /*stdout*/
    pcb->fd_table[1].file_operations.read = terminal_read;
    pcb->fd_table[1].file_operations.write = terminal_write;
    pcb->fd_table[1].file_operations.open = terminal_open;
    pcb->fd_table[1].file_operations.close = terminal_close;
    pcb->fd_table[1].flags = 1; 

    for(i = 2; i < 8; i++) {
        pcb->fd_table[i].file_operations.read = empty_read;
        pcb->fd_table[i].file_operations.write = empty_write;
        pcb->fd_table[i].file_operations.open = empty_open;
        pcb->fd_table[i].file_operations.close = empty_close;
    }

    //context switch
    uint32_t esp;
    uint32_t ebp;
    __asm__("movl %%esp, %0" : "=r"(esp));
    __asm__("movl %%ebp, %0" : "=r"(ebp));
    pcb->esp = esp;
    pcb->ebp = ebp;
    uint32_t user_eip = *((uint32_t*)copyhelp_buffer);
    uint32_t user_esp = USER_MEMORY + MB_4 - 4;
    tss.ss0 = KERNEL_DS;
    tss.esp0 = MB_8 - (KB_8*(num_pcb-1))-4;
    context_switch(user_esp, USER_CS, USER_DS, user_eip);
    
    return 0;
}

void split(const uint8_t* command, uint8_t* fname, uint8_t* arg1, uint8_t* arg2, uint8_t* arg3) {
    int i = 0, j = 0;

    // Extract filename
    while (command[i] != ' ' && command[i] != '\0') {
        fname[j++] = command[i++];
    }
    fname[j] = '\0'; // Null-terminate filename

    // Skip space between fname and arg1
    while (command[i] == ' ' && command[i] != '\0') {
        i++;
    }

    // Extract arg1
    j = 0;
    while (command[i] != ' ' && command[i] != '\0') {
        arg1[j++] = command[i++];
    }
    arg1[j] = '\0'; // Null-terminate arg1

    // Skip space between arg1 and arg2
    while (command[i] == ' ' && command[i] != '\0') {
        i++;
    }

    // Extract arg2
    j = 0;
    while (command[i] != ' ' && command[i] != '\0') {
        arg2[j++] = command[i++];
    }
    arg2[j] = '\0'; // Null-terminate arg2

    // Skip space between arg2 and arg3
    while (command[i] == ' ' && command[i] != '\0') {
        i++;
    }

    // Extract arg3
    j = 0;
    while (command[i] != ' ' && command[i] != '\0') {
        arg3[j++] = command[i++];
    }
    arg3[j] = '\0'; // Null-terminate arg3
}


int32_t close(int32_t fd){
    if(fd < 2 || fd > NUM_OPEN_FILES){
        return -1;
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    pcb->fd_table[fd].flags = 0;  
    pcb->fd_table[fd].file_operations.close = empty_close;
    pcb->fd_table[fd].file_operations.open = empty_open;
    pcb->fd_table[fd].file_operations.read = empty_read;
    pcb->fd_table[fd].file_operations.write = empty_write;
    pcb->fd_table[fd].file_pos = 0;
    pcb->fd_table[fd].inode = 0;
    return pcb->fd_table[fd].file_operations.close(fd);
}

int32_t read(int32_t fd, void* buf, int32_t nbytes){
    
    if (buf == NULL || fd < 0 || fd == 1 || fd > NUM_OPEN_FILES){
       return -1;
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    if(pcb->fd_table[fd].flags == 0) {
        return -1;
    }
    return pcb->fd_table[fd].file_operations.read(fd,buf,nbytes);
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes){
    if (buf == NULL || fd <= 0 || fd > NUM_OPEN_FILES){
       return -1;
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    if(pcb->fd_table[fd].flags == 0) {
        return -1;
    }
    return pcb->fd_table[fd].file_operations.write(fd, buf, nbytes);
}

int32_t open(const uint8_t* fname){
    int i;
    directory_entry_t dentry;
    if(read_dentry_by_name(fname, &dentry) == -1){
        return -1;  
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    for(i = 2; i < NUM_OPEN_FILES; i++) {
        if(pcb->fd_table[i].flags == 0) {
            pcb->fd_table[i].file_pos = 0;
            pcb->fd_table[i].inode = dentry.inode_number;
            pcb->fd_table[i].flags = 1;
            if(dentry.file_type == 0){             
                pcb->fd_table[i].file_operations.read = RTC_read;
                pcb->fd_table[i].file_operations.write = RTC_write;
                pcb->fd_table[i].file_operations.open = RTC_open;
                pcb->fd_table[i].file_operations.close = RTC_close;
            }else if (dentry.file_type == 1){         
                pcb->fd_table[i].file_operations.read = read_directory;
                pcb->fd_table[i].file_operations.write = write_directory;
                pcb->fd_table[i].file_operations.open = open_directory;
                pcb->fd_table[i].file_operations.close = close_directory;
            }else if (dentry.file_type == 2){         
                pcb->fd_table[i].file_operations.read = read_file;
                pcb->fd_table[i].file_operations.write = write_file;
                pcb->fd_table[i].file_operations.open = open_file;
                pcb->fd_table[i].file_operations.close = close_file;
            }
            return i;
        }
    }
  return -1;
}

int32_t get_pcb_count() {
    return num_pcb;
}
