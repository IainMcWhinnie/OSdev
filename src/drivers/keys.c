//#################################################################################################
//#-------------------------------------------keys.c----------------------------------------------#
//#################################################################################################
//#  Driver for the computer keyboard.                                                            #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

// include nesseccery files
#include "keys.h"
#include "./../kernel/common.h"
#include "screen.h"
#include "./../kernel/timer.h"


char buffer[256];
char letter;
int shifton = 0;
int capslock = 0;

void keyboard_interrupt_handler(registers_t regs){
    u8int data = inb(0x60);
    //printhex(data);
    if(data&0x80){ // a button has been released
        if(data == 0xaa | data == 0xb6 ){ // shift button is released
            shifton = 0;
        }
    }else{ // a button has been pressed
        //printhex(data);
        if(kbdus[data]){ // a printable letter has been recieved
            if( shifton | capslock ){ // shift or capslock is on --> make it capital
                printchar(kbdusCAPS[data]);
            }else{
                printchar(kbdus[data]);
            }
        }else{ // a function key has been pressed
            //printstring("Well we got ourself a difficult one");
            if(data == 0x2a | data == 0x36 ){ // shift was pressed
                shifton = 1;
            }
            if(data == 0x3a){ // capslock was pressed
                if(capslock){ // negate the truth about capslock
                    capslock = 0;
                }else{
                    capslock = 1;
                }
            }
        }
    }
    outb(0x20, 0x20);
}

void init_keyboard(){
    register_interrupt_handler(IRQ1, &keyboard_interrupt_handler);
}