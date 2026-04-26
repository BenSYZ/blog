---
title: "asm AT&T vs intel"
draftDate: 2026-04-14T22:47:49+08:00
date: 2026-04-14T22:47:49+08:00
lastMod: 2026-04-14T22:47:49+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Gemini"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "asm syntax difference between AT&T vs intel"
tags: [ "asm" ]
---


The primary difference between AT&T and Intel assembly syntax is the operand order. Intel syntax (destination first) is the native format for x86 processors used in Windows and official [Intel manuals](https://imada.sdu.dk/~kslarsen/dm546/Material/IntelnATT.htm), while AT&T syntax (source first) is the default for [GNU tools](https://stackoverflow.com/questions/972602/limitations-of-intel-assembly-syntax-compared-to-att) like GCC and GDB on Linux/Unix systems. [1, 2, 3] 

## Key Syntax Differences

| Feature [2, 4, 5, 6, 7] | Intel Syntax | AT&T Syntax |
|---|---|---|
| Operand Order | Opcode Dest, Src | Opcode Src, Dest |
| Register Prefix | None (e.g., eax) | % (e.g., %eax) |
| Immediate Value | None (e.g., 5) | $ (e.g., $5) |
| Size Suffixes | Explicit specifiers (dword ptr) | Suffixes on mnemonics (movl) |
| Memory Access | [base + index*scale + disp] | disp(base, index, scale) |

## Detailed Breakdown

* Operand Direction: In Intel syntax, mov eax, 1 means "move 1 into eax" (reads like a variable assignment: eax = 1). In AT&T syntax, movl $1, %eax means "move 1 into eax" (reads like a function call or English sentence: "move 1 to eax").
* Memory Addressing: Intel uses a mathematical expression inside brackets, which many find more intuitive. AT&T uses a comma-separated format where the displacement is outside the parentheses, e.g., 8(%ebp, %ecx, 4).
* Instruction Suffixes: AT&T uses a letter at the end of the instruction to denote data size: b (byte), w (word), l (long/32-bit), and q (quad/64-bit). Intel handles this with prefixes like byte ptr or qword ptr when the size is not clear from the registers. [2, 8, 9, 10, 11, 12] 

## Which One Should You Use?

* Use Intel if you are developing for Windows (MASM/NASM), doing reverse engineering (IDA Pro/Ghidra default to Intel), or following official [Intel/AMD documentation](https://www.reddit.com/r/osdev/comments/sz65bk/intel_or_att_syntax_assembly_for_x86_64/).
* Use [AT&T](https://www.att.com/) if you are writing inline assembly for [GCC or Clang](https://marcelofern.com/posts/asm/att-vs-intel-syntax/index.html) or working extensively in the Linux kernel where it is the established standard. [1, 8, 13, 14] 

Note: Most modern tools allow you to switch. For example, in GDB, you can use set disassembly-flavor intel to see Intel syntax instead of the default AT&T. [8] 
Are you working on a specific project, like a Linux driver or a Windows application, that requires one of these?

* [1] [https://stackoverflow.com](https://stackoverflow.com/questions/972602/limitations-of-intel-assembly-syntax-compared-to-att)
* [2] [https://imada.sdu.dk](https://imada.sdu.dk/~kslarsen/dm546/Material/IntelnATT.htm)
* [3] [https://www.secureideas.com](https://www.secureideas.com/blog/2021/05/linux-x86-assembly-how-to-build-a-hello-world-program-in-nasm.html#:~:text=The%20primary%20difference%20is%20in%20the%20operand,must%20be%20formatted%20so%20they%20aren%27t%20cross%2Dcompatible.)
* [4] [https://www.reddit.com](https://www.reddit.com/r/learnprogramming/comments/q14wga/att_vs_intel_syntax/)
* [5] [https://news.ycombinator.com](https://news.ycombinator.com/item?id=33585154#:~:text=That%27s%20interesting.%20My%20entire%20world%20revolves%20around,src%29%20is%20what%20seems%20normal%20to%20me.)
* [6] [https://www.freepascal.org](https://www.freepascal.org/docs-html/prog/progsu129.html#:~:text=The%20order%20of%20the%20source%20and%20destination,$%204%2C%20%25eax%27%20in%20the%20AT&T%20dialect.)
* [7] [https://paul.bone.id.au](https://paul.bone.id.au/blog/2018/09/05/x86-addressing/#:~:text=In%20my%20defense%20AT&T%20syntax%20was%20my,those%2C%20it%20sucks%2C%20particularly%20with%20addressing%20modes.)
* [8] [https://marcelofern.com](https://marcelofern.com/posts/asm/att-vs-intel-syntax/index.html)
* [9] [https://banisterfiend.wordpress.com](https://banisterfiend.wordpress.com/2008/08/17/att-vs-intel-assembly-syntax/)
* [10] [https://news.ycombinator.com](https://news.ycombinator.com/item?id=23467300)
* [11] [https://tuttlem.github.io](https://tuttlem.github.io/2014/03/25/assembly-syntax-intel-at-t.html)
* [12] [https://wiki.osdev.org](https://wiki.osdev.org/Opcode_syntax)
* [13] [https://www.reddit.com](https://www.reddit.com/r/osdev/comments/sz65bk/intel_or_att_syntax_assembly_for_x86_64/)
* [14] [https://stackoverflow.com](https://stackoverflow.com/questions/8549427/nasm-intel-versus-att-syntax-what-are-the-advantages)
