/**
 * @file segment.hpp
 * 
 * GDT와 메모리 segment를 정의한다.
*/

#pragma once 

#include <array>
#include <cstdint>

#include "../x86_descriptor.hpp"


/*  segment desc 를 정의한다. */
union SegmentDescriptor {
    
    uint64_t data;

    struct {

        uint64_t limit_low                      : 16;       // 세그먼트 크기의 하위 16바이트
        uint64_t base_low                       : 16;       // 세그먼트 베이스 주소의 하위 16비트
        uint64_t base_middle                    :  8;       // 세그먼트의 베이스 주소의 중간 8비트
        DescriptorType type                     :  4;       // 세그먼트 유형을 나타내는 4비트 필드
        uint64_t system_segment                 :  1;       // 시스템 세그먼트 여부를 나타내는 1비트 필드
        uint64_t descriptor_privilege_level     :  2;       // 디스크립터의 권한 레벨을 나타내는 2비트 필드
        uint64_t present                        :  1;       // 세그먼트의 존재 여부를 나타내는 1비트 필드
        uint64_t limit_high                     :  4;       // 세그먼트의 크기의 상위 4비트
        uint64_t available                      :  1;       // 시스템 소프트웨어에 제공되는 추가 필드 -> 보통은 0으로 설정됨
        uint64_t long_mode                      :  1;       // 64비트 모드(롱 모드)여부를 판별하는 1비트 필드
        uint64_t default_operation_size         :  1;       // 32비트 디폴트 오퍼레이션 크기를 나타내는 1비트 필드
        uint64_t granularity                    :  1;       // 한계 필드의 단위를 바이트(0) 또는 4KB(1)로 지정하는 1비트 필드
        uint64_t base_high                      :  8;       // 세그먼트의 베이스 주소의 상위 8비트

    } __attribute__((packed)) bits;

} __attribute__((packed)); 



// 코드 세그먼트 부분을 설정하는 함수
void SetCodeSegment (
    SegmentDescriptor& desc,
    DescriptorType type,
    unsigned int descriptor_privilege_level,
    uint32_t base,
    uint32_t limit
);


// 데이터 세그먼트 부분을 설정하는 함수
void SetDataSegment (
    SegmentDescriptor& desc,
    DescriptorType type,
    unsigned int descriptor_privilege_level,
    uint32_t base,
    uint32_t limit
);


// GDT의 3개의 디스크럽터에 값을 설정하는 함수
void SetupSegments();

