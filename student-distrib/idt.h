#ifndef _idt_H
#define _idt_H

#include "x86_desc.h"
#include "exceptions.h"

#ifndef ASM

#define SYSTEM_CALLS    0x80
#define KEYBOARD_CALLS  33
#define RTC_CALLS       40

void init_idt();

#endif /* ASM */

#endif /* _idt_H */