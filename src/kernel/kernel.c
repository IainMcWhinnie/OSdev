//#################################################################################################
//#-----------------------------------------kernel.c----------------------------------------------#
//#################################################################################################
//#  The OS kernel                                                                                #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################


#include "kernel.h" // header file for this source

#include "./../drivers/screen.h" // contains functions for writing to text mode screen
#include "common.h"              // contains type definitions and io functions
#include "descriptor_tables.h"   // used to initialise and modify the IDT and possible GDT (not yet implemented)
//#include "isr.h"                 // needed for interrupt controlling
#include "timer.h"               // needed for system clock
#include "./../drivers/keys.h"   // needed for keyboard

// "welcome" message
char* msg =  "Welcome to Iain OS,\nunless you are Iain then you've gone badly wrong. Try typing on the keyboard. Strange, huh?\n";


// Function Start()
//    --> Start of kernel execution
void start() {
    init_idt(); // initiate the interrupt table
    init_timer(256); // start a time for the system clock
    init_keyboard(); // initialise random typing

    // clear the screen and print out a welcome message
    clearscreen();
    printstring(msg);

    char *mem = (char*) 0x0;
    for (int i=0x0; i<0x1000000; i++){
        printhex(i<<4);
        for (int o = 0; o<0x10; o++){
            mem = (char*) (i<<4)+o;
            printchar(' ');
            printhexbyte(*mem);
        }
        printchar('\n');
    }
}

