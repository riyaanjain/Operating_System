#include "terminal.h"
volatile uint32_t enter_pressed = 0;
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char* tmp_buffer = (char*)buf;
    while(enter_pressed != 1) {

    }
    enter_pressed = 0;
    while(i < nbytes && i < BUFFER_LENGTH && keyboard_buffer[i-1] != '\n' && keyboard_buffer[i] != 0x0) {
        tmp_buffer[i] = keyboard_buffer[i];
        i++;
    }
    return i;
}

int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    if (buf == NULL) {
        return -1;
    }
    int i = 0;
    char* tmp_buffer = (char*)buf;
    while(i < nbytes && tmp_buffer[i] != 0x0) {
        putc(tmp_buffer[i]);
        i++;
    }
    return i;
}

int32_t terminal_open (const uint8_t* filename){
    return 0;
}

int32_t terminal_close (int32_t fd){
    return 0;
}
 
 void set_enter() {
    enter_pressed = 1;
 }
