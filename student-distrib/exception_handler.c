#include "exception_handler.h"

void exc_handler(uint32_t id, struct registers regs, uint32_t error_code) {
    // clear the screen
    clear();

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\tException Synopsis:\n\n");

    if (id < NUM_EXCEPTIONS) {
        printf("\t\t\tException Raised: %s\n", os_exceptions[id]);
        printf("\t\t\tEntry Number: %d\n\n", id);
    }


    printf("\t\t\tTechnical Error Code Data:\n\n");
    printf("\t\t\tError Code: %d\n\n", error_code);

    printf("\t\t\tRegister Information:\n\n");
    printf("\t\t\tEAX: %d\n", regs.eax);
    printf("\t\t\tEBX: %d\n", regs.ebx);
    printf("\t\t\tECX: %d\n", regs.ecx);
    printf("\t\t\tEDX: %d\n", regs.edx);
    printf("\t\t\tESI: %d\n", regs.esi);
    printf("\t\t\tEDI: %d\n", regs.edi);
    printf("\t\t\tESP: %d\n", regs.esp);
    printf("\t\t\tEBP: %d\n", regs.ebp);

    while(1);
}
