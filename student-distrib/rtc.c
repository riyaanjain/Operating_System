#include "rtc.h"

#define RTC_INDEX       0x70
#define CMOS            0x71
#define STATUS_REG_A    0X0A
#define STATUS_REG_B    0X0B
#define STATUS_REG_C    0X0C

/* void init_RTC()
 *  Functionality: Sets up RTC interrupts and configs
 *  Arguments: None
 *  Return: None
 *  References: OSDev
 ***********************************************************************************
 *  
 */
void init_RTC() {
    /*Turning on IRQ8*/
    int rate;
    char prev;
    outb(STATUS_REG_B, RTC_INDEX);		        // select register B
    prev = inb(CMOS);	            // read the current value of register B
    outb(STATUS_REG_B, RTC_INDEX);		        // set the index again (a read will reset the index to register D)
    outb(prev | 0x40, CMOS);	    // write the previous value ORed with 0x40. This turns on bit 6 of register B

    rate = 15;
    change_RTC_rate(rate);
    
    enable_irq(RTC_irq);
}

/* void RTC_handler()
 *  Functionality: Interrupt handler for RTC interrupts
 *  Arguments: None
 *  Return: None
 *  References: OSDev
 ***********************************************************************************
 *  
 */
void RTC_handler() {
    /*Uncomment this to test RTC*/
    test_interrupts();
    
    outb(STATUS_REG_C, RTC_INDEX);	// select register C
    inb(CMOS);		    // just throw away contents
    send_eoi(RTC_irq);
}

void change_RTC_rate(int rate) {
    char prev;
    rate &= 0x0F;			// rate must be above 2 and not over 15
    outb(STATUS_REG_A, RTC_INDEX);		// set index to register A, disable NMI
    prev = inb(CMOS);	// get initial value of register A
    outb(STATUS_REG_A, RTC_INDEX);		// reset index to A
    outb((prev & 0xF0) | rate, CMOS); //write only our rate to A. Note, rate is the bottom 4 bits. 0xF0 is to clear the upper bits of prev
}
