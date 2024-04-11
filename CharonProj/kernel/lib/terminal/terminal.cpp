/**
 * @file terminal.cpp
*/

//include - system
#include <cstring>

//include - local
#include "terminal.hpp"
#include "../font/font.hpp"



//Terminal-class constructor
Terminal::Terminal(
    PixelWriter& writer, const PixelColor& fg_color, 
            const PixelColor& bg_color 
) : writer_{writer}, fg_color_{fg_color}, bg_color_{bg_color},
    buffer_{}, cursor_row_{0}, cursor_column_{0} {

    }

    /*
     * PixelWriter, foreground_color, background_color에 해당하는 인자를 받고
     * 이를 이용해서 writer_, fg_color_, bg_color_와 함께 버퍼를 0으로 
     * 커서 위치를 각각 0으로 초기화하는 Terminal 생성자이다.
     *
     */
    




void Terminal::printString(const char* s) {
    while(*s)
    {
        if(*s == '\n') { 											// 1)
            newLine();
        } else if (cursor_column_ < tVertical - 1) { 								// 2)
            WriteAscii(writer_, 8*cursor_column_, 16*cursor_row_, *s, fg_color_); 	// 3)
            buffer_[cursor_row_][cursor_column_] = *s; 								// 4)
            ++cursor_column_; 											// 5)
        }
        ++s; 													// 6)
    }
}

/*
 * 터미널에 한 줄로 문자열을 작성하는 함수이다.
 * 현재 개행 문자 중 '\n'이 구현되었다.
 *
 * 동작방식:
 * 	1) 만약 문자열에 '\n'이 포함되었다면 newLine()함수를 이용해서 한 줄 밑으로 이동한다.
 * 	
 * 	2) 만약 커서의 열 위치가 터미널의 최대 열 위치의 -1  보다 작다면(터미널 인덱스는 0부터 시작) 아래의 코드가 동작한다.
 *
 * 	3) WriteAscii함수를 이용해서 터미널에 문자를 쓴다. 이때 폰트의 폭인 8픽셀을 cursor_column_에 곱하고, 
 * 	   폰트의 높이인 16픽셀을 cursor_row_에 곱하여 문자를 작성한다.
 *
 * 	4) 현재 위치에 출력된 문자를 버퍼에 저장한다.
 *
 * 	5) 커서의 열 위치를 한칸 다음 위치로 이동시킨다.
 *
 * 	6) 다음 문자로 이동한다.
 *
 */




void Terminal::newLine() {
    cursor_column_ = 0; 											// 1)
    if (cursor_row_ < tHorizontal - 1) { 									// 2)
        ++cursor_row_; 
    } else {
        for (int y = 0; y < 16*tHorizontal; ++y) { 								// 3)
            for (int x = 0; x < 8*tVertical; ++x) { 
                writer_.Write(x, y, bg_color_); 								// 4)
            }
        }
        for (int horizontal = 0; horizontal < tHorizontal - 1; ++horizontal) { 					// 5)
            memcpy(buffer_[horizontal], buffer_[horizontal + 1], tVertical + 1); 			// 6)
            print_str(writer_, 0, 16*horizontal, buffer_[horizontal], fg_color_);  	// 7)
        }
        memset(buffer_[tHorizontal - 1], 0, tVertical + 1); 						// 8)
    }
}

/*
 * 터미널에서 개행문자 '\n'(줄바꿈)을 구현하는 함수이다.
 *
 * 동작방식:
 * 	1) cursor_column_을 0으로 초기화한다.(터미널에서 한 라인의 맨 처음에서 시작하기 위함)	 
 *
 * 	2) cursor_row_의 값이 터미널의 가로길이의 -1보다 작다면, cursor_row_를 증가시킨다.
 *
 * 	3) 터미널의 배경을 초기화하는 과정이다. 폰트의 폭과 높이인 8과 16을 각각 tVertical, tHorizontal에
 * 	   곱한 만큼의 이중 for문을 사용해서 터미널의 배경을 bg_color_로 초기화한다.
 *
 * 	4) 이중 for문이 실행되는 동안 터미널의 색상을 bg_color_로 초기화한다.
 *
 * 	5) horizontal값이 tHorizontal-1의 값에 이를 때 까지를 범위로 하는 for문이다.
 *
 * 	6) 현재 행의 데이터를 다음 행으로 복사한다. -> 현재 버퍼의 모든 행을 한 칸씩 위로 이동시킨다.
 * 
 * 	7) print_str()함수를 사용해서 갱신된 버퍼 안의 데이터를 화면에 출력한다.
 *
 * 	8) memset(메모리 초기화 함수)로 마지막 행의 버퍼를 초기화한다. 
 *
 * PLUS:
 * 	과정 6)에서 버퍼의 모든 행을 한 칸씩 위로 이동시킨 이유 :
 * 		 이는 터미널의 스크롤 기능을 구현하기 위한 것으로, 터미널의 크기는 한정되어 있기에,
 * 		 새로운 줄이 추가되면 화면을 스크롤 하여 새로운 줄을 추가하기 위한 공간을 확보한다.
 */
