/**
 * @file interrupt.cpp
 * 
 * Interrupt funcs
*/

#include "interrupt.hpp"


struct InterruptDescriptor idt[IDT_SIZE];
/**
 * IDT(Interrupt Descriptor Table)의 정의 코드
 * 
 * 이 코드는 IDT를 정의하는 코드로, c++의 array 클래스를 사용하여 
 * Interrupt Descriptor을 256개 나열한 배열로 구성되어 있다.
 * 
*/

void SetIDTEntry (
    InterruptDescriptor& desc,
    InterruptDescriptorAttribute attr,
    uint64_t offset,
    uint16_t segment_selector
) {
    desc.attr = attr;
    desc.offset_low = offset & 0xffffu;
    desc.offset_middle = (offset >> 16) & 0xffffu;
    desc.offset_high = offset >> 32;
    desc.segment_selector = segment_selector;
}


void NotifyEndOfInterrupt() {
    volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
    /**
     * end_of_interrupt라는 변수를 0xfee000b0주소로 reinterpret한다.
     * 
     * volatile을 붙이는 이유는 0xfee000b0의 위치에 값을 쓰는 동작을 
     * 정확하게 수행하기 위함이며, volatile이 붙은 코드는 컴파일러의 최적화 대상이
     * 되지 않는다.
    */

    *end_of_interrupt = 0;
    /** 0xfee000b0주소(end_of_interrupt)의 값을 0으로 설정한다.*/
}

/**
 * PLUS:
 *  메인 메모리에 값을 쓰는 것으로 interrupt를 설정할 수 있는 이유:
 *      보통 메인 메모리에 값을 쓰는 것은 단순히 그 위치에 값을 저장하는 것뿐,
 *      다른 효과는 없다. 그러나 이번 코드에서 사용된 주소 <0xfee000b0>은 
 *      메인 메모리의 주소가 아니라 CPU의 레지스터 주소이다.
 *      CPU 레지스터의 주소는 0xfee00000 ~ 0xfee00400까지의 주소로 총
 *      1024바이트의 범위이다.
 * 
*/
