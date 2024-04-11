#pragma once

#include <cstdint>
#include "../graphics/graphics.hpp"

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color); 		// 터미널에 아스키 코드 1개를 작성하는 함수이다. 
void print_str(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color); 	// 터미널에 문자열을 작성하는 함수이다.
