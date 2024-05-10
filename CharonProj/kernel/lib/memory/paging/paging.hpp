/**
 * @file paging.hpp
 * 
 * paging을 정의하는 코드
*/

#pragma once

#include <cstddef>


const size_t kPageDirectoryCount = 64;

void SetupIdentityPageTable();