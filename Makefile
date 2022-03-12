# Build the kernel
CC := i386-elf-c++
LD := i386-elf-ld
BIN_DIR = bin
ALL_SOURCE_FILES := $(wildcard kernel/*.cpp) $(wildcard lib/*.cpp)
ALL_OBJS := bin/kernel_entry.o bin/kernel_init.okernel bin/basic_io.okernel bin/basic_mem_util.okernel bin/zio.olib bin/zstring.olib bin/zstringutil.olib bin/zassert.olib bin/vga.odriver bin/ata.odriver bin/instr.odriver

# First, create a blank 1.44 MB (1,440 KB) floppy disk
# Second, copy os.bin to os_floppy.img
$(BIN_DIR)/os.hdd: $(BIN_DIR)/os.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024 conv=notrunc
	dd if=$^ of=$@ bs=512 conv=notrunc

$(BIN_DIR)/os.bin: $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin
	cat $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin > $@

$(BIN_DIR)/boot.bin: boot/boot.asm
	nasm -f bin $^ -o $@

$(BIN_DIR)/kernel_entry.o: boot/kernel_entry.asm
	nasm -f elf32 $^ -o $@

$(BIN_DIR)/%.okernel: kernel/%.cpp
	$(CC) -Os -Iinclude -ffreestanding -lgcc -nostdlib -fno-rtti -fno-exceptions -m32 -std=c++20 -c -o $@ $^

$(BIN_DIR)/%.odriver: drivers/%.cpp
	$(CC) -Os -Iinclude -ffreestanding -lgcc -nostdlib -fno-rtti -fno-exceptions -m32 -std=c++20 -c -o $@ $^

$(BIN_DIR)/%.olib: lib/%.cpp
	$(CC) -Os -Iinclude -ffreestanding -lgcc -nostdlib -fno-rtti -fno-exceptions -m32 -std=c++20 -c -o $@ $^

$(BIN_DIR)/kernel.bin: $(ALL_OBJS)
	$(LD) -o $@ -Ttext=0x1000 $^ --oformat=binary

clean:
	rm $(BIN_DIR)/*
