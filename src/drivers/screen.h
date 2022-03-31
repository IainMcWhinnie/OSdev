//#################################################################################################
//#-----------------------------------------screen.h----------------------------------------------#
//#################################################################################################
//#  Header file for "screen.c". Contains definitions for screen parameters and prototypes for    #
//#  screen printing and refreshing functions.                                                    #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################

#ifndef SCREEN_H
#define SCREEN_H


// NECCESSARY include
#include "./../kernel/common.h"


// Definitions
#define VIDEO_ADDRESS   0xb8000//Address of memory mapped I/O for the video card memory in text mode
#define MAX_ROWS        25    // Number of rows in display
#define MAX_COLS        80    // Number of columns in display

#define WHITE_ON_BLACK  0x0f //0xf9  // Attribute byte for our default colour scheme .
#define REG_SCREEN_CTRL 0x3D4 // Screen device I/O ports, CONTROL I/O port
#define REG_SCREEN_DATA 0x3D5 // DATA I/O port


// Prototypes
void printchar(char ascii);
void printstring(char string[]);
void printhex(u32int n);
void printhexbyte(u8int n);
void printdec(u32int n);
void clearscreen();
void move_cursor();
void scrollscreen();


#endif