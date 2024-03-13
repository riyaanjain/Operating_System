#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "i8259.h"
#include "lib.h"

/*Keyboard IRQ index*/
#define keyboard_irq                1
#define keyboard_data_port          0x60
#define keyboard_read_write_port    0x64

/* Initialize the keyboard */
void init_keyboard();

/* Handles keyboard interrupt */
void keyboard_handler();

#endif /* _KEYBOARD_H */
