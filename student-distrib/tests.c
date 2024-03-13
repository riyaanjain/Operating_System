#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "keyboard.h"
#include "rtc.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// division error test
int div_zero_error_test() {
	TEST_HEADER;

	int a = 0;
	int b;
	b = 1/a;
	return FAIL;
}

// system call handler test
int system_call_handler_test() {
	TEST_HEADER;

	__asm__("int $0x80");
	return FAIL;
}

// invalid opcode test
int invalid_opcode_test() {
	TEST_HEADER;
	
	__asm__("ud2");
	return FAIL;
}

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	// TEST_OUTPUT("zero div test", div_zero_error_test());
	// TEST_OUTPUT("system call handler test", system_call_handler_test());
<<<<<<< HEAD
	TEST_OUTPUT("invalid opcode test", invalid_opcode_test());
=======
	// TEST_OUTPUT("invalid opcode test", invalid_opcode_test());
>>>>>>> c9e13e803f3fa8c639eaf6b9f81ffc117a79e2fa
}
