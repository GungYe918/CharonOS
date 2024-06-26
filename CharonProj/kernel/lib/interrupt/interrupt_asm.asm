; interrupt.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

global GetCS                ; uint16_t GetCS(void)
GetCS:
    xor eax, eax
    mov ax, cs
    ret

global LoadIDT              ; void LoadIDT(uint16_t limit, uint64_t offset)
LoadIDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di           ; limit
    mov [rsp + 2], rsi      ; offset
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret
