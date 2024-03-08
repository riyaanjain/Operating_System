#include "idt.h"

void init_idt() {
    int i;

    for (i = 0; i < NUM_VEC; i++) {
        // set present bit
        if ((i < 21 && i != 15) || i == SYSTEM_CALLS || i == KEYBOARD_CALLS || i == RTC_CALLS) {
            idt[i].present = 1;
        } else {
            idt[i].present = 0;
        }

        // set dpl to 0 for exceptions and 3 for system calls
        if (i == SYSTEM_CALLS) {
            idt[i].dpl = 3;
        } else {
            idt[i].dpl = 0;
        }

        // set reserved0 to 0
        idt[i].reserved0 = 0;

        // set gate type (reseerved3 is 1 for keyboard and rtc)
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        if (i == KEYBOARD_CALLS || i == RTC_CALLS) {
            idt[i].reserved3 = 1;
        } else {
            idt[i].reserved3 = 0;
        }

        // set reserved4 to 0
        idt[i].reserved4 = 0;
        
        // set seg selector to kernel code segment
        idt[i].seg_selector = KERNEL_CS;
    }

    
    SET_IDT_ENTRY(idt[0], ); // Division Error
    SET_IDT_ENTRY(idt[1], ); // Debug
    SET_IDT_ENTRY(idt[2], ); // Non-maskable Interrupt
    SET_IDT_ENTRY(idt[3], ); // Breakpoint
    SET_IDT_ENTRY(idt[4], ); // Overflow
    SET_IDT_ENTRY(idt[5], ); // Bound Range Exceeded
    SET_IDT_ENTRY(idt[6], ); // Invalid Opcode
    SET_IDT_ENTRY(idt[7], ); // Device Not Available
    SET_IDT_ENTRY(idt[8], ); // Double Fault
    SET_IDT_ENTRY(idt[9], ); // Coprocessor Segment Overrun
    SET_IDT_ENTRY(idt[10], ); // Invalid TSS
    SET_IDT_ENTRY(idt[11], ); // Segment Not Present
    SET_IDT_ENTRY(idt[12], ); // Stack-Segment Fault
    SET_IDT_ENTRY(idt[13], ); // General Protection Fault
    SET_IDT_ENTRY(idt[14], ); // Page Fault
    SET_IDT_ENTRY(idt[16], ); // x87 Floating-Point Exception
    SET_IDT_ENTRY(idt[17], ); // Alignment Check
    SET_IDT_ENTRY(idt[18], ); // Machine Check
    SET_IDT_ENTRY(idt[19], ); // SIMD Floating-Point Exception
    SET_IDT_ENTRY(idt[20], ); // Virtualization Exception
    SET_IDT_ENTRY(idt[SYSTEM_CALLS], ); // System Calls
    SET_IDT_ENTRY(idt[KEYBOARD_CALLS], ) // Keyboard Calls
    SET_IDT_ENTRY(idt[RTC_CALLS], ); // RTC Calls

    lidt(idt_desc_ptr);
}
