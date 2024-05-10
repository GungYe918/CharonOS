#pragma once

#include "../framebuffer/frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};

//define color
extern PixelColor Black;
extern PixelColor White;
extern PixelColor Grey;
extern PixelColor Red;
extern PixelColor Orange;
extern PixelColor Yellow;
extern PixelColor Green;
extern PixelColor Cyan;
extern PixelColor Blue;
extern PixelColor Purple;


/*---------- v0.0.4 UPDATE ----------*/ 
extern PixelColor Magenta;
extern PixelColor Lavender;
extern PixelColor Coral;
extern PixelColor Navy;
extern PixelColor Olive;

/*
 * 이 변수들은 특정 색을 나타내는 RGB값을 미리 정의한 것이다.
 * */


class PixelWriter { 	
    public:
        PixelWriter(const FrameBufferConfig& config) : config_{config} {
      }
    virtual ~PixelWriter() = default;
    virtual void Write(int x, int y, const PixelColor& c) = 0;

    protected:
        uint8_t* PixelAt(int x, int y) {
        return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

    private:
        const FrameBufferConfig& config_;
};

/*
 * PixelWriter:
 * 	이 클래스는 FrameBufferConfig를 사용하여 화면에 픽셀을 쓰는 기능을 가진 함수들을
 * 	포괄하는 클래스이다. 각 변수들은 virtual형태로 구현되어 있으며, 아래의 코드에서 정의된다.
 *
 * Write():
 * 	이 함수는 (x, y)형태로 주어진 좌표에 특정 색의 픽셀을 쓰는 기능을 가진 함수이다.
 *
 * PixelAt():
 * 	이 함수는 주어진 좌표(x, y)에 대한 픽셀의 포인터를 반환한다. 따라서 이 함수를 통해 
 * 	디스플레이 버퍼에 접근하여 픽셀을 쓸 수 있다. 
 *
 * config_:
 * 	이 멤버 변수는 const로 선언된 FrameBufferConfig(frame_buffer_config.hpp 헤더에 정의)의 참조 변수로
 * 	PixelWriter에 FrameBuffer과 관련된 정보를 전달한다. 
 *
 *
 * */




class RGBResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor& c) override;
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor& c) override;
};


/*
 * 위의 두 클래스는 각각 PixelWriter를 상속한 클래스로, 첫 번째 클래스는 
 * RGB형식으로 정의된 픽셀에 대한 쓰기 작업을 수행하는 클래스이며, 두 번째 클래스는 
 * BGR형식으로 정의된 필셀에 대한 쓰기 작업을 수행하는 클래스이다. 
 *
 * 왜 RGB형식과 BGR형식에 관해서 따로 클래스를 정의하는지에 대한 이유는 
 * frame_buffer_config.hpp의 frame_buffer관련 주석에 설명되어 있다.
 *
 * */



//2D vector struct 
template <typename V>
struct Vector2D {
    V x, y;

    //Vector의 덧셈에 대한 정리
    template <typename U>
    Vector2D<V>& operator += (const Vector2D<U>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    /**
     * EX)
     * 
     * Vector2D<int> vec1 {1, 2};
     * Vectir2D<float> vec2 {2.5, 3.5};
     * vec1 += Vec2;
     * 
     * 
     * -------------RESULT-------------
     * vec1 == {3, 5}
    */
};

//paint func
void paintRectangle(
    PixelWriter& writer, const Vector2D<int>& pos,
    const Vector2D<int>& size, const PixelColor& c
);

void fillRectangle(
    PixelWriter& writer, const Vector2D<int>& pos,
    const Vector2D<int>& size, const PixelColor& c
);

void paintCircle(
    PixelWriter& writer, const Vector2D<int>& center,
    int radius, const PixelColor& c
);

void fillCircle(
    PixelWriter& writer, const Vector2D<int>& center,
    int radius, const PixelColor& c
);

void paintEllipse(
    PixelWriter& writer, const Vector2D<int>& center,
    int radiusX, int radiusY, const PixelColor& c
);


/*
 * 디스플레이에 표시할 도형들을 쓰는 함수들이다.
 *
 * 1. paintRectangle():
 * 	시작 위치를 의미하는 pos로부터 PixelWriter를 이용해서 직사각형을 출력하는 함수이다.
 *
 * 2. fillRectangle():
 * 	시작 위치를 의미하는 pos로부터 PixelWriter를 이용해서 직사각형을 그리고 내부를 특정 색의 픽셀로 채우는 함수이다.
 * 
 * 3. paintCircle():
 * 	위의 paintRectangle() 함수와 동일하게 기능하며, 특정 색의 픽셀로 원을 그린다.
 * 	이 함수는 브레젠험 알고리즘을 응용한 것으로 각 픽셀에 대한 직선의 근사치를 계산하여 가장 가까운 점을 그린다.
 * 
 * 4. fillCircle():
 * 	위의 fillRectangle() 함수와 동일하게 기능하며, 특정 색의 픽셀로 원 안을 채운다. 
 *
 * 5. paintEllipse();
 * 	구현 예정, 화면에 타원을 출력하는 함수이다.
 *
 * */
