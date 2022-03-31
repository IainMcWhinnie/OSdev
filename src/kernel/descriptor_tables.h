//#################################################################################################
//#-----------------------------------descriptor_tables.h-----------------------------------------#
//#################################################################################################
//#  Header file for "descriptor_tables.c". Contains data types for IDT entries and pointers.     #
//#  Also features a lot of externs for assembly defined interrupt handlers.                      #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "common.h"

// define a struct for a IDT gate descriptor
// the IDT is an array of at max 256 of these

typedef struct idt_gate_descriptor{
    u16int base_lo;
    u16int segment_selector;
    u8int always_zero;
    u8int flags;
    u16int base_hi;
}__attribute__((packed)) idt_gate_descriptor_t;


// define a struct that points to the address of the IDT
// this data will be loaded with the lidt command
typedef struct idt_descriptor{
    u16int limit;
    u32int base;
}__attribute__((packed)) idt_descriptor_t;

// posiblities for the flags byte for an idt entry
#define IDT_GATE_FLAG_32BIT 0b00001000
#define IDT_GATE_FLAG_CONST 0b00000110
#define IDT_GATE_FLAG_PRESENT 0b10000000


// prototypes
void init_idt();
void idt_set_gate(u8int num, u32int base, u16int segment_selector, u8int flags);
void PIC_remap(int offset1, int offset2);


// extern to assembly interrupt handlers
extern void idt_flush(u32int addr);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// now the externs for the irqs

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


// define constants used for reseting the Programmable Interrupt Controller

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */



#endif