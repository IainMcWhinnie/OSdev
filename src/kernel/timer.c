//#################################################################################################
//#-------------------------------------------timer.c---------------------------------------------#
//#################################################################################################
//#  Gives the system a internal timing system using the Programmable Interval Timer (PIT) on     #
//#  IRQ0 to create a system clock. On starting the clock is initialised to 00:00:00 and it       #
//#  counts up indefinitely.                                                                      #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################


// include the neccessary files
#include "timer.h" // header file containing prototypes
#include "isr.h" // for interrupt handling
#include "./../drivers/screen.h" // to print to the screen

// variables to keep track of time
u32int tick = 0; // the PIT only goes so slowly so tick updates 256 times a second
// set up the timing variables
u32int seconds = 0; 
u32int minutes = 0;
u32int hours = 0;



// print time since power up
void printtime(){
   char digit = hours/10;     // get the higher hour digit
   printchar(digit+'0');      // print it out
   digit = hours-(digit*10);  // get the lower hour digit
   printchar(digit+'0');      // print it out
   printchar(':');            // print seperator
   digit = minutes/10;        // and etc.....
   printchar(digit+'0');
   digit = minutes-(digit*10);
   printchar(digit+'0');
   printchar(':');
   digit = seconds/10;
   printchar(digit+'0');
   digit = seconds-(digit*10);
   printchar(digit+'0');
}

// interrupt handler for the PIT at IRQ0
void timer_callback(registers_t regs){
   tick++; // update the clock tick
   if(tick%256 == 0){ // check if a second has passed
      seconds++; // update seconds
   }
   if(seconds == 60){ // check if a minute has passed
      seconds = 0; // reset seconds
      minutes++; // update minutes
   }
   if(minutes == 60){ // check if an hour has passed
      minutes = 0; // reset minutes
      hours++; // update hours
   }
}

void init_timer(u32int frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);
   //printhex((u32int) &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}