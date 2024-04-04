#include "system_calls.h"

static int num_pcb = 0;
extern void flush_tlb();
extern void parent_return(uint32_t execute_ebp, uint32_t execute_esp, uint8_t status);
extern void context_switch(uint32_t d, uint32_t c, uint32_t b, uint32_t a);

int32_t halt(uint8_t status){
    int i;
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*(num_pcb)));
    if(num_pcb != 0) {
        num_pcb--;
    }
    pcb_t* parent_pcb = (pcb_t*)(MB_8-(KB_8*num_pcb));
    uint32_t pa = MB_8 + ((parent_pcb->pid-1) * MB_4);
    page_directory_single[USERMEM_INDEX].address_bits = pa >> 12;
    flush_tlb();

    //closing FDs
    for(i = 0; i < NUM_OPEN_FILES; i++){
        pcb->fd_table[i].flags = 0;
    }

    tss.ss0 = KERNEL_DS;
    tss.esp0 = tss.esp0 = MB_8 - (KB_8*(parent_pcb->pid-1));
    parent_return(parent_pcb->ebp,parent_pcb->esp,status);
    return -1;
}


int32_t execute(const uint8_t* command) {
    // return -1 if program does not exist or filename is not an executable
    // return 256 if program dies by an exception
    // if call halt, then return what halt returns
    directory_entry_t dentry;
    uint8_t fname[MAX_FILENAME_LENGTH];
    uint8_t args[MAX_NUM_ARGS][MAX_ARGS_LEN] = {{0}};
    split(command, fname, args); // Parse the arguments passed in
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
    pcb->args[0] = args[0][0] + (args[0][1] << 8) + (args[0][2] << 16) + (args[0][3] << 24);
    pcb->args[1] = args[1][0] + (args[1][1] << 8) + (args[1][2] << 16) + (args[1][3] << 24);
    pcb->args[2] = args[2][0] + (args[2][1] << 8) + (args[2][2] << 16) + (args[2][3] << 24);
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
    tss.esp0 = MB_8 - (KB_8*(num_pcb-1));

    context_switch(user_esp, USER_CS, USER_DS, user_eip);
    
    return 0;
}

void split(const uint8_t* command, uint8_t* fname, uint8_t args[MAX_NUM_ARGS][MAX_ARGS_LEN]) {
    int i, j, k;
    i = j = 0;

    // Extract filename
    while (command[i] != ' ' && command[i] != '\0' && j < MAX_ARGS_LEN - 1) {
        fname[j++] = command[i++];
    }
    fname[j] = '\0'; // Null-terminate filename

    // Skip spaces
    while (command[i] == ' ' && command[i] != '\0') {
        i++;
    }

    // Initialize all args to empty strings
    for (j = 0; j < MAX_NUM_ARGS; j++) {
        args[j][0] = '\0';
    }

    // Extract arguments
    for (k = 0; k < MAX_NUM_ARGS && command[i] != '\0'; k++) {
        j = 0; 

        while (command[i] != ' ' && command[i] != '\0' && j < MAX_ARGS_LEN - 1) {
            args[k][j++] = command[i++];
        }
        args[k][j] = '\0'; 

        // Skip spaces
        while (command[i] == ' ' && command[i] != '\0') {
            i++;
        }
    }
}

int32_t close(int32_t fd){
    if(fd < 2 || fd > NUM_OPEN_FILES){
        return -1;
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    pcb->fd_table[fd].flags = 0;  
    return pcb->fd_table[fd].file_operations.close(fd);
}

int32_t read(int32_t fd, void* buf, int32_t nbytes){
    
    if (buf == NULL || fd < 0 || fd > NUM_OPEN_FILES){
       return -1;
    }
    pcb_t* cur_pcb_ptr = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    if(cur_pcb_ptr->fd_table[fd].flags == 0) {
        return -1;
    }
    return cur_pcb_ptr->fd_table[fd].file_operations.read(fd,buf,nbytes);
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes){
    if (buf == NULL || fd < 0 || fd > NUM_OPEN_FILES){
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
int32_t getargs(uint8_t* buf, int32_t nbytes){
    if(buf == NULL){
        return -1;
    }
    pcb_t* pcb = (pcb_t*)(MB_8 - (KB_8*num_pcb));
    strncpy((int8_t*)buf, (int8_t*)(pcb->args), nbytes);
    return 0;
}
int32_t get_pcb_count() {
    return num_pcb;
}
