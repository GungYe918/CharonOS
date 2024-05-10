/**
 * @file graphics.cpp
*/


#include "graphics.hpp"




// Color black
PixelColor Black = {0, 0, 0};

// Color white
PixelColor White = {255, 255, 255};

// Color grey
PixelColor Grey = {80, 80, 80};

// Color red
PixelColor Red = {255, 0, 0};

// Color orange
PixelColor Orange = {255, 165, 0};

// Color yellow
PixelColor Yellow = {255, 255, 0};

// Color green
PixelColor Green = {0, 255, 0};

// Color cyan
PixelColor Cyan = {0, 255, 255};

// Color blue
PixelColor Blue = {0, 0, 255};

// Color purple
PixelColor Purple = {128, 0, 128};

/*---------- v0.0.4 UPDATE ----------*/ 

// Color Magenta
PixelColor Magenta = {255, 0, 255};

// Color Lavender
PixelColor Lavender = {230, 230, 230};

// Color Coral
PixelColor Coral = {255, 127, 80};

// Color Navy
PixelColor Navy = {0, 0, 128};

// Color Olive
PixelColor Olive = {128, 128, 0};



void RGBResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) 
{
    auto p = PixelAt(x, y); 		// 1)
    p[0] = c.r;   				// 2)
    p[1] = c.g; 				// 3)
    p[2] = c.b; 				// 4)
}

/* 
 * 이 함수는 graphics.hpp에 정의된 RGBResv8BitPerColorPixelWriter 클래스의 가상함수인 Write()의 구현 코드이다.
 *
 * 동작방식: 
 * 	1) auto(uint8_t) 자료형으로 선언된 p변수에 PixelAt()함수를 이용하여 얻은 특정 픽셀의 메모리 위치값을 할당한다.
 * 	2) p의 첫 번째 요소의 RGB값 중 R값을 할당한다.
 * 	3) p의 첫 번째 요소의 RGB값 중 G값을 할당한다.
 * 	4) p의 첫 번째 요소의 RGB값 중 B값을 할당한다.
 *
 * */


void BGRResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) 
{
    auto p = PixelAt(x, y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
}

/*
 * 위의 함수와 동일하게 작동한다.
 * 
 */

void paintRectangle(
    PixelWriter& writer, const Vector2D<int>& pos,
    const Vector2D<int>& size, const PixelColor& c
) {
    for (int dx = 0; dx < size.x; ++dx) {
        writer.Write(pos.x + dx, pos.y, c);
        writer.Write(pos.x + dx, pos.y + size.y - 1, c);
    }
    for (int dy = 1; dy < size.y - 1; ++dy) {
        writer.Write(pos.x, pos.y + dy, c);
        writer.Write(pos.x + size.x - 1, pos.y + dy, c);
    }
}


void paintCircle(
    PixelWriter& writer, const Vector2D<int>& center,
    int radius, const PixelColor& c
) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        // 현재 위치와 대칭된 위치에 대해 원을 그린다.
        writer.Write(center.x + x, center.y - y, c);
        writer.Write(center.x + y, center.y - x, c);
        writer.Write(center.x - y, center.y - x, c);
        writer.Write(center.x - x, center.y - y, c);
        writer.Write(center.x - x, center.y + y, c);
        writer.Write(center.x - y, center.y + x, c);
        writer.Write(center.x + y, center.y + x, c);
        writer.Write(center.x + x, center.y + y, c);

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}



void fillRectangle(
    PixelWriter& writer, const Vector2D<int>& pos,
    const Vector2D<int>& size, const PixelColor& c
) {
    for (int dy = 0; dy < size.y; ++dy) {
        for (int dx = 0; dx < size.x; ++dx) {
            writer.Write(pos.x + dx, pos.y + dy, c);
        }
    }
}

void fillCircle(
    PixelWriter& writer, const Vector2D<int>& center,
    int radius, const PixelColor& c
) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x*x + y*y <= radius*radius) {
                writer.Write(center.x + x, center.y + y, c);
            }
        }
    }
}

void paintEllipse(
    PixelWriter& writer, const Vector2D<int>& center,
    int radiusX, int radiusY, const PixelColor& c
) {

    int x = 0;
    int y = radiusY;
    int err = 0;

    while (radiusY * radiusY * x <= radiusX * radiusX * y) {

        // 현재 위치와 대칭된 위치에 대해 타원을 그린다.
        writer.Write(center.x + x, center.y - y, c);
        writer.Write(center.x - x, center.y - y, c);
        writer.Write(center.x - x, center.y + y, c);
        writer.Write(center.x + x, center.y + y, c);

	x += 1;
	if(err <= 0) {
	    err += 2 * radiusY * radiusY * x + radiusY * radiusY;
	}

	if (err > 0) {
            y -= 1;
            err -= 2 * radiusX * radiusX * y + radiusX * radiusX;
        }
    }

    err = 0;
    x = radiusX;
    y = 0;

    // 같은 코드를 X와 Y값을 바꾸어 한번 더 실행

    while(radiusX * radiusX * y <= radiusY * radiusY * x) {
        
	//현재 위치와 대칭된 위치에 대해 타원을 그린다.
	writer.Write(center.x + x, center.y - y, c);
        writer.Write(center.x - x, center.y - y, c);
        writer.Write(center.x - x, center.y + y, c);
        writer.Write(center.x + x, center.y + y, c);

        y += 1;
        if (err <= 0) {
            err += 2 * radiusX * radiusX * y + radiusX * radiusX;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * radiusY * radiusY * x + radiusY * radiusY;
        }
    
    }
}