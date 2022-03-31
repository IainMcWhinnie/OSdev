;#################################################################################################
;#---------------------------------------print_string32.asm--------------------------------------#
;#################################################################################################
;#  Prints to the screen in 32 Protected Mode by directly modifying the memory mapped I/O video  #
;#  memory at 0xb8000. Takes one parameter, the address of the start of the string, in EBX.      #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################



; define the logical address of the video memory
%assign videomemory 0xb8000

; define the colour of the text: 0 = black background and f = white text
%assign textcolour 0x0f



print_string_pm: ; prints null terminated string pointed at by EBX

    pusha ; push the contents of all registers onto the stack to preserve them
    mov edx, videomemory ; move the address of video memory into EDX

print_string_loop: ; start the printing loop
    ; move the current character pointed at by EBX into AL
    mov al, [ebx]
    ; mov to the higher bit the colour of the character to print
    mov ah, textcolour
    ; AX now holds two bytes of which will be written to video memory in that format

    cmp al, 0 ; check the character is not the Null-terminating character
    je print_string_done ; if it is then stop printing

    mov [edx], ax ; move the two bytes in AX which refer to the character and its format into the video memory address in EDX

    add ebx, 1 ; move to the next character (each character is 1 byte)
    add edx, 2 ; move to the next space in video memory (each cell is two bytes of information)

    ; print the next character
    jmp print_string_loop

print_string_done: ; done printing
    popa ; restore the registers
    ret ; return from the function

