#include "exception_handler.h"

void exc_handler(uint32_t id, struct registers regs, uint32_t error_code) {
    clear();
    
    printf("\033[1;44m"); // Set background color to blue
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[1;37m"); // Set text color to white
    printf("\t\t\tException Synopsis:\n\n");
    printf("\033[1;34m"); // Set text color to blue

    if (id < NUM_EXCEPTIONS) {
        printf("\t\t\tException Raised: %s\n", os_exceptions[id]);
        printf("\t\t\tEntry Number: %d\n\n", id);
    }

    printf("\033[1;37m"); // Set text color to white

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

    printf("\033[0m"); // Reset color
}

