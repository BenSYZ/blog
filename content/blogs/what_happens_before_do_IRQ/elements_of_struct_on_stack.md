---
title: "结构体中的成员在栈上的顺序"
draftDate: 2026-04-17T15:27:07+08:00
date: 2026-04-17T15:27:07+08:00
lastMod: 2026-04-17T15:27:07+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "arg mem address in stack"
tags: [ "Linux kernel", "C-lang", "memory" ]
---


rdi 传入指针后，指向 r15

```c
struct pt_regs {
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long rbp;
	unsigned long rbx;
/* arguments: non interrupts/non tracing syscalls only save upto here*/
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long rax;
	unsigned long rcx;
	unsigned long rdx;
	unsigned long rsi;
	unsigned long rdi;
	unsigned long orig_rax;
/* end of arguments */
/* cpu exception frame or undefined */
	unsigned long rip;
	unsigned long cs;
	unsigned long eflags;
	unsigned long rsp;
	unsigned long ss;
/* top of stack page */
};
```

stack graph

```
low address
+-----+
| r15 |
| r14 |
| r13 |
| ... |
| ss  |
+-----+
high address
```

上图中 栈顶就是在上面，栈底就是在下面，但是地址是低位在上，高位在下

这样的图形中，结构体前面的内容在最上面（在最低的地址），在栈顶

多个参数时，第一个栈上参数（前几个是放在寄存器上的）在最上面，在栈顶。

压栈是从后往前压的

----------

为了 top 对应 high address，bottom 对应 low address，很多时候画是这样画的

这样 栈顶就是在下面，栈底就是在上面，但是方位代表了地址。

```
high address
+-----+
| ss  |
| ... |
| r13 |
| r14 |
| r15 |
+-----+
low address
```

这样的图形中，结构体前面的内容在最下面（在最低的地址），在栈顶

多个参数时，第一个栈上参数（前几个是放在寄存器上的）在最下面，在栈顶。

压栈是从后往前压的
