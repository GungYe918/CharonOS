/**
 * @file libcxx_support.cpp
 * 
 * C++표준 라이브러리에서 제공되는 몇 기능들에 대한 특수 정의
*/

#include <new>
#include <cerrno>


//동적 할당 실패 시 기본 핸들러 호출
std::new_handler std::get_new_handler() noexcept {
    return nullptr;
}

//메모리 정렬 & 할당 등에 실패할 시 'ENOMEM'을 반환
extern "C" int posix_memalign(void**, size_t, size_t) {
    return ENOMEM;
}