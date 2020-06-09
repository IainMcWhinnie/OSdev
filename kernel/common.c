//#################################################################################################
//#-----------------------------------------common.c----------------------------------------------#
//#################################################################################################
//#  Contains functions for I/O operations with peripherals                                       #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################


// This source's header file
#include "common.h"



// Output a byte of data to the port given
void outb(u16int port, u8int value){
    __asm__ ("out %%al, %%dx" : :"a" ( value ), "d" ( port ));
}

// Output a word of data to the port given
void outw(u16int port, u16int value){
    __asm__ ("out %%ax, %%dx" : :"a" ( value ), "d" ( port ));
}

// Input a byte of data from the port given
u8int inb(u16int port){
    unsigned char result;
    __asm__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

// Input a word of data from the port given
u16int inw(u16int port){
    unsigned char result;
    __asm__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}


void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}