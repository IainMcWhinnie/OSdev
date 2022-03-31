//#################################################################################################
//#-------------------------------------------isr.c-----------------------------------------------#
//#################################################################################################
//#  Contains the handler functions for interrupts. Defines a function that allows a custom       #
//#  handler to be added as an isr.                                                               #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

#include "common.h"
#include "isr.h"
#include "./../drivers/screen.h"


isr_t interrupt_handlers[256]; // create an array of interrupt handlers

// function for adding a new interrupt handler
void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler; // add the interrupt handler
}


// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    // for now print an little message
    printstring("recieved interrupt: ");
    printhex(regs.int_no);
    printchar('\n');

    // now call our interrupt handler function
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        //printhex((u32int) &handler);
        handler(regs);
    }
}


// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no]; // get the correct handler from the list of pointers
       handler(regs); // call the handler function
   }
}