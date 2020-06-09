//#################################################################################################
//#-----------------------------------descriptor_tables.c-----------------------------------------#
//#################################################################################################
//#  Contains the function to initialise the Interrupt Descriptor Table (IDT). This sets up the   #
//#  IDT with handler functions defined in "interrupts.asm". These are short handlers which push  #
//#  their interrupt numbers to the stack and then call a common handler function. The end of the #
//#  chain of handler functions is the handler functions defined in "isr.c" for handling both     #
//#  IRQs and ISRs.                                                                               #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

#include "descriptor_tables.h"


// init the IDT data structure
idt_gate_descriptor_t IDT[256];
// init the IDT pointer (not a pointer in the C sense) structure
idt_descriptor_t IDT_ptr;




// initiliase the IDT with the first 32 cpu interrupts and the 16 hardware interrupts
void init_idt(){

    // set the base to the address of the IDT data
    IDT_ptr.base = (u32int) &IDT;
    // set the limit to the size of each IDT entry * 256 -1 (intel specifies the value must be 1 less than a multiple of 8)
    IDT_ptr.limit = 8*256-1;

    char* baseaddr = (char*) IDT; // interpret IDT base address as string pointer so we can write over it without trouble
    for (int i=0; i< 256*8; i++){ // loop through every byte of the IDT
        baseaddr[i] = 0;          // and write a zero over it
    }

    u8int flags = IDT_GATE_FLAG_32BIT | IDT_GATE_FLAG_CONST | IDT_GATE_FLAG_PRESENT; // flags to pass to our idt entries
    u16int code_seg = 0b1000; // use the first segment described by the GDT, our code segment for 
    // interupts 0-31 are used by the CPU so we definitly need to intialise them
    idt_set_gate(0, (u32int) isr0, code_seg, flags); // add an idt entry for interrupt 0 that points to the assembly routine isr0
    idt_set_gate(1, (u32int) isr1, code_seg, flags);
    idt_set_gate(2, (u32int) isr2, code_seg, flags);
    idt_set_gate(3, (u32int) isr3, code_seg, flags);
    idt_set_gate(4, (u32int) isr4, code_seg, flags);
    idt_set_gate(5, (u32int) isr5, code_seg, flags);
    idt_set_gate(6, (u32int) isr6, code_seg, flags);
    idt_set_gate(7, (u32int) isr7, code_seg, flags);
    idt_set_gate(8, (u32int) isr8, code_seg, flags);
    idt_set_gate(9, (u32int) isr9, code_seg, flags);
    idt_set_gate(10, (u32int) isr10, code_seg, flags);
    idt_set_gate(11, (u32int) isr11, code_seg, flags);
    idt_set_gate(12, (u32int) isr12, code_seg, flags);
    idt_set_gate(13, (u32int) isr13, code_seg, flags);
    idt_set_gate(14, (u32int) isr14, code_seg, flags);
    idt_set_gate(15, (u32int) isr15, code_seg, flags);
    idt_set_gate(16, (u32int) isr16, code_seg, flags);
    idt_set_gate(17, (u32int) isr17, code_seg, flags);
    idt_set_gate(18, (u32int) isr18, code_seg, flags);
    idt_set_gate(19, (u32int) isr19, code_seg, flags);
    idt_set_gate(20, (u32int) isr20, code_seg, flags);
    idt_set_gate(21, (u32int) isr21, code_seg, flags);
    idt_set_gate(22, (u32int) isr22, code_seg, flags);
    idt_set_gate(23, (u32int) isr23, code_seg, flags);
    idt_set_gate(24, (u32int) isr24, code_seg, flags);
    idt_set_gate(25, (u32int) isr25, code_seg, flags);
    idt_set_gate(26, (u32int) isr26, code_seg, flags);
    idt_set_gate(27, (u32int) isr27, code_seg, flags);
    idt_set_gate(28, (u32int) isr28, code_seg, flags);
    idt_set_gate(29, (u32int) isr29, code_seg, flags);
    idt_set_gate(30, (u32int) isr30, code_seg, flags);
    idt_set_gate(31, (u32int) isr31, code_seg, flags);

    // remap the Programmable Interrupt Controller to use the interripts 32-47
    
    PIC_remap(32, 40); // remap the PIC with master at address 32 and slave at address 40

    // now define the interrupt handlers for the PIC

    idt_set_gate(32, (u32int) irq0, code_seg, flags); // add an interrupt handler for interrupt 32 that points to the assembly routine irq0
    idt_set_gate(33, (u32int) irq1, code_seg, flags);
    idt_set_gate(34, (u32int) irq2, code_seg, flags);
    idt_set_gate(35, (u32int) irq3, code_seg, flags);
    idt_set_gate(36, (u32int) irq4, code_seg, flags);
    idt_set_gate(37, (u32int) irq5, code_seg, flags);
    idt_set_gate(38, (u32int) irq6, code_seg, flags);
    idt_set_gate(39, (u32int) irq7, code_seg, flags);
    idt_set_gate(40, (u32int) irq8, code_seg, flags);
    idt_set_gate(41, (u32int) irq9, code_seg, flags);
    idt_set_gate(42, (u32int) irq10, code_seg, flags);
    idt_set_gate(43, (u32int) irq11, code_seg, flags);
    idt_set_gate(44, (u32int) irq12, code_seg, flags);
    idt_set_gate(45, (u32int) irq13, code_seg, flags);
    idt_set_gate(46, (u32int) irq14, code_seg, flags);
    idt_set_gate(47, (u32int) irq15, code_seg, flags);

    // now we need to update the idt register in the cpu

    idt_flush((u32int)&IDT_ptr); // send a pointer to the idt pointer using the external assembly function

    // now we can finally enable interrupts whooo
    __asm__("sti");

}





// configure one of the IDT gate descriptors
void idt_set_gate(u8int num, u32int base, u16int segment_selector, u8int flags){
    // set the higher and lower bits of the base address
    IDT[num].base_lo = base & 0xFFFF;
    IDT[num].base_hi = (base >> 16) & 0xFFFF;

    // set the segment selector
    IDT[num].segment_selector = segment_selector;

    IDT[num].always_zero = 0; // this should always be zero and it is :) 
    
    // set the flags
    IDT[num].flags   = flags;
}






void PIC_remap(int offset1, int offset2)
{
    // code to remap the PIC from the internet
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}