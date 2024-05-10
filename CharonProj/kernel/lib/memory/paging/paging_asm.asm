; gdt.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

global SetCR3               ; void SetCR3(uint64_t value)
SetCR3:
    mov cr3, rdi
    ret
