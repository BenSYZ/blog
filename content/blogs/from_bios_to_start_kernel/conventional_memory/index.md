According to the Multiboot specification, mem_lower specifies the amount (size) of lower memory (conventional memory below 1 MB) measured in kilobytes (KB). On a standard x86 PC architecture, mem_lower is almost always 640 (representing 640 KB of conventional memory spanning from physical address 0x00000 up to 0xA0000).

Therefore, mbi.mem_lower << 10 calculates the upper boundary (the end address) of conventional memory in bytes

For a standard 640 KB system, 640 * 1024 evaluates to 0xA0000 (the address right where the VGA text buffer and BIOS ROM areas begin).

https://gemini.google.com/app/35692b897ec58f41

## Technical Context: Memory Map Location

In real mode and legacy x86 system initialization, physical address space below 1 MB (Conventional Memory) is partitioned as follows:

* **`0x00000` – `0x9FFFF**` (0 to 640 KB): Conventional System RAM (Usable by OS/Programs).
* **`0xA0000` – `0xBFFFF**` (640 KB to 768 KB): Video Display RAM & Adapters.
* **`0xA0000` – `0xAFFFF**` (64 KB): VGA Frame Buffer (Graphics modes).
* **`0xB0000` – `0xB7FFF**` (32 KB): MDA / Monochrome text mode memory.
* **`0xB8000` – `0xBFFFF**` (32 KB): CGA / VGA Color Text Mode frame buffer (e.g., standard DOS console, early OS boot printouts).


* **`0xC0000` – `0xFFFFF**` (768 KB to 1 MB): Video ROM BIOS, Option ROMs, and System BIOS.

---

## How to Use `0xA0000`

make run
