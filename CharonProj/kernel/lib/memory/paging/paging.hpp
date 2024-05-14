/**
 * @file paging.hpp
 * 
 * paging을 정의하는 코드
*/

#pragma once

#include <cstddef>


// PageDirectoryCount(64)
const size_t kPageDirectoryCount = 64;


// IdentityPageTable 설정 함수
void SetupIdentityPageTable();