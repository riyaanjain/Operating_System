#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "i8259.h"
#include "lib.h"

/*RTC IRQ index*/
#define RTC_irq    8

/* Initialize the RTC */
void init_RTC();

/* Handles RTC interrupt */
void RTC_handler();

#endif /* _RTC_H */
