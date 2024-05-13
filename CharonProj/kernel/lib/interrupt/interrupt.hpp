/**
 * @file interrupt.hpp
 * 
 * define interrupt
*/

#pragma once

#include <array>
#include <cstdint>
#include "../memory/x86_descriptor.hpp"
#define IDT_SIZE 256

union InterruptDescriptorAttribute {
    uint16_t data;                                  // 인터럽트 디스크럽터의 속성을 저장한다.
    
    struct {

        uint16_t interrupt_stack_table : 3;         // 인터럽트 스택 테이블을 나타내는 3비트 크기의 필드이다.
        uint16_t : 5;                               // 패딩 역할을 하는 5비트 크기의 빈 공간이다.
        DescriptorType type : 4;                    // 디스크럽터 유형을 나타내는 4비트 크기의 필드이다.
        uint16_t : 1;                               // 패딩 역할을 하는 1비트 크기의 빈 공간이다.
        uint16_t descriptor_privilege_level : 2;    // 디스크럽터의 권한 수준을 나타내는 2비트 크기의 필드이다.
        uint16_t present : 1;                       // 디스크럽터의 존재 여부를 나타내는 1비트 크기의 필드이다.

    } __attribute__((packed)) bits;

} __attribute__((packed));

/**
 * PLUS:
 * 패딩의 역할과 필요한 이유:
 *      패딩은 데이터 구조체나 union에서 사용되는 기술로, 메모리 정렬을 위해 추가적인 비트를 
 *      사용하는 것을 의미한다. 패딩이 필요한 이유는 대체로 하드웨어적인 상호작용 때문으로, 
 *      데이터가 특정 위치에 있다는 것을 보장함으로서 성능을 향상시킬 수 있다.
 *      따라서 본 코드에서도 각각 5비트와 1비트의 패딩이 사용되었다.
 * 
 * PLUS:
 *  union의 의미:
 *      c++에서 union은 메모리를 공유하는 여러 맴버를 가진 데이터 형식이다. 
 *      union(공용체) 내의 각 맴버들은 같은 메모리 위치를 공유하기 때문에, 
 *      한 번에 오직 하나의 맴버만 값을 가질 수 있다. 따라서 공용체의 크기는 그 안의
 *      가장 큰 맴버의 크가와 같다.
 * 
 * PLUS:
 *  interrupt_stack_table의 정의:
 *      interrupt_stack_table은 인터럽트 발생 시 해당 인터럽트에 대한 처리를 
 *      위해 사용되는 스택 테이블을 가리키는 필드이다. 시스템은 인터럽트를 처리할 때 
 *      일반적으로 현재 실행중인 프로세스의 스택 테이블을 사용하지 않고, 이렇게 별도의 
 *      스택 공간을 사용한다. 이 때문에 인터럽트 처리 도중에 다른 인터럽트가 발생하여도 
 *      스택이 꼬이지 않고 안정적인 처리가 가능하다.
 * 
 * PLUS:
 *  __attribute((packed))__의 의미:
 *      이 지시어는 컴파일러에게 구조체나 클래스의 맴버 변수들을 
 *      패딩 없이(구조체나 클래스 내의 각 맴버 사이에 추가 바이트 삽입 없이)
 *      메모리에 연속적으로 배치하도록 지시한다. Interrupt와 관련된 코드는 
 *      하드웨어 사양대로 정확한 위치에 삽입되어야 하기 때문에 컴파일러가 추가적인
 *      바이트를 삽입하지 않도록 하는 것이다. 
*/

struct InterruptDescriptor {

    uint16_t offset_low;                    // 인터럽트 핸들러의 낮은 16비트에 해당하는 메모리 오프셋을 저장한다.
    uint16_t segment_selector;              // 인터럽트 핸들러가 위치한 코드 세그먼트를 가리키는 세그먼트 셀렉터를 저장한다. 
    InterruptDescriptorAttribute attr;      // 인터럽트 디스크럽터의 속성을 저장한다.
    uint16_t offset_middle;                 // 인터럽트 핸들러의 중간 16비트에 해당하는 메모리 오프셋을 저장한다.
    uint32_t offset_high;                   // 인터럽트 핸들러의 높은 16비트에 해당하는 메모리 오프셋을 저장한다.
    uint32_t reserved;                      // 예약된 필드로, 지금은 사용되지 않으나, 미래의 확장을 위해 확보된 공간이다. 

} __attribute__((packed));

/**
 * PLUS:
 *  인터럽트 핸들러를 설정하는 필드가 3개 존재하는 이유:
 *      x86 아키텍처는 16비트, 32비트, 64비트에 대한 호환성을 
 *      유지하며 진화해왔기 때문에, 각각의 비트에 맞는 운영체제에 대한 
 *      호환성을 제공하기 위해 필드가 3개로 나뉘어졌다. 
 *      따라서 인터럽트 핸들러를 설정하는 경우, 위 3가지 필드를 조합해서
 *      64비트 주소를 설정하게 된다. 
*/



extern struct InterruptDescriptor idt[IDT_SIZE];
/**
 * IDT를 정의한다.
 * 
 * PLUS:
 *  IDT의 구조:
 *      IDT는 인터럽트 번호와 인터럽트 핸들러를 매핑하기 위한 테이블로
 *      메인 메모리 상에 배치되어 있다. 0에서 255까지의 인터럽트 번호에 대해 
 *      각각의 다른 인터럽트 핸들러를 지정할 수 있다. 인터럽트 핸들러를 IDT에
 *      등록하게 되면, 실제 인터럽트가 발생하였을 때 등록된 핸들러가 호출되게 된다.
 *      
*/



constexpr InterruptDescriptorAttribute MakeIDTAttr(
    DescriptorType type, 
    uint8_t descriptor_privilege_level,
    bool present = true,
    uint8_t interrupt_stack_table = 0

) {
    InterruptDescriptorAttribute attr{};

    attr.bits.interrupt_stack_table = interrupt_stack_table;
    attr.bits.type = type;
    attr.bits.descriptor_privilege_level = descriptor_privilege_level;
    attr.bits.present = present;

    return attr;
}



void SetIDTEntry (
    InterruptDescriptor& desc,
    InterruptDescriptorAttribute attr,
    uint64_t offset,
    uint16_t segment_selector
);

/**
 * interrupt.cpp에서 자세하게 설명
*/


class InterruptVector {
    public:
        enum Number {
            kXHCI = 0x40,
        };
};

/**
 * @brief 인터럽트 벡터를 설정한다.
 * 
 * 인터럽트 벡터란 커널이 처리할 256가지의 
 * 인터럽트를 효율적으로 관리하기 위해 인터럽트의
 * 종류마다 다른 번호를 할당하여 관리하는 구조를 말한다.
 * 
 * 인터럽트 벡터는 고정적으로 번호가 할당된 것도 있고, 
 * 개발자의 마음대로 설정할 수 있는 것도 있다.
 * 예를 들어 0으로 나누는 것에 관한 인터럽트는 0번으로 고정되어 있다.
 * 
 * 이 코드에서 XHCI의 경우 USB과 관련된 인터럽트로, 개발자
 * 마음대로 설정할 수 있다.
 * 
*/


struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

/**
 * @brief 인터럽트가 발생했을 때 스택에 저장되는 인터럽트 프레임을 정의한다.
 * 
 * rip:
 *  현재 실행중인 명령의 주소(Instruction Pointer)를 나타내는 64비트 정수형 변수
 * 
 * cs:
 *  현재 실행중인 코드 세그먼트(CS)의 세그먼트 셀렉터를 나타내는 64비트 정수형 변수
 * 
 * rflags:
 *  현재 프로세스의 플래그 레지스터(RFLAGS)를 나타내는 64비트 정수형 변수
 * 
 * rsp:
 *  현재 스택의 주소를 나타내는 64비트 정수형 변수
 * 
 * ss:
 *  현재 실행중인 코드 세그먼트(SS)의 세그먼트 셀렉터를 나타내는 64비트 정수형 변수
 *  
*/

void NotifyEndOfInterrupt();
/**
 * @brief 인터럽트가 종료되었음을 알리는 메모리를 특정 주소에 작성하는 함수
 * 
*/
