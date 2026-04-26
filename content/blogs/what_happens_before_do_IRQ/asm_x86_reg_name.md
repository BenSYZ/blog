---
title: "asm x86 reg name"
draftDate: 2026-04-18T09:43:56+08:00
date: 2026-04-18T09:43:56+08:00
lastMod: 2026-04-18T09:43:56+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "asm x86 reg name rule of SP/BP/IP"
tags: [ "asm" ]
---

* SP: stack pointer, 记录了栈顶在内存中的地址
* BP: base pointer, 记录了栈底在内存中的地址
* IP: instruction pointer, 记录了即将要执行的指令在内存中的地址

* 16 位中，他们的名字就是 sp，bp，ip
* 32 位中，我们会给这些寄存器的名字加一个前缀 E（Extended）
* 64 位中，我们会给这些寄存器的名字加一个前缀 R（Register）

[前缀为什么是 R](https://softwareengineering.stackexchange.com/a/127764/469476)

## 查看
gdb/qemu 中 info register

和 $sp 类似，$fp（或 $ebp / $rbp）在 GDB 中代表的是基址指针（Base Pointer），通常也被称为帧指针（Frame Pointer）。

| 64位全称 (Long)      | 32位名称 (Double Word) | 16位名称 (Word) | 8位低位 (Byte) | auto bit pseudo-reg in gdb |
|----------------------|------------------------|-----------------|----------------|----------------------------|
| $rsp (Stack Pointer) | `$esp`                 | `$sp`           | `$spl`         | `$sp`                      |
| $rbp (Base Pointer)  | `$ebp`                 | `$bp`           | `$bpl`         | `$fp`                      |

To print 16 bit of sp: force the type

```gdb
p/x (unsigned short)$rsp
```

有些 GS/GDT/IDT 只能在 qemu 中看，gdb 中是看不到的


