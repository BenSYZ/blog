---
title: "asm x86 call and stack"
draftDate: 2026-04-18T11:29:06+08:00
date: 2026-04-18T11:29:06+08:00
lastMod: 2026-04-18T11:29:06+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "asm x86 function call and stack change"
tags: [ "asm" ]
---

本文只讨论 C 语言中的函数函数调用，不涉及换栈的操作。

The CALL instruction in x86-64 assembly automatically decreases the stack pointer (RSP) by 8 bytes to push the return address (RIP) onto the stack. This action ensures the stack points to the top of the new stack frame, effectively allocating space for the return address. 

## main
### 函数调用前

```
     +-------------------+
     |                   | ← rbp
     |   main stack var  |
     |   main stack var  | ← rsp
     +-------------------+
```

在讨论 fn 后我们会知道当前 rbp 指的是谁

### call 前 (push arg)
准备参数压栈 by push

```
     +-------------------+
     |                   | ← rbp
     |   main stack var  |
     |   main stack var  |
     |   fn arg var      |
     |   fn arg var      | ← rsp
     +-------------------+
```

### call fn
call 等同于:

push %rip
jmp func_addr

1. 把 rip 压入栈
2. 跳转到函数地址

此时的栈：

```
     +-------------------+
     |                   | ← rbp
     |   main stack var  |
     |   main stack var  |
     |   fn arg var      |
     |   fn arg var      |
     |   RIP in main     | ← rsp
     +-------------------+
```

rbp 还是 main 的，rsp 指向 rip

### call 后（add $0x18, %rsp)
把往低地址方向增长的栈顶，缩回去。

## fn
### 进入
此时，rbp 还在 main 的 rbp 上，rsp 在 fn 内了，我们需要将 rbp 上移，在上移前需要保存起来，用于到时恢复。
所以在正式开始 fn 的工作前，fn 中存着上级函数的 rbp 和 rip，那么 rsp 呢？rsp 不需要存，连续的，向上把 rip 弹出，就回到上级函数的正确位置了。

pushq  %rbp
movq   %rsp,%rbp
sub    $0x18,%rsp   # 为栈上数据留下空间，后续基本不动 rsp 了（除非传参等）

1. 将 rbp 压入栈
2. 并将 rsp 指向 rbp
3. 如果 fn 有栈上数据，先将 rsp 下移

保存上级 rbp

```
     +---------------------+
     |                     | ← rbp
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        |
     |   RIP in main       | ← rsp(old)
     |   rbp of main       | ← rsp
     +---------------------+
```

下移 rbp

```
     +---------------------+
     |                     | ← rbp(old)
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        |
     |   RIP in main       |
     |   rbp of main       | ← rsp/rbp
     +---------------------+
```

正式开始 fn 内的工作了，下移 rsp

```
     +---------------------+
     |                     |
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        |
     |   RIP in main       |
     |   rbp of main       | ← rsp(old)/rbp
     |   rsv fn stack var  |
     |   rsv fn stack var  | ← rsp
     +---------------------+
```

### 退出
leave
ret

#### leave
把 rsp 和 rbp 都回退，准备弹出 rip 回到上级函数

mov %rbp,%rsp ; Restore stack pointer to where the base pointer is
pop %rbp      ; Pop the saved base pointer back into ebp

先把 rsp 回退到 rbp 的位置，对应进入时的下移 rsp 准备 fn 的参数

```
     +---------------------+
     |   rbp of main       |
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        |
     |   RIP in main       | ← rbp/rsp
     |   fn stack var      |
     |   fn stack var      | ← rsp(old)
     +---------------------+
```

再把 保存的 rbp 退回，对应进入时的保存上级 rbp，同时 rsp 也上移

```
     +---------------------+
     |   rbp of main       | ← rbp
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        | ← rsp
     |   RIP in main       | ← rbp(old)/rsp(old)
     |   fn stack var      |
     |   fn stack var      |
     +---------------------+
```

#### ret

ret 等同于

pop %rip

```
     +---------------------+
     |   rbp of main       | ← rbp
     |   main stack var    |
     |   main stack var    |
     |   fn arg var        |
     |   fn arg var        | ← rsp
     |   RIP in main       | ← rsp(old)
     |   fn stack var      |
     |   fn stack var      |
     +---------------------+
```

## main
把压入的参数回退

add $0x18, %rsp

```
     +---------------------+
     |   rbp of main       | ← rbp
     |   main stack var    |
     |   main stack var    | ← rsp
     |   fn arg var        |
     |   fn arg var        | ← rsp(old)
     |   RIP in main       |
     |   rsv fn stack var  |
     |   rsv fn stack var  |
     +---------------------+
```

## summary

| code part | complex instr | enter（上往下）          | exit（下往上）           | note（enter）                    |
|-----------|---------------|--------------------------|--------------------------|--------------------------------|
| main      | prepare arg   | push arg                 | add $0x20, $rsp          | 为 fn 的临时变量准备空间       |
| main      | call/ret      | call.1: push %rip        | ret.1: pop %rip          | 保存 rip，用于从 fn 回退       |
| main      | call/ret      | call.2: jmp fn_addr      | ret.2: jmp main_offset   | 跳转开始 fn                    |
| fn        | enter/leave   | enter.1: push %rbp       | leave.1.1: pop %rbp      | 正式开始 fn 前的工作：保存 rbp |
| fn        | enter/leave   | enter.2: mov %rsp %rbp   | leave.1.2: pop %rbp      | 正式开始 fn 前的工作：下移 rsp |
| fn        | enter/leave   | enter.2: sub $0x18, %rsp | leave.2: mov %rbp, %rsp  | 为 fn 的临时变量准备空间       |

### 调用方
准备参数、调用前：

+ rbp 指向该层函数的 栈底，栈底记录着上层 rbp，栈底之上（栈外）存的是上层 RIP
+ rsp 指向该层函数的 栈顶

1. 准备参数
    * push arg (rsp move down )
2. 调用(call fn)
    * push rip
    * jmp fn_addr
3. 清理参数
    * move rsp (rsp move up )

此时恢复到了准备参数和调用前的状态

### 被调用方

正式代码前

1. 保存上层函数栈底
    * push %rbp
2. 移动栈底
    * mov %rsp, %rbp
3. 移动栈顶，准备临时变量
    * sub $0xF, %rsp

正式代码后

1. 缩回栈顶
    * mov %rbp, %rsp
2. 栈底移回上层函战地栈底
    * pop %rbp
3. 返回（ret)
    * pop rip


## GDB to view stack


```c
#define ul unsigned long
void fn2(){
    ul a = 0xC00;
    ul b = 0xC01;
}
void fn1(){
    ul a = 0xB00;
    ul b = 0xB01;
    fn2();
}
int main(){
    ul a = 0xA00;
    ul b = 0xA01;
    fn1();
}
```

Tips:

0. Windows 上 unsigned long 占用的空间是 32bit，可以用 unsigned long long
1. 统一用 unsigned long，这样栈上数据和指针一致，都是 64 bit
2. 申请两个变量，如果申请了三个，会有为了对齐，申请 4 个变量空间(4*8byte)

```
(gdb) x/16xg $rsp
0x0007fffffffe4d0:	0x00007fffffffe4f0	0x000055555555514d # rbp of fn,             rip of fn
0x0007fffffffe4e0:	0x0000000000000b00	0x0000000000000b01 # fn1 var a,             fn1 var b
0x0007fffffffe4f0:	0x00007fffffffe510	0x000055555555516d # rbp of main,           rip of main
0x0007fffffffe500:	0x0000000000000a00	0x0000000000000a01 # main var a,            main var b
0x0007fffffffe510:	0x00007fffffffe5c0	0x00007ffff7dd46c1 # rbp of caller of main, rip of caller of main
0x0007fffffffe520:	0x00007fffffffe600	0x00007fffffffe648
0x0007fffffffe530:	0x00000001f7fc5000	0x0000555555555150
0x0007fffffffe540:	0x00007fffffffe580	0x00007ffff7fe36c6
(gdb) bt
#0  fn2 () at a.c:3
#1  0x000055555555514d in fn1 () at a.c:9
#2  0x000055555555516d in main () at a.c:14
```

bt 打印的其实就是 rip，出现栈混乱的时候大概是中间插入了奇怪的数据，导致找不到 rip

```
# at fn2
(gdb) p $rbp
$1 = (void *) 0x7fffffffe4d0
(gdb) p $rip
$2 = (void (*)()) 0x55555555511d <fn2+4>

(gdb) up
#1  0x000055555555514d in fn1 () at a.c:9
9	    fn2();

# at fn1
(gdb) p $rbp
$3 = (void *) 0x7fffffffe4f0
(gdb) p $rip
$4 = (void (*)()) 0x55555555514d <fn1+29>

(gdb) up
#2  0x000055555555516d in main () at a.c:14
14	    fn1();

# at main
(gdb) p $rbp
$5 = (void *) 0x7fffffffe510
(gdb) p $rip
$6 = (void (*)()) 0x55555555516d <main+29>
```


## Tips:
inline 不会有压栈 rbp 等是不变的

