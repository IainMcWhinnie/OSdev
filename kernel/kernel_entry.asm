;#################################################################################################
;#--------------------------------------kernel_entry.asm-----------------------------------------#
;#################################################################################################
;#  Sits at the start of the kernel compilation and calls the start function of the kernel.      #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################

; the code is 32 bit assembler
[bits 32]
; the start function is defined elsewhere (in kernel.c), underscore on C functions in assembly is a convention used by the compiler
[extern _start]


; call the start function
call _start

; The kernel function should never return but if it does then loop infinitely
jmp $