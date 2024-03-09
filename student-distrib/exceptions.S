#define ASM     1
#define SYSTEM_CALLS    0x80

#define EXCEPTION(function_name, vector)            \
    .globl function_name                            ;\
    function_name:                                  ;\
        pushl $0                                    ;\
        pushal                                      ;\
        pushfl                                      ;\
        pushl $vector                               ;\
        call exc_handler                            ;\
        addl $4, %esp                               ;\
        popfl                                       ;\
        popal                                       ;\
        addl $4, %esp                               ;\
        iret

#define EXCEPTION_ERROR_CODE(function_name, vector) \
    .globl function_name                            ;\
    function_name:                                  ;\
        pushal                                      ;\
        pushfl                                      ;\
        pushl $vector                               ;\
        call exc_handler                            ;\
        addl $4, %esp                               ;\
        popfl                                       ;\
        popal                                       ;\
        addl $4, %esp                               ;\
        iret

EXCEPTION(division_error, 0);
EXCEPTION(debug, 1);
EXCEPTION(non_maskable_interrupt, 2);
EXCEPTION(breakpoint, 3);
EXCEPTION(overflow, 4);
EXCEPTION(bound_range_exceeded, 5);
EXCEPTION(invalid_opcode, 6);
EXCEPTION(device_not_available, 7);
EXCEPTION_ERROR_CODE(double_fault, 8);
EXCEPTION(coprocessor_segment_overrun, 9);
EXCEPTION_ERROR_CODE(invalid_tss, 10);
EXCEPTION_ERROR_CODE(segment_not_present, 11);
EXCEPTION_ERROR_CODE(stack_segment_fault, 12);
EXCEPTION_ERROR_CODE(general_protection_fault, 13);
EXCEPTION_ERROR_CODE(page_fault, 14);
EXCEPTION(asm_floating_point_exception, 16);
EXCEPTION_ERROR_CODE(alignment_check, 17);
EXCEPTION(machine_check, 18);
EXCEPTION(simd_floating_point_exception, 19);
EXCEPTION(system_call, SYSTEM_CALLS);
