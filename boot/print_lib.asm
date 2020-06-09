;#################################################################################################
;#-----------------------------------------print_lib.asm-----------------------------------------#
;#################################################################################################
;#  Prints to the screen in 16 bit Real Mode using BIOS interrupts. Takes one parameter in bx    #
;#  that is the address of the first character in the string.                                    #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################



; prints a string to the screen in text mode, takes one parameter the address of the string in register bx
print_string:
    ; move the character of the string pointed at by bx into al. Use es because ds should still has the value 0x400
    mov al, [es:bx]
    ; mov the print routine command 0x0e into ah
    mov ah, 0x0e
    ; call BIOS interrupt 0x10, documented at http://www.ctyme.com/intr/rb-0106.htm
    int 0x10

    ; move to the next character by adding 1 to the address (each character is 1 byte)
    add bx, 1
    ; again move the character of the string pointed at by bx into al
    mov al, [es:bx]
    ; check if the character is a string terminating zero byte
    cmp al, 0x00
    ; if it equals zero then stop printing
    je stop
    ; otherwise print the next character and repeat
    jmp print_string
    stop:
    ; return the function
    ret

