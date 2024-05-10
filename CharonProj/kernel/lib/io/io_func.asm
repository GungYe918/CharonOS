; io_func.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, R8, R9
;

bits 64
section .text



global WriteIo32    ; void WriteIo32(uint16_t addr, uint32_t data)
WriteIo32:
    mov dx, di      ; RDI레지스터에 저장된 16비트 주소를 DX레지스터로 복사 -> 파라미터 Addr의 값과 동일
    mov eax, esi    ; RSI레지스터에 저장된 32비트 데이터를 EAX레지스터로 복사 -> 파라미터 Data의 값과 동일
    out dx, eax     ; DX레지스터에 지정된 I/O포트로부터 EAX레지스터에 저장된 32비트 데이터를 출력     
    ret             ; 함수 반환


global ReadIo32     ; uint32_t ReadIo32(uint16_t addr)
ReadIo32:
    mov dx, di      ; 앞의 기능과 알치
    in eax, dx      ; DX레지스터에 저장된 IO포트에서 32비트 정수를 EAX레지스터에 복사 
    ret