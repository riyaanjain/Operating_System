#include "terminal.h"
volatile uint32_t enter_pressed = 0;

/* terminal_read (int32_t fd, void* buf, int32_t nbytes)
 *  Functionality: Reads characters from the keyboard buffer into a user buffer until ENTER is pressed or the buffer is full.
 *  Arguments:  fd - File descriptor (not used here).
 *              buf - Pointer to the buffer where read characters will be stored.
 *              nbytes - Maximum number of bytes to read.
 *  Return: The number of bytes read into the buffer.
 *  References: None
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER: This function relies on a global variable `enter_pressed` to detect when the enter key is pressed.*/
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char* tmp_buffer = (char*)buf;
    while(enter_pressed != 1) {

    }
    enter_pressed = 0;
    while(i < nbytes && i < BUFFER_LENGTH && keyboard_buffer[i-1] != '\n' && keyboard_buffer[i] != 0x0) {
        tmp_buffer[i] = keyboard_buffer[i];
        i++;
    }
    return i;
}

/* terminal_write (int32_t fd, const void* buf, int32_t nbytes)
 *  Functionality: Writes characters from a user buffer to the terminal until a null terminator is encountered or the specified number of bytes have been written.
 *  Arguments:  fd - File descriptor (not used here).
 *              buf - Pointer to the buffer containing characters to write.
 *              nbytes - Maximum number of bytes to write.
 *  Return: The number of bytes written.
 *  References: None
 ***********************************************************************************
 */
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    if (buf == NULL) {
        return -1;
    }
    int i = 0;
    char* tmp_buffer = (char*) buf;
    while (i < nbytes) {
        if (tmp_buffer[i] != 0x0) {
            putc(tmp_buffer[i]);
        }
        i++;
    }
    return i;
}

/* int32_t terminal_open (const uint8_t* filename)
 *  Functionality: Opens the terminal.
 *  Arguments: filename - Name of the file (not used here).
 *  Return: Always returns 0 
 *  References: None
 ***********************************************************************************
 */
int32_t terminal_open (const uint8_t* filename){
    return 0;
}

/* terminal_close (int32_t fd)
 *  Functionality: Closes the terminal.
 *  Arguments: fd - File descriptor of the terminal (not used here).
 *  Return: Always returns 0.
 *  References: None
 ***********************************************************************************
 */
int32_t terminal_close (int32_t fd){
    return 0;
}
 
 /* set_enter()
 *  Functionality: Sets the `enter_pressed` flag to 1 to indicate that the ENTER key has been pressed.
 *  Arguments: None
 *  Return: None
 *  References: None
 ***********************************************************************************
 */
 void set_enter() {
    enter_pressed = 1;
}
