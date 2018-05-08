as = i686-elf-as
cc = i686-elf-gcc
qemu = qemu-system-i386
cflags = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
lflags = -ffreestanding -O2 -nostdlib -lgcc

src = $(wildcard *.c) $(notdir $(wildcard lib/*.c)) $(notdir $(wildcard driver/*.c))
linker = linker.ld
target = sage.bin
iso = sage.iso

vpath %.c lib
vpath %.c driver

$(target) : $(src:%.c=build/%.o) build/boot.o
	$(cc) -T $(linker) -o $@ $(lflags) $^
	cp $(target) ../iso/boot/
	grub-mkrescue -o ../$(iso) ../iso/
	$(qemu) ../$(iso) -d int -no-reboot -monitor stdio

build/boot.o : boot.s
	$(as) $< -o $@

build/%.o : %.c
	$(cc) -c $< -o $@ $(cflags)

build/%.d : %.c
	@set -e; \
	rm -f $@; \
	$(cc) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(src:%.c=build/%.d)
