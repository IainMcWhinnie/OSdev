//#################################################################################################
//#-----------------------------------------common.h----------------------------------------------#
//#################################################################################################
//#  Header file for "common.c". Contains useful type definitions and prototype for IO functions  #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

#ifndef COMMON_H
#define COMMON_H


// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

//Prototypes for function
void outb(u16int port, u8int value);
void outw(u16int port, u16int value);
u8int inb(u16int port);
u16int inw(u16int port);

void io_wait(void);


#endif