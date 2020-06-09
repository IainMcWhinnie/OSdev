;#################################################################################################
;#-----------------------------------------disk_lib.asm------------------------------------------#
;#################################################################################################
;#  Loads data from disk in 16 bit Real Mode using BIOS interrupts. Parameters are:              #
;#      ES:BX --> where the data is copied to in RAM                                             #
;#      DH    --> number of sectors to load                                                      #
;#      DL    --> drive number                                                                   #
;#  The function will check to see if the load was successful. If not it will print an error     #
;#  message and enter an infinite loop.                                                          #
;#                                                                                               #
;#  disk_lib.asm is dependant on print_lib.asm                                                   #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################



disk_load :

    ; store the value of dx on the stack so we can later use for error checking
    push dx 

    ; now populate the registers with the correct parameters expected by the BIOS interrupt
    ; from http://www.ctyme.com/intr/rb-0607.htm the parameters are

    mov ah, 0x02 ; AH -- we want the BIOS read sector function
    mov al, dh   ; AL -- how many sectors to read, in this case we read the number of sectors passed as an argument in DH
                 ; BX -- where to write the data, already set as an argument
    mov ch, 0x00 ; CH -- cylinder number ( = 0 )
    mov cl, 0x02 ; CL -- which sector to start reading from, in this case number 2: the one after the boot sector
    mov dh, 0x00 ; DH -- head number ( = 0 )
                 ; DL -- driver number, already set as an argument

    int 0x13 ; call the BIOS interrupt

    jc disk_error ; the BIOS interrupts sets the carry flag if an error occurs, therefore jump to the error code

    pop dx ; restore DX from the stack to check the number of sectors
    ; the BIOS interrupt sets AH to the number of sectors it managed to read
    cmp dh, al ; if AL ( sectors read ) != DH ( sectors expected )
    jne disk_error ; number of sectors dont match therefroe jump to error code

    ; no errors have occured so return
    ret

disk_error:
    ; an error has occured, print a debug message
    ; using the function defined in "print_lib.asm" which this code is dependant on
    mov bx, DISK_ERROR_MSG ; pass the string as an argument in BX
    call print_string ; call the print function

    ; loop infinitely, there has been an fatal error and further execution of the bootloader cannot go ahead
    jmp $

; define the error message string
DISK_ERROR_MSG db " Disk read error ! ", 0