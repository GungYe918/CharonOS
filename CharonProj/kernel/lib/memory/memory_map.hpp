#pragma once 

#include <stdint.h>

struct MemoryMap {

    unsigned long long buffer_size;
    void* buffer;
    unsigned long long map_size;
    unsigned long long map_key;
    unsigned long long descriptor_size;
    uint32_t descriptor_version;
    
};

struct MemoryDescriptor {

    uint32_t type;
    uintptr_t physical_start;
    uintptr_t virtual_start;
    uint64_t number_of_pages;
    uint64_t attribute;

};

#ifdef __cplusplus              /* Main.c(bootloader)에서 접근할 경우 비활성화 */
enum class MemoryType {
    kEfiReservedMemoryType,
    kEfiLoaderCode,
    kEfiLoaderData,
    kEfiBootServicesCode,
    kEfiBootServicesData,
    kEfiRuntimeServicesCode,
    kEfiRuntimeServicesData,
    kEfiConventionalMemory,
    kEfiUnusableMemory,
    kEfiACPIReclaimMemory,
    kEfiACPIMemoryNVS,
    kEfiMemoryMappedIO,
    kEfiMemoryMappedIOPortSpace,
    kEfiPalCode,
    kEfiPersistentMemory,
    kEfiMaxMemoryType
};

inline bool operator == (uint32_t lhs, MemoryType rhs) {
    return lhs == static_cast<uint32_t>(rhs);
}

inline bool operator == (MemoryType lhs, uint32_t rhs) {
    return rhs == lhs;
}

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

const int kUEFIPageSize = 4096;
#endif