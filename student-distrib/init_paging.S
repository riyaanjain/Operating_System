.globl init_paging

# void init_paging(page_directory);
#
# Interface: Register-based arguments (not C-style)
#    Inputs: Physical address of page directory
#   Outputs: None
# Registers: Clobbers EAX

# Function sets up page directory, enables support for large pages
# enables paging mode, and flushes Translation Lookaside Buffer

init_paging:

    pushl %ebp
    movl %esp, %ebp 

  
    movl 8(%ebp), %eax
   
    movl %eax, %cr3 

 
    movl %cr4, %eax
  
    orl $0x00000010, %eax

    movl %eax, %cr4

 
    movl %cr0, %eax

    orl $0x80000001, %eax

    movl %eax, %cr0

    movl %cr3, %eax
    movl %eax, %cr3


    leave
    ret
