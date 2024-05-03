#include "mouse.hpp"
#include "../graphics/graphics.hpp"

namespace {
    const int cursorWidth = 15;
    const int cursorHeight = 24;
    const char cursor_shape[cursorHeight][cursorWidth + 1] = {
        "@              ",
        "@@             ",
        "@.@            ",
        "@..@           ",
        "@...@          ",
        "@....@         ",
        "@.....@        ",
        "@......@       ",
        "@.......@      ",
        "@........@     ",
        "@.........@    ",
        "@..........@   ",
        "@...........@  ",
        "@............@ ",
        "@......@@@@@@@@",
        "@......@       ",
        "@....@@.@      ",
        "@...@ @.@      ",
        "@..@   @.@     ",
        "@.@    @.@     ",
        "@@      @.@    ",
        "@       @.@    ",
        "         @.@   ",
        "         @@@   ",
    };

    //render mouse cursor
    void DrawCursor(PixelWriter* pixel_writer, Vector2D<int> position) { 
        for (int dy = 0; dy < cursorHeight; ++dy) {
            for (int dx = 0; dx < cursorWidth; ++dx) {
                if (cursor_shape[dy][dx] == '@') {
                    pixel_writer->Write(position.x + dx, position.y + dy, {0, 0, 0});
                } else if (cursor_shape[dy][dx] == '.') {
                    pixel_writer->Write(position.x + dx, position.y + dy, {255, 255, 255});
                }
            }
        }
    }

    void EraseCursor(
        PixelWriter* pixel_writer, Vector2D<int> position,
        PixelColor erase_color
    ) {
        for (int dy = 0; dy < cursorHeight; ++dy) {
            for (int dx = 0; dx < cursorWidth; ++dx) {
                if (cursor_shape[dy][dx] != ' ') {
                    pixel_writer->Write(position.x + dx, position.y + dy, erase_color);
                }
            }
        }
    }
}


//define mouse_class
MouseCursor::MouseCursor(
    PixelWriter* writer, PixelColor erase_color,
        Vector2D<int> initial_position
) : pixel_writer_{writer},
    erase_color_{erase_color},
    position_{initial_position}
{
    DrawCursor(pixel_writer_, position_);
}

void MouseCursor::MoveRelative(Vector2D<int> displacement) {
    //1. 이전 위치의 픽셀 초기화
    EraseCursor(pixel_writer_, position_, erase_color_);

    //2. 새 위치로 이동
    position_ += displacement;

    //3. 새 위치 좌표에 커서 생성
    DrawCursor(pixel_writer_, position_);
}

