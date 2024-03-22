#include "terminal.h"

int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char* tmp_buffer = (char*)buf;
    while(i < nbytes-1 && i < BUFFER_LENGTH && keyboard_buffer[i] != '\n' && keyboard_buffer[i] != '\0') {
        tmp_buffer[i] = keyboard_buffer[i];
        i++;
    }
    tmp_buffer[i] = '\n';
    return i+1;
}

int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    int i = 0;
    char* tmp_buffer = (char*)buf;
    while(i < nbytes-1 && i < BUFFER_LENGTH && tmp_buffer[i] != '\n') {
        putc(tmp_buffer[i]);
        i++;
    }
    putc('\n');
    return i;
}

int32_t terminal_open (const uint8_t* filename){
    return 0;
}

int32_t terminal_close (int32_t fd){
    return 0;
}
