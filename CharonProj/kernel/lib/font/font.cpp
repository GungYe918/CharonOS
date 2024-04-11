/**
 * @file font.cpp
*/

#include "font.hpp"

extern const uint8_t _binary_kernelFont_bin_start;  						// kernelFont 바이너리의 start 포인트를 불러온다.
extern const uint8_t _binary_kernelFont_bin_end; 						// kernelFont 바이너리의 end   포인트를 불러온다.
extern const uint8_t _binary_kernelFont_bin_size; 						// kernelFont 바이너리의 크기를 불러온다.

/*
 * python으로 생성한 kernelFont 바이너리를 외부 변수로 불러오는 코드이다.
 * */






//fetch font to kernelFont.o
const uint8_t* FetchFont(char f) {
    auto index = 16 * static_cast<unsigned int>(f); 				// 1)
    if (index >= reinterpret_cast<uintptr_t>(&_binary_kernelFont_bin_size)) 	 		// 2)
    { 
        return nullptr; 									// 3)
    }
    return &_binary_kernelFont_bin_start + index; 						// 4)
}

/*
 * kernelFont 바이너리에서 폰트를 불러오는 함수이다. 입력된 문자가 유효한 범위 내에 있으면 
 * 해당 문자에 해당하는 폰트 데이터의 시작 위치를 반환하고, 
 * 그렇지 않으면 nullptr을 반환한다.
 *
 * 동작방식:
 * 	1) auto(unsigned int) 자료형 변수인 index에 (unsigned int)로 캐스팅 된 문자 f와 16을 곱한 값을 할당한다.
 *
 * 	2) 계산한 index의 값이 폰트 데이터의 크기보다 크거나 같은지를 확인한다. 
 *
 * 	3) 만약 if문이 성립한다면(폰트 데이터의 범위를 벗어난다면) nullptr을 반환한 후 함수를 종료한다.
 *
 * 	4) index값이 유효한 경우(if문이 거짓인 경우) _binary_kernelFont_bin_start 값에 계산된 index값을 더한 값을 반환한다.
 *
 * PLUS:
 * 	1)에서 문자 f에 16을 곱한 이유:
 * 		각 문자가 폰트 데이터에서 차지하는 공간의 크기가 16바이트 이므로 입력한 문자에 대한 인덱스를
 * 		구하기 위해서는 문자 f에 16을 곱하여 index의 값을 정해야 한다.
 *
 * */








//write ascii letter
void WriteAscii(PixelWriter& writer, int x, int y, char f, const PixelColor& color) {
    const uint8_t* font = FetchFont(f); 							// 1)
    if (font == nullptr) { 									// 2)
        return;
    }

    for (int dy = 0; dy < 16; ++dy) { 								// 3)
        for (int dx = 0; dx < 8; ++dx) {  							// 4)
            if ((font[dy] << dx) & 0x80u) { 							// 5)
            writer.Write(x + dx, y + dy, color); 					// 6)
          }
      }
    }
}

/*
 * 이 함수는 font.hpp에 정의된 WriteAscii() 함수를 구현한 것이다. 
 * 이 함수는 특정 위치 (x, y)에 주어진 문자 f를 특정 색상 color로 쓰는 기능을 하는 함수이다.
 *
 * 동작방식:
 * 	1) FetchFont함수를 사용해서 문자 f에 해당하는 폰트 데이터를 가져온다.
 *
 * 	2) 만약 font변수의 값이 nullptr이라면(FetchFont에서 폰트 데이터의 범위를 벗어난 경우) 함수를 종료한다.
 *
 * 	3) 높이에 대한 반복문을 시작한다. 폰트는 16x8 크기의 픽셀 배열로 표시되므로 높이에 대한 루프의 최대는 16으로 제한된다.
 *
 * 	4) 너비에 대한 반복문을 시작한다. 폰트의 너비는 8픽셀이므로 너비에 대한 루프의 최대는 8로 제한된다.
 *
 * 	5) 현재의 폰트 데이터를 가져와서 'dx'만큼 비트를 이동하고, 비트의 값이 1이라면 해당 위치에 writer.Write 함수가 실행된다.
 *
 * 	6) PixelWriter 클래스의 Write함수를 이용해서 주어진 위치(x + dx, y + dy)에 픽셀을 그린다.
 *
 *
 * PLUS:
 * 	과정 5)에 대한 심화설명:
 * 		1. font[dy]는 현재 행에 대한 바이트를 가지고 있다.
 * 		2. (font[dy] << dx) -> 이 코드에서 font[dy]의 값을 왼쪽으로 'dx'만큼 비트쉬프트 연산을 수행한다.
 * 		3. & 0x80u -> 이 코드에서 비트 단위 AND연산을 수행한다. 이 연산은 결과적으로 (font[dy] << dx)의 가장 왼쪽 비트가 1인지 확인한다.
 * 		4. 만약 결과가 참이라면(가장 왼쪽 비트가 1이라면) if문 아래의 코드가 동작한다. 
 * */






void print_str(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color) 
{
    for (int i=0; s[i] != '\0'; i++) { 								// 1)
        WriteAscii(writer, x+8*i, y, s[i], color); 					// 2)
    }
}

/* 
 * 이 함수는 위에서 정의한 WriteAscii()함수를 사용해서 화면에 문자열을 출력하는 함수이다.
 * WriteAscii()함수가 문자(아스키 코드) 1개만을 출력할 수 있다면, 이 함수를 통해 "Hello, world"와 같은 string을 출력할 수 있다.
 *
 *
 * 동작방식:
 * 	1) 포인터 's'가 지시하는 문자열의 범위 내에서 순환하는 for문이다.
 *
 * 	2) WriteAscii()함수를 호출하여 각 문자에 다한 출력을 실행한다. 이때 x값은 폰트의 너비인 8과 문자열의 번호를 나타내는 값인 i를 곱해
 * 		x에 더한다. 따라서 각 문자들이 중복되지 않고 매끄럽게 이어지며 출력될 수 있다.
 * 	
 *
 * */

