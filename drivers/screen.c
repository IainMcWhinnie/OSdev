//#################################################################################################
//#-----------------------------------------screen.c----------------------------------------------#
//#################################################################################################
//#  Contains useful functions for writing to the screen in text mode via memory                  #
//#################################################################################################
//#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
//#################################################################################################


// This source's header file
#include "screen.h"
#include "./../kernel/common.h"


// initialise the cursors position on screen
u8int cursor_x = 0;
u8int cursor_y = 0;

// move the position of the cursor to the current values of x and y
void move_cursor(){
    u16int cursorLocation = cursor_y * 80 + cursor_x; // offset in video memory
    outb(REG_SCREEN_CTRL,14); // load high byte of posistion
    outb(REG_SCREEN_DATA, cursorLocation>>8); 
    outb(REG_SCREEN_CTRL,15); // load low byte
    outb(REG_SCREEN_DATA, cursorLocation);
}

// print a character to the screen at the current posisiton then update the cursor and screen to the next printing posistion
void printchar(char ascii){
    u16int cursorLocation = cursor_y * 80 + cursor_x; // offset in video memory

    if(ascii != 10 & ascii != 9 & ascii!=8){ // if the character to print is not the newline character or tabular character or backspace
        char *video_memory = (char*) VIDEO_ADDRESS; // point to the video memory
        video_memory[cursorLocation*2] = ascii; // set the ascii data in video memory
        video_memory[cursorLocation*2+1] = WHITE_ON_BLACK; // set the format data
        cursor_x += 1; // move the cursor right one spcae
    }else if(ascii == 10){ // newline character
        cursor_x = 0; // carriage return
        cursor_y += 1; // new line
    }else if(ascii == 9){
        cursor_x += 4;
    }else if(ascii == 8){
        if(cursor_x != 0){
            cursor_x -= 1;
            printchar(' ');
            cursor_x -= 1;
        }else{
            if(cursor_y != 0){
                cursor_y -= 1;
                cursor_x = MAX_COLS-1;
                printchar(' ');
                cursor_y -= 1;
                cursor_x = MAX_COLS-1;
            }
        }
    }

    if(cursor_x >= MAX_COLS){ // the cursor has gone off the right side of the screen
        cursor_x = 0; // move back to the start of...
        cursor_y += 1; // ...a new line
    }
    if(cursor_y >= MAX_ROWS){ // the cursor has gone off the bottom of the screen
        scrollscreen(); // scroll the contents of the screen
        cursor_y -= 1; // move back up one line to the last line of the screen
        cursor_x = 0; // move back to the start of the line
    }
    move_cursor(); // update the cursor position ready for the next printed character
}

// print a string
void printstring(char string[]){
    int i = 0; // counter starts at 0 for the first character
    while(string[i]){ // while not pointing at the null terminating character
        printchar(string[i]); // print the character in the string
        i++; // move to the next character
    }
}

// clear the screen
void clearscreen(){
    cursor_x = 0; // set the cursor to the beginning of the screen
    cursor_y = 0;
    for( int i=0; i<MAX_ROWS*(MAX_COLS);i++){ // loop through ever space on screen
        printchar(0x20); // overwrite the data with the space character
    }
    cursor_x = 0; // reset the cursor to the beginning of the screen
    cursor_y = 0;
}


// scroll screen
void scrollscreen(){
    char *video_memory = (char*) VIDEO_ADDRESS; // reference the video memory
    for(int i = 0; i < MAX_COLS*(MAX_ROWS-1); i++){ // loop through ever character of every row except the last row 
        video_memory[i*2] = video_memory[(i+MAX_COLS)*2]; //set the data of the character to the data from the column below
    }
    for(int i = MAX_COLS*(MAX_ROWS-1); i < MAX_COLS*(MAX_ROWS); i++){ // loop through ever character of the last row
        video_memory[i*2] = 0x20; // set the characters to space to clear the row
    }
}

// print a 32-bit hex number
void printhex(u32int n){
    // print the 0x identifier for a hex number
    printchar('0');
    printchar('x');

    for(int i=7; i>=0; i--){ //loop through each hex character (4 bits/char * 8 chars = 32 bits)
        int upper = (n&(0xf<<i*4))>>i*4; // select the data for the hex digit by and-ing with a shift of 0xf then shift it so it is in the first byte
        char l = upper+'0'; // convert the number to ascii digit by adding the value of zero
        if(l<='9'){ // the hex is printable as a digit
            printchar(l); // just print it
        }else{ // the hex isnt printed as a digit from 0-9
            printchar(l+7); // add the offset ('A'-'0'-10=7) to make it a uppercase letter
        }
    }
}

// print a byte hex number
void printhexbyte(u8int n){
    // print the 0x identifier for a hex number
    for(int i=1; i>=0; i--){ //loop through each hex character (4 bits/char * 8 chars = 32 bits)
        int upper = (n&(0xf<<i*4))>>i*4; // select the data for the hex digit by and-ing with a shift of 0xf then shift it so it is in the first byte
        char l = upper+'0'; // convert the number to ascii digit by adding the value of zero
        if(l<='9'){ // the hex is printable as a digit
            printchar(l); // just print it
        }else{ // the hex isnt printed as a digit from 0-9
            printchar(l+7); // add the offset ('A'-'0'-10=7) to make it a uppercase letter
        }
    }
}

// print out a denery number
void printdec(u32int n){
    int div = 1000000000; // the largest power of ten we will need for a 32 bit number
    for(int i=0; i<10; i++){
        int mod = n/div; // integer division, gets the current digit
        if( mod != 0){ // if the digit is 0 dont bother printing it
            char digit = (char) mod; // convert to a character
            digit += '0'; // make it an ascii character
            printchar(digit); // print it to screen
            n = n - mod*div; // no subtract it from n so that we can single out following digits
        }
        div /= 10; // move down to the next power of ten
    }
}