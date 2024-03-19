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

/* Before video memory address test
 * 
 * Checks that dereferencing adress before the video memory starts doesn't work
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: Kernel freeze
 * Coverage: 0x00000 - 0xB7FFF
 */
int before_vid_test(){
	TEST_HEADER;

	unsigned int BEFORE_VID_ADDR = 0xB7FFF;
	int test;
	int *ptr = (int*)BEFORE_VID_ADDR;
	test = *ptr;

	return FAIL;	/*If kernel didn't freeze, FAIL returned*/
}

/* Starting video memory address test
 * 
 * Checks that dereferencing adress before at video memory starts works
 * Inputs: None
 * Outputs: PASS
 * Side Effects: None
 * Coverage: 0xB8000 - 0xB8FFC
 */
int start_of_vid_test(){
	TEST_HEADER;

	unsigned int VIDEO_MEM_ADDR = 0xB8000;
	int test;
	int *ptr = (int*)VIDEO_MEM_ADDR;
	test = *ptr;

	return PASS;
}

/* End of video memory address test
 * 
 * Checks that dereferencing adress at end of video memory starts works
 * Inputs: None
 * Outputs: PASS
 * Side Effects: None
 * Coverage: 0xB8FFC
 */
int end_of_vid(){
	TEST_HEADER;

	unsigned int END_OF_VID_ADDR = 0xB8FFC;
	int test;
	int *ptr = (int*)END_OF_VID_ADDR;
	test = *ptr;

	return PASS;
}

/* After video memory address test
 * 
 * Checks that dereferencing adress after video memory starts doesnt work
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: Kernel freeze
 * Coverage: 0xB8FFD - 0x3FFFFF
 */
int after_vid_test()
{
	TEST_HEADER;

	unsigned int AFTER_VID_ADDR = 0xB8FFD;
	int test;
	int *ptr = (int *)AFTER_VID_ADDR;
	test = *ptr;

	return FAIL;	/*If kernel didn't freeze, FAIL returned*/
}

/* Before kernel address dereferencing test
 * 
 * Checks that dereferencing adress before kernel memory starts doesnt work
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: Kernel freeze
 * Coverage: 0x3FFFFF
 */
int before_kernel_page_test(){
	TEST_HEADER;

	unsigned int BEFORE_KERNEL_ADDR = 0x3FFFFF;

	int test;
	int *ptr = (int *)BEFORE_KERNEL_ADDR;
	test = *ptr;
	return FAIL;	/*If kernel didn't freeze, FAIL returned*/
}

/* Start of kernel address dereferencing test
 * 
 * Checks that dereferencing adress before kernel memory starts works
 * Inputs: None
 * Outputs: PASS
 * Side Effects: None
 * Coverage: 0x400000
 */
int start_of_kernel_test(){
	TEST_HEADER;

	unsigned int KERNEL_ADDR = 0x400000;
	int test;
	int *ptr = (int *)KERNEL_ADDR;
	test = *ptr;

	return PASS;
}

/* Bottom of kernel address dereferencing test
 * 
 * Checks that dereferencing adress at bottom of kernel memory starts works
 * Inputs: None
 * Outputs: PASS
 * Side Effects: None
 * Coverage: 0x7FFFFC
 */
int bottom_kernel_page_test(){
	TEST_HEADER;

	unsigned int BOTTOM_OF_KERNEL = 0x7FFFFC;

	int test;
	int *ptr = (int *)BOTTOM_OF_KERNEL;
	test = *ptr;
	return PASS;
}

/* After kernel address dereferencing test
 * 
 * Checks that dereferencing adress after kernel memory starts doesnt work
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: Kernel freeze
 * Coverage: 0x7FFFFD
 */
int after_kernel(){
	TEST_HEADER;

	unsigned int OUT_OF_RANGE_ADDR = 0x7FFFFD;

	int test;
	int *ptr = (int*)OUT_OF_RANGE_ADDR;
	test = *ptr;
	return FAIL;		/*If kernel didn't freeze, FAIL returned*/
}

/* Null dereferencing test
 * 
 * Checks that dereferencing null doesnt work
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: Kernel freeze
 * Coverage: 0x7FFFFD
 */
int null_paging_test(){
	TEST_HEADER;

	int test;
	int *ptr = NULL;
	test = *ptr;
	return FAIL;		/*If kernel didn't freeze, FAIL returned*/
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
	// TEST_OUTPUT("invalid opcode test", invalid_opcode_test());
	//TEST_OUTPUT("before vid", before_vid_test());
	//TEST_OUTPUT("start_of_vid_test", start_of_vid_test());
	//TEST_OUTPUT("end of vid", end_of_vid());
	// TEST_OUTPUT("after_vid_test", after_vid_test());
	//TEST_OUTPUT("before_kernel_page_test", before_kernel_page_test());
	// TEST_OUTPUT("start_of_kernel_test", start_of_kernel_test());
	//TEST_OUTPUT("bottom_kernel_page_test", bottom_kernel_page_test());
	//TEST_OUTPUT("out_of_range_paging_test", after_kernel());
	//TEST_OUTPUT("null_paging_test", null_paging_test());
}
