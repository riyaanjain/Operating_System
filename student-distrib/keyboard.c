#include "keyboard.h"

char ascii[58] = { 0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x0, ' ',      'q', 'w',
                   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
                   'l', ';', '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, 0x0, 0x0, ' '};

void init_keyboard() {
    enable_irq(keyboard_irq);
}

void keyboard_handler() {
    cli();
    uint8_t c = inb(keyboard_data_port);
    if(c >= 58) {
        send_eoi(keyboard_irq);
        sti();
        return;
    }
    putc(ascii[c]);
    send_eoi(keyboard_irq);
    sti();
}
