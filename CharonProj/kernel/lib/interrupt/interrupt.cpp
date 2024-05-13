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
    desc.attr = attr;                                   // 1)
    desc.offset_low = offset & 0xffffu;                 // 2)
    desc.offset_middle = (offset >> 16) & 0xffffu;      // 3)
    desc.offset_high = offset >> 32;                    // 4)   
    desc.segment_selector = segment_selector;           // 5)
}

/**
 * 이 함수는 파라미터 desc로 지정한 인터럽트 디스크럽터에 
 * 여러 설정을 기록하는 함수이다.
 * 
 * 동작방식:
 *  1) 함수 내에 전달된 attr값을 desc의 attr맴버에 할당한다.
 * 
 *  2) 함수에 전달된 offset값의 낮은 16비트만 추출하여 
 *     desc구조체의 offset_low에 저장한다.
 *      
 *  3) 함수에 전달된 offset값의 중간 16비트를 추출하여 
 *     desc구조체의 offset_middle 값에 저장한다.
 * 
 *  4) 함수에 전달된 offset값의 높은 16비트를 추출하여
 *     desc구조체의 offset_high 값에 저장한다.
 *     
 *  5) 인터럽트 핸들러가 위치한 코드 세그먼트를 가리키는 
 *     세그먼트 셀렉터를 저장한다. 
 *  
*/




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
