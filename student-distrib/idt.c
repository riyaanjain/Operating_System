#include "idt.h"

void init_idt() {
    // offset (16 bits) - offset_31_16
    // p (1 bit) - present
    // dpl (2 bits) - dpl
    // 0 (1 bit) - reserved0
    // gatetype (4 bits) - {size, reserved1, reserved2, reserved3}
    // reserved (8 bits) - reserved4
    // segselector (16 bits) - seg_selector
    // offset (16 bits) - offset_15_00

    // dpl is 0 for interrupts and 3 for sys calls
    // present set to 1 for valid descriptors (except 15)
    // gate type:
        // 0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
        // 0b0110 or 0x6: 16-bit Interrupt Gate
        // 0b0111 or 0x7: 16-bit Trap Gate
        // 0b1110 or 0xE: 32-bit Interrupt Gate
        // 0b1111 or 0xF: 32-bit Trap Gate
    // segselector: {index, ti, rpl}
        // Index: Bits 3-15 of the Index of the GDT or LDT entry referenced by the selector. Since Segment Descriptors are 8 bytes in length, the value of Index is never unaligned and contains all zeros in the lowest 3 bits.
        // TI: Specifies which descriptor table to use. If clear (0) then the GDT is used, if set (1) then the current LDT is used.
        // RPL: same as dpl for selector

    int i;
    for (i = 0; i < NUM_VEC; i++) {
        idt[i].reserved0 = 0;
        if (i < 20 && i != 15) {
            idt[i].present = 1;
        } else {
            idt[i].present = 0;
        }
        idt[i].size = 32;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;
        idt[i].dpl = 0;
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

}
