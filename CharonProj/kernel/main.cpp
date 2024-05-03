/**
 * @file main.cpp
*/

// include - system
    #include <cstdint>
    #include <cstddef>
    #include <cstdio>

    #include <numeric>
    #include <vector>


// include - local
// Kernel
    // font
    #include "lib/font/font.hpp"

    // graphics
    #include "lib/graphics/graphics.hpp"

    // frame buffer
    #include "lib/framebuffer/frame_buffer_config.hpp"

    // terminal
    #include "lib/terminal/terminal.hpp"

    // pci
    #include "lib/io/pci.hpp"

    // mouse
    #include "lib/mouse/mouse.hpp"

    // log
    #include "lib/log/logger.hpp"


// USB driver
    #include "usb/memory.hpp"
    #include "usb/device.hpp"
    #include "usb/classdriver/mouse.hpp"
    #include "usb/xhci/xhci.hpp"
    #include "usb/xhci/trb.hpp"





/**
 * DEFINES
*/

//define desktop color
const PixelColor DesktopBGColor{45, 118, 237};
const PixelColor DesktopFGColor{255, 255, 255};


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



char mouse_cursor_buf[sizeof(MouseCursor)];
MouseCursor* mouse_cursor;

void MouseObserver(int8_t displacement_x, int8_t displacement_y) {
    mouse_cursor->MoveRelative({displacement_x, displacement_y});
}

void SwichEhci2Xhci(const pci::Device& xhc_dev) {
    bool intel_ehc_exist = false;

    for (int i = 0; i < pci::num_device; ++i) {
        if (pci::devices[i].class_code.Match(0xcu, 0x03u, 0x20u) 
        && 0x8086 == pci::ReadVendorId(pci::devices[i])) {
            intel_ehc_exist = true;
            break;
        }
    }

    if (!intel_ehc_exist) {
        return;
    }

    uint32_t superspeed_ports = pci::ReadConfigReg(xhc_dev, 0xdc);
    pci::WriteConfigReg(xhc_dev, 0xd8, superspeed_ports);
    uint32_t ehci2xhci_ports = pci::ReadConfigReg(xhc_dev, 0xd4);
    pci::WriteConfigReg(xhc_dev, 0xd0, ehci2xhci_ports);

    Log(kDebug, "SwichEhciXhci: SS = %02, xHCI = %02x\n", superspeed_ports, ehci2xhci_ports);
}







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
    paintEllipse(
        *pixel_writer,
        {60, kFrameHeight - 50},
        15, 30,
        Red
    );
    paintEllipse(
        *pixel_writer,
        {100, kFrameHeight - 50},
        15, 30,
        Blue
    );
    paintEllipse(
        *pixel_writer,
        {60, kFrameHeight - 50},
        60, 30,
        Green
    );
    paintCircle(
        *pixel_writer,
        {kFrameWidth-500, kFrameHeight-500},
        15,
        Green
    );
    paintCircle(
        *pixel_writer,
        {kFrameWidth-800, kFrameHeight-500},
        15,
        Blue
    );



    //enter terminal envirorment
    terminal = new(terminal_buf) Terminal(
        *pixel_writer, DesktopFGColor, DesktopBGColor
    );
    /*  fg_color, bg_color, writer를 인수로 갖는 터미널 변수를 새로 설정한다.  */




    printk("CharonOS ver 0.0.4\n"); 		/*  현재 커널의 버전을 표시한다.  */
    printk("0/O\n");
    printk("1 + 9 = %d\n", 10);
    printk("\n\nGungYE is the greatest King of the world!!");



    SetLogLevel(kWarn);



    //render mouse cursor
    mouse_cursor = new(mouse_cursor_buf) MouseCursor {
        pixel_writer, DesktopBGColor, {300, 200}
    };

    auto err = pci::ScanAllBus();
    Log(kDebug, "ScanAllBus: %s\n", err.Name());

    for (int i = 0; i < pci::num_device; ++i) {
        const auto& dev = pci::devices[i];
        auto vendor_id = pci::ReadVendorId(dev);
        auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);

        Log(kDebug, "%d.%d.%d: vend %04x, class %08x, head %02x\n",
                dev.bus, dev.device, dev.function,
                vendor_id, class_code, dev.header_type);
    }

    // find xhc
    pci::Device* xhc_dev = nullptr;
    for (int i = 0; i < pci::num_device; ++i) {
        if (pci::devices[i].class_code.Match(0x0cu, 0x03u, 0x30u)) {
            xhc_dev = &pci::devices[i];

        if (0x8086 == pci::ReadVendorId(*xhc_dev)) 
            break;
        }
    }

    if (xhc_dev) {
        Log(kInfo, "xHC has been found: %d.%d.%d\n",
                xhc_dev->bus, xhc_dev->device, xhc_dev->function);
    }

    

    const ValueWithError<uint64_t> xhc_bar = pci::ReadBar(*xhc_dev, 0);
    Log(kDebug, "ReadBar: %s\n", xhc_bar.error.Name());
    const uint64_t xhc_mmio_base = xhc_bar.value & ~static_cast<uint64_t>(0xf);
    Log(kDebug, "xHC mmio_base = %08lx\n", xhc_mmio_base);


    /*  init xhc  */
    usb::xhci::Controller xhc{xhc_mmio_base};


    if (0x8086 == pci::ReadVendorId(*xhc_dev)) {
        SwichEhci2Xhci(*xhc_dev);
    }
    {
        auto err = xhc.Initialize();
        Log(kDebug, "xhc.Initialize: %s\n", err.Name());
    }


    Log(kInfo, "xHC starting\n");
    xhc.Run();


    /*  configure port  */
    usb::HIDMouseDriver::default_observer = MouseObserver;

    for (int i = 1; i <= xhc.MaxPorts(); ++i) {
        auto port = xhc.PortAt(i);
        Log(kDebug, "Port %d: IsConnected=%d\n", i, port.IsConnected());


        if (port.IsConnected()) {
            if (auto err = ConfigurePort(xhc, port)) {
                Log(kError, "failed to configure port: %s at %s, Line%d\n",
                        err.Name(), err.File(), err.Line());
                continue;   
            }
        }
    }

    
    while(1) {
        if (auto err = ProcessEvent(xhc)) {
            Log(kError, "Error while ProcessEvent: %s at %s, Line:%d\n",
                    err.Name(), err.File(), err.Line());
        }
    }


    while (1) {
        __asm__("hlt");
    }
    /*  임시: 커널의 초기설정이 끝나면 어셈블리어를 사용해서 커널을 정지시킨다.  */

}

extern "C" void __cxa_pure_virtual() {
    while(1) {
        __asm__("hlt");
    }
}