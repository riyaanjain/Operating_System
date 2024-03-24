#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "i8259.h"
#include "lib.h"

/*Keyboard IRQ index*/
#define keyboard_irq                1
#define keyboard_data_port          0x60

#define char_z                      122
#define char_a                      97

#define L_SCANCODE                  38

#define SCANCODES                   58
#define CAPS_LOCK_PRESS             0x3A
#define LSHIFT_PRESS                0x2A
#define LSHIFT_RELEASE              0xAA
#define RSHIFT_PRESS                0x36
#define RSHIFT_RELEASE              0xB6
#define LRCTRL_PRESS                0x1D
#define LRCTRL_RELEASE              0x9D
#define LRALT_PRESS                 0x38
#define LRALT_RELEASE               0xB8

#define ENTER                       '\n'
#define BACKSPACE                   0x08

#define BUFFER_LENGTH               128

char keyboard_buffer[BUFFER_LENGTH];

/* Initialize the keyboard */
void init_keyboard();

/* Handles keyboard interrupt */
void keyboard_handler();

#endif /* _KEYBOARD_H */
