---
title: "asm x86 mov and lea"
draftDate: 2026-04-18T14:29:47+08:00
date: 2026-04-18T14:29:47+08:00
lastMod: 2026-04-18T14:29:47+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "asm x86 mov and lea"
tags: [ "asm" ]
---


## mov
```asm
# prepare rax rbx
	movq	$1, %rax
	movq	$2, %rbx

# mov
	movq	%rax, %rbx      # copy content in rax to rbx
	movq	(%rax), %rbx    # copy content in memory whose address is recorded in rax to rbx
	movq	%rax, (%rbx)    # copy content in rbx to memory whose address is recorded in rbx
#	movq	(%rax), (%rbx)  # not allowed: number of operand in memory location at most to one

# leaq
	leaq	(%rax), %rbx    # copy memory address of a memory whose address is recorded in rax To rbx
	leaq	8(%rsp), %rbp   # copy memory address of a memory whose address is value recorded in rax plus 8 To rbx
```

### 直接拷贝和解引用

```asm
mov %rsp, %rbp   #改寄存器的内容
mov %rsp, (%rbp) # de-reference 改寄存器指向的内存
```

括号等同于 C 中 `*`


## leaq
`leaq`: Load Effective Address Quadwards

### 将内存偏移拷贝

```
lea 0x8(%rsp), %rbp
```

等同于

```
mov %rsp, %rbp
add $0x8, %rbp
```


------

括号在 mov 和 lea 中含义不同

```
lea (%rsp), %rbp
```

等同于

```
mov %rsp, %rbp
```

## 访问 rip
为什么 rip 不能通过 add 访问，而要通过 lea 访问

> Gemini
> add %rip, %rdx 被禁止，主要是出于架构设计（By Design）和流水线效率的考虑：
>
>     流水线模糊性（Pipelining）：现代 CPU 是流水线化的。当一条指令在“执行阶段”时，%rip 实际上已经指向了后面好几条指令的位置。如果允许 mov %rdx, %rip，程序员看到的“当前地址”到底是哪一个？为了避免歧义，硬件设计者规定：%rip 只能通过跳转指令（jmp/call）修改，或通过 lea 这种特定寻址方式读取。
>     指令编码限制：在 x86 指令编码中，寄存器通常用 3 位或 4 位二进制表示（如 rax=0, rcx=1）。但在通用的算术指令（add, sub）的编码表里，根本没有给 %rip 预留“位置”。
>
> `lea    0xea5(%rip),%rdx`
>
> 逻辑上等同于：
> ```
> mov %rip %rdx
> add %rdx $0xea5
> ```

> Intel® 64 and IA-32 Architectures Software Developer’s Manual Combined Volumes: 1, 2A, 2B, 2C, 2D, 3A, 3B, 3C, 3D, and 4
> Volume 1 3.5 INSTRUCTION POINTER
