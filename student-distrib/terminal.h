#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "keyboard.h"

char terminal_buffer[BUFFER_LENGTH];

int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open (const uint8_t* filename);
int32_t terminal_close (int32_t fd);

#endif /* _TERMINAL_H */
