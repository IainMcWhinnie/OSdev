;#################################################################################################
;#-----------------------------------------interrupts.asm----------------------------------------#
;#################################################################################################
;#  Defines interrupt handler routines and an IDT loading routine. Each interrupt handler is     #
;#  defined by a macro and simply pushes its interrupt number and calls the common handler with  #
;#  calls the high level c functions in "isr.c".                                                 #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################


; first declare a function to load our idt table
GLOBAL _idt_flush    ; Allows the C code to call idt_flush().

_idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]        ; Load the IDT pointer.
   ret


; now we define some macros to set up an interrupt handler for everyone of the first 32 interrupts in the IDT
; if the interrupt doesnt push an error (interrupts 8, 10-14 & 17) then we'll push a dummy one
; all the functions call the common interrupt handler isr_common_stub and identify themselves by pushing their interrupt number onto the stack
%macro ISR_NOERRCODE 1
  GLOBAL _isr%1        ; %1 accesses the first parameter.
  _isr%1:
    cli ; first disable other interrupts
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  GLOBAL _isr%1
  _isr%1:
    cli ; first disable other interrupts
    push byte %1 ; only need to push the interrupt number since the error is already pushed onto the stack
    jmp isr_common_stub
%endmacro

; define the first 32 interrupt handlers as either no error code or error code
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


extern _isr_handler ; defined in "isr.c"

; now define the common interrupt handler
isr_common_stub:
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; get the current data segment descriptor
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call _isr_handler ; call the c handler functions

   pop eax        ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti            ; the intial interrupt handler disabled interrupts so enable them again now we are done
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP (special instruction designed for returning from interrupts)



; now create a macro to use to define handlers for the PIC irqs
%macro IRQ 2
  GLOBAL _irq%2
  _irq%2:
    cli
    push byte 0
    push byte %1
    jmp irq_common_stub
%endmacro

IRQ 32, 0
IRQ 33, 1
IRQ 34, 2
IRQ 35, 3
IRQ 36, 4
IRQ 37, 5
IRQ 38, 6
IRQ 39, 7
IRQ 40, 8
IRQ 41, 9
IRQ 42, 10
IRQ 43, 11
IRQ 44, 12
IRQ 45, 13
IRQ 46, 14
IRQ 47, 15

extern _irq_handler

irq_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; get the current data segment descriptor
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call _irq_handler ; call c handler functions

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP