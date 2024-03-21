#include "exception_handler.h"

void exc_handler(uint32_t id, struct registers regs, uint32_t error_code) {
    // clear the screen
    clear();
    printf("\n");
    printf("Exception Synopsis:\n");
    if (id != SYSTEM_CALLS) {
        if(id < NUM_EXCEPTIONS) {
            printf("Exception raised %s", os_exceptions[id]);
        }
    } else {
        printf("System Call Exception");
    }
    while(1);
}
