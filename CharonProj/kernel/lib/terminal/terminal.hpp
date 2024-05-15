#pragma once

#include "../graphics/graphics.hpp"

class Terminal {
    public:
        static const int tHorizontal = 25; 			 	                // 터미널의 가로 크기를 정의한다. 
        static const int tVertical = 80; 			 	                // 터미널의 세로 크기를 정의한다. 

        Terminal( 							                            // forground_color, background_color를 인자로 받고
            PixelWriter& writer, const PixelColor& fg_color,  	 	    // 터미널 영역의 픽셀을 초기화하는 생성자이다.
            const PixelColor& bg_color
        );
        void printString(const char* s); 				                // 터미널 안에서 한 라인에 맞추어 문자를 쓴다.

    private: 
        void newLine(); 						                        // 줄 바꿈을 구현하는 함수이다.

        PixelWriter& writer_; 						                    // pixelwriter 객체를 참조한다.
        const PixelColor fg_color_, bg_color_; 				            // PixelColor 형식으로 fg_color, bg_color를 정의한다.
        char buffer_[tHorizontal][tVertical + 1]; 			            // 터미널에 출력할 문자열을 저장하는 2차원 버퍼를 정의한다.

        int cursor_row_, cursor_column_; 				                // 현재 커서의 위치를 나타내는 변수를 정의한다.
};
