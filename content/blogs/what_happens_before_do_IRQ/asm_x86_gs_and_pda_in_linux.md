---
title: "linux gs and pda"
draftDate: 2026-04-17T10:33:24+08:00
date: 2026-04-17T10:33:24+08:00
lastMod: 2026-04-17T10:33:24+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "linux gs and pda"
tags: [ "Linux kernel", "asm" ]
---

换栈的时候

```
cmoveq %gs:pda_irqstackptr,%rsp
```

* cmoveq: Condition MOVe Equal Quadratic (when ZF=1)
* pda: Processor Data Area
* GS: ?? 段选择寄存器之一，常用于 process data area(pda)


* GS
qemu 中我们可以看到 GS 是

```
GS =0000 ffffffff8052a000 ffffffff 00c00100
```

GS:offset 等同于 offset(%gs) 但

* %gs:variable_name：常用于访问全局定义的 per-CPU 变量。
* offset(%gs)：在手写汇编或计算特定偏移时更为常见。

也就是说上面这段 cmoveq 在条件成立时等同于

```
movq pda_irqstackptr(%gs), %rsp
```

linux 中定义了 pda_irqstackptr 宏为 40

执行完 cmoveq 之后 rsp 确实拿到了地址是 GS+0x28 的内存

```
(gdb) x (0xffffffff8052a000 + 0x28)
0xffffffff8052a028 <boot_cpu_pda+40>:   0xffffffff805c0fc0
(gdb) p $rsp
$54 = (void *) 0xffffffff805c0fc0 <boot_cpu_stack+16320>
```

GDB 中查看 GS（0xffffffff8052a000） 地址的内存内容是：

```
(gdb) x 0xffffffff8052a000
0xffffffff8052a000 <boot_cpu_pda>:      0xffffffff804f14a0
```


GS 指向的内存中存的 boot_cpu_pda:

```c
struct x8664_pda {
	struct task_struct *pcurrent;	/* 0  Current process */
	unsigned long data_offset;	/* 8 Per cpu data offset from linker
					   address */
	unsigned long kernelstack;  /* 16 top of kernel stack for current */
	unsigned long oldrsp; 	    /* 24 user rsp for system call */
        int irqcount;		    /* 32 Irq nesting counter. Starts with -1 */
	int cpunumber;		    /* 36 Logical CPU number */
#ifdef CONFIG_CC_STACKPROTECTOR
	unsigned long stack_canary;	/* 40 stack canary value */
					/* gcc-ABI: this canary MUST be at
					   offset 40!!! */
#endif
	char *irqstackptr;
    //...
};
struct x8664_pda boot_cpu_pda[NR_CPUS] __cacheline_aligned;
```

GS offset 0x40 存的 `boot_cpu_stack`

```c
char boot_cpu_stack[IRQSTACKSIZE] __attribute__((section(".bss.page_aligned")));
```

和 GS 指向的内存中存的 x8664_pda boot_cpu_stack 在这里关联上

```c
void pda_init(int cpu){
	struct x8664_pda *pda = cpu_pda(cpu);
	if (cpu == 0) {
		/* others are initialized in smpboot.c */
		pda->pcurrent = &init_task;
		pda->irqstackptr = boot_cpu_stack; 
	} else {
		pda->irqstackptr = (char *)
			__get_free_pages(GFP_ATOMIC, IRQSTACK_ORDER);
		if (!pda->irqstackptr)
			panic("cannot allocate irqstack for cpu %d", cpu); 
	}
}
```

## summary
* GS 指向 `x8664_pda`
* %GS:40 是 `x8664_pda -> irqstackptr`，在 pda_init 时和 boot_cpu_stack 关联上

## swapgs

SWAPGS is a privileged x86-64 CPU instruction used by operating system kernels to quickly swap the user-mode GS base register with a kernel-mode GS base address. It enables safe and fast access to per-CPU kernel data structures (like task pointers or stack pointers) when transitioning from user space to kernel space (Ring 0), particularly via the SYSCALL instruction. 

```
	.macro interrupt func
    // ...
	je 1f
	swapgs
1:	incl	%gs:pda_irqcount
	cmoveq %gs:pda_irqstackptr,%rsp
	push    %rbp
	call \func

```

In Linux assembly (GAS syntax), je 1f means "Jump if Equal" to a local numeric label 1 located forward (f) in the code. It checks the Zero Flag (ZF) set by a previous cmp instruction; if values were equal, it jumps to the next 1: label, otherwise, it continues. 

