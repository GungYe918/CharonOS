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
    uint16_t data;
    
    struct {

        uint16_t interrupt_stack_table : 3;
        uint16_t : 5;
        DescriptorType type : 4;
        uint16_t : 1;
        uint16_t descriptor_privilege_level : 2;
        uint16_t present : 1;

    } __attribute__((packed)) bits;

} __attribute__((packed));

/**
 * PLUS:
 *  __attribute((packed))__의 의미:
 *      이 지시어는 컴파일러에게 구조체나 클래스의 맴버 변수들을 
 *      패딩 없이(구조체나 클래스 내의 각 맴버 사이에 추가 바이트 삽입 없이)
 *      메모리에 연속적으로 배치하도록 지시한다. Interrupt와 관련된 코드는 
 *      하드웨어 사양대로 정확한 위치에 삽입되어야 하기 때문에 컴파일러가 추가적인
 *      바이트를 삽입하지 않도록 하는 것이다. 
 * 
*/

struct InterruptDescriptor {

    uint16_t offset_low;
    uint16_t segment_selector;
    InterruptDescriptorAttribute attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;

} __attribute__((packed));



extern struct InterruptDescriptor idt[IDT_SIZE];



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


class InterruptVector {
    public:
        enum Number {
            kXHCI = 0x40,
        };
};


struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void NotifyEndOfInterrupt();
