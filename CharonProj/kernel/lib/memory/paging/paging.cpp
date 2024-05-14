#include "paging.hpp"

#include <array>
#include "paging_asm.h"


namespace {
    const uint64_t kpageSize4K = 4096;                                                  // 4KB 페이지의 크기
    const uint64_t kPageSize2M = 512 * kpageSize4K;                                     // 2MB 페이지의 크기
    const uint64_t kPageSize1G = 512 * kPageSize2M;                                     // 1GB 페이지의 크기

    alignas(kpageSize4K) std::array<uint64_t, 512> pml4_table;                          // 페이지 매핑 레벨(PML4) 테이블을 나타내는 배열, 페이지 테이블의 최상위 레벨 
    alignas(kpageSize4K) std::array<uint64_t, 512> pdp_table;                           // 페이지 디렉토리 포인터(PDP) 테이블을 나타내는 배열
    alignas(kpageSize4K) 
        std::array<std::array<uint64_t, 512>, kPageDirectoryCount> page_directory;      // 페이지 디렉토리를 나타내는 2차원 배열, 페이지 테이블의 중간 레벨
}

/**
 * @brief paging에 필요한 값들을 정의하는 익명 namespace
 * 
 * PLUS:
 *  익명 namespace를 사용하는 이유:
 *      익명 namespace를 사용할 경우 이 namespace안의 값은 
 *      외부에서 참조 혹은 사용이 불가능하게 된다. 주로 익명 namespace안에는 
 *      외부에서 사용할 경우 중복, 함수 충돌 등이 발생할 수 있는 민감한 코드를 
 *      작성하거나, 보안의 강화를 위해 강한 익명성이 요구되는 코드들을 작성하게 된다.
 * 
 * PLUS:
 *  pml4, pdp에 대한 설명:
 *      PML4:
 *          zzzz
 * 
 *      PDP.:
 *          sss
 * 
*/


void SetupIdentityPageTable() {
    pml4_table[0] = reinterpret_cast<uint64_t>(&pdp_table[0]) | 0x003;

    for (int i_pdpt = 0; i_pdpt < page_directory.size(); ++i_pdpt) {
        pdp_table[i_pdpt] = reinterpret_cast<uint64_t>(&page_directory[i_pdpt]) | 0x003;

        for (int i_pd = 0; i_pd < 512; ++i_pd) {
            page_directory[i_pdpt][i_pd] = i_pdpt * kPageSize1G + i_pd * kPageSize2M | 0x083;
        }
    }

    SetCR3(reinterpret_cast<uint64_t>(&pml4_table[0]));
}
