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
    #include "lib/pci/pci.hpp"

    // mouse
    #include "lib/mouse/mouse.hpp"

    // log
    #include "lib/log/logger.hpp"

    // interrupt 
    #include "lib/interrupt/interrupt.hpp"
    #include "lib/interrupt/interrupt_asm.h"

    // queue
    #include "lib/queue/queue.hpp"

    // memory map
    #include "lib/memory/memory_map.hpp"

    // segments
    #include "lib/memory/segment/segment.hpp"

    // GDT
    #include "lib/memory/GDT/gdt.h"

    // paging
    #include "lib/memory/paging/paging.hpp"
    #include "lib/memory/paging/paging_asm.h"

    // memory manager
    #include "lib/memory/MMR/memory_manager.hpp"

    // window compositor
    #include "lib/compositor/window/window.hpp"





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


char memory_manager_buf[sizeof(BitmapMemoryManager)];
BitmapMemoryManager* memory_manager;


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



/*  xhci handler  */
usb::xhci::Controller* xhc;

struct Message {
    enum Type {
        kInterruptXHCI,
    } type;
};

kQueue<Message>* main_queue;

__attribute__((interrupt))
void IntHandlerXHCI(InterruptFrame* frame) {
    main_queue->Push(Message{Message::kInterruptXHCI});

    NotifyEndOfInterrupt();
}






/**
 * KernelMain 시작
*/



// KernelMain
alignas(16) uint8_t kernel_main_stack[1024 * 1024];                 
extern "C" void KernelMainNewStack(
    const FrameBufferConfig& frame_buffer_config_ref,
    const MemoryMap& memory_map_ref
) {

    /**
     * 원래 진입점인 KernelMain의 스택 영역을 이동시켰다.
     * 관련 코드는 memory/new_entry.asm에 작성되어 있다.
     * 
    */

    const FrameBufferConfig& frame_buffer_config{frame_buffer_config_ref};
    const MemoryMap& memory_map{memory_map_ref};

    switch (frame_buffer_config.pixel_format) { 				    // 1)
        case kPixelRGBResv8BitPerColor: 					        // 2)
            pixel_writer = new(pixel_write_buf)
            RGBResv8BitPerColorPixelWriter{frame_buffer_config};
        break;

        case kPixelBGRResv8BitPerColor: 					        // 3)
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
        {200, kFrameHeight - 300},
        60, 30,
        Magenta
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




    printk("CharonOS v0.0.7\n"); 		/*  현재 커널의 버전을 표시한다.  */
    SetLogLevel(kWarn);



    SetupSegments();

    const uint16_t kernel_cs = 1 << 3;
    const uint16_t kernel_ss = 2 << 3;
    SetDSAll(0);
    SetCSSS(kernel_cs, kernel_ss);

    SetupIdentityPageTable();

    //mark allocated 
    ::memory_manager = new(memory_manager_buf) BitmapMemoryManager;

    const auto memory_map_base = reinterpret_cast<uintptr_t>(memory_map.buffer);
    uintptr_t available_end = 0;

    for (
        uintptr_t iter = memory_map_base;
        iter < memory_map_base + memory_map.map_size;
        iter += memory_map.descriptor_size
    ) {
        auto desc = reinterpret_cast<MemoryDescriptor*>(iter);

        if (available_end < desc->physical_start) {
            memory_manager->MarkAllocated(
                FrameID{available_end / kBytesPerFrame},
                (desc->physical_start - available_end) / kBytesPerFrame
            );
        }

        const auto physical_end = 
            desc->physical_start + desc->number_of_pages * kUEFIPageSize;

        if (IsAvailable(static_cast<MemoryType>(desc->type))) {
            available_end = physical_end;
        } else {
            memory_manager->MarkAllocated(
                FrameID{desc->physical_start / kBytesPerFrame},
                desc->number_of_pages * kUEFIPageSize / kBytesPerFrame
            );
        }
    }
    memory_manager->SetMemoryRange(FrameID{1}, FrameID{available_end / kBytesPerFrame});

    if (auto err = InitializeHeap(*memory_manager)) {
        Log(
            kError,
            "failed to allocate pages: %s at %s:%d\n",
            err.Name(), err.File(), err.Line()
        );
        exit(1);
    }


    //render mouse cursor
    mouse_cursor = new(mouse_cursor_buf) MouseCursor {
        pixel_writer, DesktopBGColor, {300, 200}
    };

    std::array<Message, 32> main_queue_data;
    kQueue<Message> main_queue{main_queue_data};
    ::main_queue = &main_queue;

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


    SetIDTEntry(idt[InterruptVector::kXHCI], MakeIDTAttr(DescriptorType::kInterruptGate, 0),
                reinterpret_cast<uint64_t>(IntHandlerXHCI), kernel_cs);

    LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]));


    const uint8_t bsp_local_apic_id = 
        *reinterpret_cast<const uint32_t*>(0xfee00020) >> 24;
    
    pci::ConfigureMSIFixedDestination(
        *xhc_dev, bsp_local_apic_id,
        pci::MSITriggerMode::kLevel, 
        pci::MSIDeliveryMode::kFixed,
        InterruptVector::kXHCI, 0
    );



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

    ::xhc = &xhc;
    __asm__("sti");


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
        __asm__("cli");

        if (main_queue.Count() == 0) {
            __asm__("sti\n\thlt");
            continue;
        }

        Message msg = main_queue.Front();
        main_queue.Pop();
        __asm__("sti");

        switch (msg.type) {
            case Message::kInterruptXHCI:
                while (xhc.PrimaryEventRing() -> HasFront()) {
                    if (auto err = ProcessEvent(xhc)) {
                        Log(kError, "Error while ProcessEvent: %s at %s:%d\n",
                                err.Name(), err.File(), err.Line());
                    }
                }
                break;
            default:
                Log(kError, "Unkown message type: %d\n", msg.type);
        }
    }

}

extern "C" void __cxa_pure_virtual() {
    while(1) {
        __asm__("hlt");
    }
}