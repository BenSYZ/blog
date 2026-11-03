[org 0x7c00]
[bits 16]

start:
    ; 1. Switch to VGA Mode 13h (320x200, 256 colors) via BIOS INT 10h
    mov ax, 0x0013
    int 0x10

    ; 2. Set ES segment register to VGA frame buffer base (0xA0000)
    mov ax, 0xA000
    mov es, ax

    ; 3. Fill the 320x200 (64,000 pixels) frame buffer with color index 0x0A (Light Green)
    xor di, di          ; DI = 0 (Destination offset: 0xA000:0x0000)
    mov cx, 320 * 200   ; Total number of pixels to write
    mov al, 0x0A        ; Color index (0x0A = Light Green in standard VGA palette)
    rep stosb           ; Copy AL to ES:DI, increment DI, repeat CX times

hang:
    cli
    hlt
    jmp hang            ; Infinite loop to stop CPU execution

; Pad out the rest of the 512-byte boot sector and add the boot signature
times 510 - ($ - $$) db 0
dw 0xAA55
