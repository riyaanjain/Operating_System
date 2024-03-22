#include "keyboard.h"

char ascii_1[SCANCODES] = { 0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, ' ', 'q', 'w',
                   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0x0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
                   'l', ';', '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, 0x0, 0x0, ' '};
char ascii_2[SCANCODES] = { 0x0, 0x0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE, ' ', 'Q', 'W',
                   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, 0x0,'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
                   'L', ':', '"', '~', 0x0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x0, 0x0, 0x0, ' '};

char caps_lock = 0;
char lshift = 0;
char rshift = 0;
char ctrl = 0;
char alt = 0;
int kbd_buffer = 0;

/* void init_keyboard()
 *  Functionality: Initializes the keyboard interrupt
 *  Arguments: None
 *  Return: None
 ***********************************************************************************
 *  
 */
void init_keyboard() {
    enable_irq(keyboard_irq);
}

/* void keyboard_handler()
 *  Functionality: Interrupt handler for keyboard events
 *  Arguments: None
 *  Return: None
 ***********************************************************************************
 *  
 */
void keyboard_handler() {
    cli();
    uint8_t c = inb(keyboard_data_port);
    if(ascii_1[c] == ENTER) {
        keyboard_buffer[kbd_buffer] = '\n';
        kbd_buffer = 0;
    }
    if(c == 0x0 || c == LRALT_PRESS || c == LRALT_RELEASE) {
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == CAPS_LOCK_PRESS) {
        if(caps_lock == 0) {
            caps_lock = 1;
        } else {
            caps_lock = 0;
        }
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == LSHIFT_PRESS) {
        lshift += 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == LSHIFT_RELEASE) {
        lshift -= 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == RSHIFT_PRESS) {
        rshift += 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == RSHIFT_RELEASE) {
        rshift -= 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == LRCTRL_PRESS) {
        ctrl += 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c == LRCTRL_RELEASE) {
        ctrl -= 1;
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(lshift || rshift) {
        if(c < SCANCODES) {
            putc(ascii_2[c]);
            keyboard_buffer[kbd_buffer] = ascii_2[c];
            if(kbd_buffer == BUFFER_LENGTH-1) {
                kbd_buffer = 0;
            } else {
                kbd_buffer++;
            }
            send_eoi(keyboard_irq);
            sti();
            return;
        }
    }
    if(ctrl == 1 && c == L_SCANCODE) {
        clear();
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    if(c < SCANCODES) {
        if(caps_lock == 1 && ascii_1[c] <= char_z && ascii_1[c] >= char_a) {
            putc(ascii_2[c]);
            keyboard_buffer[kbd_buffer] = ascii_2[c];
            if(kbd_buffer == BUFFER_LENGTH-1) {
                kbd_buffer = 0;
            } else {
                kbd_buffer++;
            }
            send_eoi(keyboard_irq);
            sti();
            return;
        } else if (caps_lock == 0 && ascii_1[c] <= char_z && ascii_1[c] >= char_a) {
            putc(ascii_1[c]);
            keyboard_buffer[kbd_buffer] = ascii_1[c];
            if(kbd_buffer == BUFFER_LENGTH-1) {
                kbd_buffer = 0;
            } else {
                kbd_buffer++;
            }
            send_eoi(keyboard_irq);
            sti();
            return;
        } else {
            putc(ascii_1[c]);
            keyboard_buffer[kbd_buffer] = ascii_1[c];
            if(kbd_buffer == BUFFER_LENGTH-1) {
                kbd_buffer = 0;
            } else {
                kbd_buffer++;
            }
            send_eoi(keyboard_irq);
            sti();
            return;
        }
    }
    send_eoi(keyboard_irq);
    sti();
    return;
}

