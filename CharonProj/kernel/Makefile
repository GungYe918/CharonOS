TARGET = kernel.elf

# DIR of cpp file
OBJS = main.o lib/graphics/graphics.o lib/font/font.o	lib/terminal/terminal.o	lib/io/pci.o	lib/io/io_func.o	lib/font/kernelFont.o	newlib_support.o	\
		lib/log/logger.o	lib/mouse/mouse.o	libcxx_support.o	\
		usb/memory.o	usb/device.o	usb/xhci/ring.o	usb/xhci/trb.o	usb/xhci/xhci.o	\
		usb/xhci/port.o	usb/xhci/device.o	usb/xhci/devmgr.o	usb/xhci/registers.o	\
		usb/classdriver/base.o	usb/classdriver/hid.o	usb/classdriver/keyboard.o	\
		usb/classdriver/mouse.o


DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

CPPFLAGS += -I.

CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone

CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17

LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static


.PHONY: all
all: $(TARGET)

# clean
.PHONY: clean
clean:
	rm -rf *.o lib/graphics/*.o lib/font/*.o
	rm	-rf	kernelFont.o
	rm -rf .main.d lib/graphics/*.d lib/font/.graphics.d
	rm	-rf	kernel.elf

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)	-lc	-lc++

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@


%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $<


.%.d: %.c
	clang $(CPPFLAGS) $(CFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.c=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<


#kernelFont.bin: lib/font/kernelFont.txt
#	../tools/makefont.py -o $@ $<

#kernelFont.o: kernelFont.bin
#	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

.%.d: %.bin
	touch $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)