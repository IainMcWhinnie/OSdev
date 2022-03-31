;#################################################################################################
;#---------------------------------------bootloader.asm------------------------------------------#
;#################################################################################################
;#  A bootloader for the OS. The bootloader does the following:                                  #
;#     1. Prints a debug message to show it has started.                                         #
;#     2. Loads 5 sectors of kernel into ram at the offset defined by kerneloffset               #
;#     3. Prints a debug message to show it has loaded the kernel                                #
;#     4. Configures the GDT and starts 32bit Protected Mode                                     #
;#     5. Prints a debug message to show it has successfully start Protected Mode                #
;#                                                                                               #
;#  bootloader.asm will compile to be a 512 bytes long file named bootloader.bin. This file is   #
;#  placed at sector 0 of the disk and is loaded into RAM by BIOS at the address 0x7c00. It      #
;#  makes use of BIOS interrupts to perform its various functions.                               #
;#                                                                                               #
;#  Most of the code is taken from elsewhere, i.e. variety of online bootloader/OS tutorials.    #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################


; the computer initially boots in 16 Real Mode so tell the compiler this
[bits 16]

; tell the compiler that the program is loaded in RAM at 0x7c00 so that addresses can be corrected likewise
[org 0x7c00]

; the kernel should be loaded into RAM into RAM at 0x1000
%assign kernaloffset 0x1000



; the BIOS stores the drive number of the drive booted from in the register dl
; store this value in the variable BOOT_DRIVE as we will need it later
; for some reason on my samsung laptop the BIOS gives ds (data segment register) a value of 0x400
; for the BIOS routines to work ds must still be 0x400 so simply use es instead
mov ax, 0
mov es, ax ; make sure es register has a value of 0
mov [es:BOOT_DRIVE] , dl ; store the value of dl in BOOT_DRIVE

; print a debug string. The argument is an address in the bx register
mov bx, real_mode_message
call print_string ; call the print routine from "print_lib.asm"

; set up the stack at 0x9000
mov bp, 0x9000
mov sp, bp

; load the kernel

; the data will be loaded at the address ES:BX
mov bx, kernaloffset ; mov the address to bx, es has already been set to 0
mov dh, 50 ; load 5 sectors
mov dl, [es:BOOT_DRIVE] ; load the boot drive number into dl
call disk_load ; call the function disk load with the arguments (DH: no. sectors, DL: drive number, ES:BX: data buffer), implemented in "disk_lib.asm"

; print another debug string this time to show weve successfully loaded the kernel
mov bx, loaded_kernel_msg ; pass the address of the string as an argument in bx
call print_string ; call the printing function

; switch to Protected Mode function
; this function will disable interrupts then initialise the GDT and finally reset the segment registers
; interrupts are not reanabled in this function
; the GDT implements a Basic Flat Model as described by the Intel Developers Manual
; one segment for code and the other for data both with privilege level 0
; the function switch_to_pm should never return
call switch_to_pm

; the code never executes to here but if it does...
jmp $ ; ...never ending loop!

; define imports that are defined in 16 bit assembly
%include "./src/boot/disk_lib.asm"
%include "./src/boot/print_lib.asm"

; The next import converts from 16 bit to 32 bit assembly
%include "./src/boot/start_pm.asm"

;-------------------------------------------------------- 32 bit assembly -----------------------------------------------------

; we now execute 32 bit assembly

; this is where we arrive after switching to and initialising Protected Mode
BEGIN_PM :

; print a debug message to show we have successfully arrived in Protected Mode
mov ebx , protected_mode_message ; pass the argument of the string address
call print_string_pm ; Use our 32- bit print routine.

; now we can jump to the start of the program execution by calling the address at kerneloffset
call kernaloffset

; again the code never executes to here but if it does...
jmp $ ; ...never ending loop!


; 32 bit assembly imports
%include "./src/boot/gdt.asm"
%include "./src/boot/print_string32.asm"

; data and variables definitions
real_mode_message db "Started in 16-bit Real Mode. ", 0
loaded_kernel_msg db "Loaded kernel Successfully! ", 0
protected_mode_message db "Successfully landed in 32-bit Protected Mode. ", 0
BOOT_DRIVE : db 0

; bootsector padding
; to make it a valid boot sector it must be 512 bytes long and end with the magic number 0xaa55
times 510 -($-$$) db 0 ; pad with zeros until 510 bytes long
dw 0xaa55 ; write the last two bytes as the magic number