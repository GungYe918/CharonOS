/**
 * @file mouse.hpp
*/

#pragma once

#include "../graphics/graphics.hpp"

class MouseCursor { 						            // 마우스의 위치와 이동을 정의하는 마우스 클래스이다.
    public:
        MouseCursor(
            PixelWriter* writer, PixelColor erase_color,
            Vector2D<int> initial_position
        );
        void MoveRelative(Vector2D<int> displacement);

	/*
	 * MouseCursor() :
	 * 	PixelWriter(graphics.hpp에 정의), 지울 색상, 초기 위치를 받는 생성자이다.
	 *
	 * MoveRelative() : 
	 * 	현 위치와 이동할 위치를 고려하여 마우스의 이동을 전담하는 함수이다.
	 * */

    private:   
        PixelWriter* pixel_writer_ = nullptr; 			// PixelWriter를 지시하는 포인터이다.
        PixelColor erase_color_; 				        // MouseCursor() 생성자에서 사용되는 erase_color_를 정의한 것이다.
        Vector2D<int> position_; 				        // 마우스 커서의 현재 위치를 나타내는 변수이다.
};
