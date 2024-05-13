#pragma once 

#include <stdint.h>

struct MemoryMap {

    unsigned long long buffer_size;             // 메모리 맵 정보를 저장하는 버퍼의 크기
    void* buffer;                               // 메모리 맵 정보를 저장하는 버퍼의 포인터
    unsigned long long map_size;                // 메모리 맵의 크기
    unsigned long long map_key;                 // 메모리 맵의 고유 키 -> 메모리 맵의 정보의 업데이트 여부를 판단
    unsigned long long descriptor_size;         // 메모리 디스크럽터의 크기
    uint32_t descriptor_version;                // 메모리 디스크럽터의 버전 -> 메모리 디스크럽터의 형식, 구조 변경 여부를 판별
    
};

struct MemoryDescriptor {

    uint32_t type;                              // 메모리 영역의 종류를 나타내는 변수
    uintptr_t physical_start;                   // 메모리 영역의 시작 주소
    uintptr_t virtual_start;                    // 메모리 영역의 가상 주소
    uint64_t number_of_pages;                   // 메모리 영역의 페이지 수 (주로 4KB, 8KB, 4MB)
    uint64_t attribute;                         // 메모리 영역의 속성 -> 읽기, 쓰기 여부, 실행 가능 여부 정의

};

#ifdef __cplusplus                              /* Main.c(bootloader)에서 접근할 경우 비활성화 */
enum class MemoryType {                         // UEFI에서 정의된 메모리 타입을 나타내는 열거형
    
    kEfiReservedMemoryType,                     // 예약된 메모리 영역
    kEfiLoaderCode,                             // 부트로더 코드가 로드되는 메모리 영역
    kEfiLoaderData,                             // 부트로더 데이터가 로드되는 메모리 영역   
    kEfiBootServicesCode,                       // 부트 서비스의 코드가 위치하는 메모리 영역
    kEfiBootServicesData,                       // 부트 서비스의 데이터가 위치하는 메모리 영역
    kEfiRuntimeServicesCode,                    // 런타임 서비스의 코드가 위치하는 메모리 영역
    kEfiRuntimeServicesData,                    // 런타임 서비스의 데이터가 위치하는 메모리 영역 
    kEfiConventionalMemory,                     // 일반적으로 사용 가능한 메모리 영역
    kEfiUnusableMemory,                         // 사용할 수 없는 메모리 영역
    kEfiACPIReclaimMemory,                      // ACPI(Advanced Configuration and Power Interface)에서 다시 요구되는 메모리
    kEfiACPIMemoryNVS,                          // ACPI Non-Volatile Storage(NVS) 메모리 영역
    kEfiMemoryMappedIO,                         // 메모리에 매핑된 입출력(I/O) 영역
    kEfiMemoryMappedIOPortSpace,                // 메모리에 매핑된 I/O 포트 공간
    kEfiPalCode,                                // 플랫폼 추상 레이어(PAL) 코드가 위치하는 메모리 영역
    kEfiPersistentMemory,                       // 영구 저장소(하드 디스크, SSD 등)로 사용되는 메모리 영역
    kEfiMaxMemoryType                           // 이 열거형의 끝

};



// MemoryType(rhs) 열거형과 lhs 사이의 벼교 연산을 수행하는 연산자.
inline bool operator == (uint32_t lhs, MemoryType rhs) {
    return lhs == static_cast<uint32_t>(rhs);
}

/**
 * @brief MemoryType(rhs) 열거형과 lhs 사이의 벼교 연산을 수행하는 연산자.
 * 
 * 이 연산자는 rhs와 lhs의 값이 같은지를 비교한다. MemoryType열거형이 내부적으로는 
 * uint32_t로 표현되기 때문에 rhs를 uint32_t값으로 변환하여 비교하게 된다.
 * 
*/



// MemoryType(lhs) 열거형과 rhs 사이의 벼교 연산을 수행하는 연산자.
inline bool operator == (MemoryType lhs, uint32_t rhs) {
    return rhs == lhs;
}

/**
 * @brief MemoryType(lhs) 열거형과 rhs 사이의 벼교 연산을 수행하는 연산자.
 * 
 * 위의 연산자와 기능 동일
 * 
*/



inline bool IsAvailable(MemoryType memory_type) {
    return 
        memory_type == MemoryType::kEfiBootServicesCode     ||
        memory_type == MemoryType::kEfiBootServicesData     ||
        memory_type == MemoryType::kEfiConventionalMemory    ;
}
/**
 * @brief OS가 자율적으로 사용할 수 있는 메모리 영역을 구분하는 함수이다.
 * 
 * 이 코드는 커널이 메모리 할당 요청을 받고 메모리를 제공할 때 
 * 이 메모리가 빈 영역인지 아닌지를 판단한다. 만약 요청받은 메모리가 빈 메모리라면
 * 자유롭게 사용할 수 있지만, 이미 사용중인 메모리라면 접근해선 안 된다.
 * 
 * 동작방식:
 *  이 코드는 memory_type인자로 메모리 타입을 받고, 아래의 3가지 조건의 만족에 따라 
 *  참/거짓을 반환한다.
 * 
 *  memory_type == MemoryType::kEfiBootServicesCode     --> 부트 서비스 코드를 위한 메모리 영역
 *  memory_type == MemoryType::kEfiBootServicesData     --> 부트 서비스 데이터를 위한 메모리 영역
 *  memory_type == MemoryType::kEfiConventionalMemory   --> 일반적으로 사용 가능한 메모리(RAM) 영역
 * 
 *  이 3가지 중 하나라도 true라면 IsAvailable은 true를 반환한다.
 * 
*/

const int kUEFIPageSize = 4096;                 /*  페이지 크기(4KB)  */
#endif