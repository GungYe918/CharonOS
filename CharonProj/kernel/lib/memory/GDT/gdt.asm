; gdt.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

global LoadGDT              ; void LoadGDT(uint16_t limit, uint64_t offset)
LoadGDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di           ; limit
    mov [rsp + 2], rsi      ; offset
    lgdt [rsp]
    mov rsp, rbp
    pop rbp
    ret

global SetCSSS              ; void SetCSSS(uint16_t cs, uint16_t ss)
SetCSSS:
    push rbp
    mov rbp, rsp
    mov ss, si
    mov rax, .next
    push rdi                ; CS
    push rax                ; RIP
    o64 retf
.next:
    mov rsp, rbp
    pop rbp
    ret

global SetDSAll             ; void SetDSAll(uint16_t value)
SetDSAll:
    mov ds, di
    mov es, di
    mov fs, di
    mov gs, di
    ret

