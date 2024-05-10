; new_entry.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

extern kernel_main_stack
extern KernelMainNewStack

global KernelMain 
KernelMain:
    mov rsp, kernel_main_stack + 1024 * 1024
    call KernelMainNewStack

.fin:
    hlt
    jmp .fin