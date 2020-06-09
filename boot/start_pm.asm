;#################################################################################################
;#------------------------------------------start_pm.asm-----------------------------------------#
;#################################################################################################
;#  Sets up the GDT and initialises Protected Mode. This function never returns and instead      #
;#  jumps to the label BEGIN_PM where execution in 32 bit PM mode can begin.                     #
;#  Depends on "gdt.asm"                                                                         #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################



; Switch to protected mode
switch_to_pm:

    cli ; switch off interrupts until we have set up the PM interrupt descriptor table (IDT) otherwise interrupts will create CPU errors.
    
    lgdt [es:gdt_descriptor] ; load the global descriptor table defined in "gdt.asm"

    ; to make the switch to protected mode, we set the first bit of CR0, a control register
    mov eax, cr0 ; load the control register
    or eax, 0x1 ; set the first bit high
    mov cr0, eax ; save the control register

    jmp CODE_SEG:init_pm ; make a far jump (i.e. to a new segment) to our 32-bit
    ; code. This also forces the CPU to flush its cache of
    ; pre-fetched and real-mode decoded instructions, which can
    ; cause problems.

;-------------------------------------------------------- 32 bit assembly -----------------------------------------------------
; we now define the use of 32 bit assembly
[bits 32]

; first we should initialise registers and the stack once in PM.
init_pm:
    mov ax , DATA_SEG ; now in PM , our old segments are meaningless ,
    mov ds, ax        ; so we point our segment registers to the
    mov ss, ax        ; data selector defined in "gdt.asm"
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; update our stack position so it is right
    mov esp, ebp ; at the top of the free space.

    call BEGIN_PM ; finally call the start of 32 bit execution in the bootloader