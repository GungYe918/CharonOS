/**
 * @file main.cpp
*/

//include - system
    #include <cstdint>
    #include <cstddef>
    #include <cstdio>

    #include <numeric>
    #include <vector>
    #include <cstdarg>


//include - local
    //font
    #include "lib/font/font.hpp"

    //graphics
    #include "lib/graphics/graphics.hpp"

    //frame buffer
    #include "lib/framebuffer/frame_buffer_config.hpp"

    //terminal
    #include "lib/terminal/terminal.hpp"

    //pci
    #include "lib/io/pci.hpp"




/**
 * DEFINES
*/


//define - operator
void operator delete(void* obj) noexcept {

}

//define desktop color
const PixelColor DesktopBGColor{45, 118, 237};
const PixelColor DesktopFGColor{255, 255, 255};


//define cursor shape
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


//define pixel-write buffer
char pixel_write_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;
/*  픽셀 작성자에 사용되는 버퍼의 크기와 픽셀 작성자 변수를 설정한다.  */



//define Terminal-buf
char terminal_buf[sizeof(Terminal)];
Terminal* terminal;
/*  터미널에 사용될 버퍼와 터미널 변수를 설졍한다.  */



//define printk (print string in terminal)
int printk(const char* format, ...) {
    va_list ap;
    int result;
    char s[2048];

    va_start(ap, format);
    result = vsprintf(s, format, ap);
    va_end(ap);

    terminal->printString(s);
    return result;
}

/*
 * 터미널에서 숫자와 문자 출력 등을 지원하는 printk 함수이다. 
 * %d등의 형식 지정자를 사용하여 사칙연산을 지원한다.
 *
 * 문자열을 저장할 버퍼는 2038크기로 되어 있으며, 
 * c++의 vsprintf()함수를 사용하여 형식화된 문자열을 printString()함수로 
 * 출력 가능한 형태로 변형한 뒤 터미널에 출력을 시작한다.
 *
 *
 * */




/**
 * KernelMain 시작
*/




// KernelMain
extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) 
{
    switch (frame_buffer_config.pixel_format) { 				// 1)
        case kPixelRGBResv8BitPerColor: 					// 2)
            pixel_writer = new(pixel_write_buf)
            RGBResv8BitPerColorPixelWriter{frame_buffer_config};
        break;

        case kPixelBGRResv8BitPerColor: 					// 3)
            pixel_writer = new(pixel_write_buf)
            BGRResv8BitPerColorPixelWriter{frame_buffer_config};
        break;
    }

    /*
     * 화면에 픽셀을 쓰는 픽셀 작성자를 설정하는 코드이다. frame_buffer_config 헤더 파일에
     * 정의되어 있는 형식인 RGB, BGR 형식을 따라  그에 맞는 픽셀 작성자를 선택한다.
     *
     * 동작방식:
     *     1) frame_buffer_config 헤더파일에 정의되어 있는 픽셀 포멧에 따라 픽셀 작성자를 선택하는
     *        switch 문이다.
     *     
     *     2) 만약 픽셀 형식이 RGB 형식이라면 RGB 형식을 따르는 픽셀 작성자를 선택한다.
     *
     *     3) 만약 픽셀 형식이 BGR 형식이라면 BGR 형식을 따르는 픽셀 작성자를 선택한다. 
     *
     */



    const int kFrameWidth = frame_buffer_config.horizontal_resolution;
    const int kFrameHeight = frame_buffer_config.vertical_resolution;
    /*  frame_buffer_config 헤더파일에 정의되어 있는 화면 비율 설정을 커널의 프레임 길이로 정의한다.  */



    
    //render desktop envirorment
    fillRectangle(
        *pixel_writer,
        {0, 0},
        {kFrameWidth, kFrameHeight - 50},
        DesktopBGColor
    );
    fillRectangle(
        *pixel_writer,
        {0, kFrameHeight - 50},
        {kFrameWidth, 50},
        Black
    );
    fillRectangle(
        *pixel_writer,
        {0, kFrameHeight - 50},
        {kFrameWidth / 5, 50},
        Grey
    );
    paintCircle(
        *pixel_writer,
        {10 + 30 / 2, kFrameHeight - 40 + 30 / 2},
        15,
        {160, 160, 160}
    );


    //enter terminal envirorment
    terminal = new(terminal_buf) Terminal(
        *pixel_writer, DesktopFGColor, DesktopBGColor
    );
    /*  fg_color, bg_color, writer를 인수로 갖는 터미널 변수를 새로 설정한다.  */




    printk("CharonOS ver 0.0.1\n"); 		/*  현재 커널의 버전을 표시한다.  */



    //render mouse cursor
    for (int dy = 0; dy < cursorHeight; ++dy) {
        for (int dx = 0; dx < cursorWidth; ++dx) {
            if (cursor_shape[dy][dx] == '@') {
                pixel_writer->Write(600+dx, 400+dy, {0, 0, 0});
            } else if (cursor_shape[dy][dx] == '.') {
                pixel_writer->Write(600+dx, 400+dy, {255, 255, 255});
            }
        }
    }

    /*
     * 위에 정의된 cursor_shape로부터 마우스 커서를 렌더링 한다.
     * 2차원인 cursor_shape 배열을 순환하며  문자 '@'는 pixel_writer를 이용해 검은색을 렌더하고
     * 문자 '.'은 흰색으로 렌더한다.
     *
     *
     */


    
    printk("0/O\n");

    auto err = pci::ScanAllBus();
    printk("ScanAllBus: %s\n", err.Name());

    for (int i = 0; i < pci::num_device; ++i) {
        const auto& dev = pci::devices[i];
        auto vendor_id = pci::ReadVendorId(dev.bus, dev.device, dev.function);
        auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
        printk("%d.%d.%d: vend %04x, class %08x, head %02x\n",
                dev.bus, dev.device, dev.function,
                vendor_id, class_code, dev.header_type);
    }

    /*
     * 현재 사용가능한 PCI와 BUS를 검색하고 id와 code, function를 화면에 출력한다.
     */

    while (1) {
        __asm__("hlt");
    }
    /*  임시: 커널의 초기설정이 끝나면 어셈블리어를 사용해서 커널을 정지시킨다.  */

}
