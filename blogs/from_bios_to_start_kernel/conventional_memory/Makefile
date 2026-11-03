.PHONY: build run
run: build
	qemu-system-i386 -drive format=raw,file=vga.bin
build:
	nasm -f bin vga.asm -o vga.bin
clean:
	rm -f vga.bin

