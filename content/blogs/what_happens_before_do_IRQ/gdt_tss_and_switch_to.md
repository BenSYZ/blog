---
title: "gdt, tss and switch_to"
draftDate: 2026-04-15T10:56:01+08:00
date: 2026-04-15T10:56:01+08:00
lastMod: 2026-04-15T10:56:01+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben, Gemini"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "gdt and tss"
tags: [ "Linux", "Linux task", "asm" ]
---

[Gemini talk](https://www.google.com/search?client=firefox-b-d&q=what+is+global+descriptor+table&fbs=ADc_l-Z6XGGbkKmpylZqg2J7ZOuVNvV7jKtLqzLZ63dd2zPtrsojpaijeJRLrp4clUvcwVJFwPc3SunfhAadIHhnTsFBVMAAXIZgDnZp5KTskh6spmEAN_m5c9Zl7QQ9S1AEoyH9D4lVpv_wnngMRGAxLVYtAQ0lv6yE5rdDK6YSHRfe9mjQavptxK4vEoFd-TzuKbzrFaNFsrKlv_gEfcxDRavrClfR-Q&ved=2ahUKEwjE3dvK0e6TAxWMDTQIHTwEA6cQ0NsOegQIAxAB&aep=10&ntc=1&mstk=AUtExfAFlhqj7TECPIPnuH2Rfbk4_jTcsVwZGnrXQ9YZFv9GR5c-KZ_wyVy-DWVHuX9hT4IQ2lzAZg3ShTFHZBZuWY2p-WtAwgIxKnnqhXdlG6H1C_v5ER_elvkoz1CHvQBOsIiPIdh7sSBcOnkTdVD17hjcuyTEHNDbOeTsxX-zwaH2Q4SbZovJhpuhTiIL-5jUSCaAIAB1PbvY_MlAXYULYnVaIjMImScqxYKO1JSWaL5_c9lI6BfOMii-WVzAKw-8AVYj-nKH9TuUj-Sz5tf11SkF1ebQCvnW4nzJggLa6fM6JhXMjSBdmqqJRcbBQRGHmsH2CmZDtW5kGQ&csuir=1&mtid=5uDeaeS5OYu90PEPi7KBIQ&udm=50)


## TSS 的功能

* 特权级切换与堆栈保存：当处理器从用户态 (Ring 3) 进入内核态 (Ring 0) 时（例如发生中断或系统调用），TSS 提供了内核态所需的堆栈指针 (ESP0/RSP0)。没有它，CPU 将不知道把寄存器压入哪个堆栈，从而导致系统崩溃。
* I/O 权限管理：TSS 包含一个 I/O 许可位图 (I/O Permission Bitmap)，操作系统可以用它来精确控制用户进程可以访问哪些硬件端口。
* 中断堆栈表 (IST)：在 64 位模式下，TSS 引入了 IST，允许某些关键中断（如双重错误 Double Fault）使用独立、预定义的已知良好堆栈，确保即使内核堆栈溢出，系统也能处理异常。


## TSS 与 GDT 的关系
TSS 本身并不是 GDT 的一部分，但它必须在 GDT 中拥有一个描述符 (TSS Descriptor) 才能被使用。 

    操作系统在内存中开辟一块空间定义 TSS。
    在 GDT 中创建一个描述符，指向该空间的基地址和长度。
    通过 LTR (Load Task Register) 指令将 GDT 中的选择子加载到处理器的 TR 寄存器 中，从而激活该 TSS。 

现代操作系统（如 Linux 或 Windows） 通常不再为每个进程创建单独的 TSS，而是为每个 CPU 核心维护一个全局 TSS。在进程切换时，内核只需动态更新这个全局 TSS 中的堆栈指针即可。 

## summary(by Ben)

TSS: task state segment: 在内存中，per task
GDT: TSS 在内存中的地址存于 GDT 中，per CPU

任务切换时更新 TSS 即可

## contents in TSS(by Ben)

简单来说 TSS 中存了当前 task 所需的所有状态，rsp rbp 等等

```
     15                   0
     +--------------------+
     | Task LDT Selector  | 42
     | DS Selector        | 40
     | SS Selector        | 38
     | CS Selector        | 36
     | ES Selector        | 34
     | DI                 | 32
     | SI                 | 30
     | BP                 | 28
     | SP                 | 26
     | BX                 | 24
     | DX                 | 22
     | CX                 | 20
     | AX                 | 18
     | FLAG Word          | 16
     | IP (Entry Point)   | 14
     | SS2                | 12
     | SP2                | 10
     | SS1                | 8
     | SP1                | 6
     | SS0                | 4
     | SP0                | 2
     | Previous Task Link | 0
     +--------------------+

Figure 10-10. 16-Bit TSS Format
> Volume 3A, 10.6, 16-BIT TASK-STATE SEGMENT (TSS)
```

64 位中还包括 I/O map base address
```
    31                      15                   0
    +----------------------+----------------------+
    | I/O Map Base Address | Reserved             | 100
    | Reserved                                    | 96 
    | Reserved                                    | 92 
    | IST7 (upper 32 bits)                        | 88 
    | IST7 (lower 32 bits)                        | 84 
    | IST6 (upper 32 bits)                        | 80 
    | IST6 (lower 32 bits)                        | 76
    | IST5 (upper 32 bits)                        | 72
    | IST5 (lower 32 bits)                        | 68
    | IST4 (upper 32 bits)                        | 64
    | IST4 (lower 32 bits)                        | 60
    | IST3 (upper 32 bits)                        | 56
    | IST3 (lower 32 bits)                        | 52
    | IST2 (upper 32 bits)                        | 48
    | IST2 (lower 32 bits)                        | 44
    | IST1 (upper 32 bits)                        | 40
    | IST1 (lower 32 bits)                        | 36
    | Reserved                                    | 32
    | Reserved                                    | 28
    | RSP2 (upper 32 bits)                        | 24
    | RSP2 (lower 32 bits)                        | 20
    | RSP1 (upper 32 bits)                        | 16
    | RSP1 (lower 32 bits)                        | 12
    | RSP0 (upper 32 bits)                        | 8
    | RSP0 (lower 32 bits)                        | 4
    | Reserved                                    | 0
    +----------------------+----------------------+
Reserved bits. Set to 0.

> Volume 3A, 10.7, TASK MANAGEMENT IN 64-BIT MODE
```

```c
// include/asm-x86/processor_64.h 
struct tss_struct {
	u32 reserved1;
	u64 rsp0;	
	u64 rsp1;
	u64 rsp2;
	u64 reserved2;
	u64 ist[7];
	u32 reserved3;
	u32 reserved4;
	u16 reserved5;
	u16 io_bitmap_base;
	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit. Thus we have:
	 *
	 * 128 bytes, the bitmap itself, for ports 0..0x3ff
	 * 8 bytes, for an extra "long" of ~0UL
	 */
	unsigned long io_bitmap[IO_BITMAP_LONGS + 1];
} __attribute__((packed)) ____cacheline_aligned;
```

TSS 中的 SP 指用于记录内核栈地址，而不记录用户栈地址，用户栈地址存于内核栈

## kernel context switch(TODO)
当 CPU 在两个内核任务（即进程的内核态部分）之间切换时，由于不涉及特权级从 Ring 3 到 Ring 0 的跳转，TSS 此时是不参与的。
内核栈指针（SP）的存储位置取决于该任务的状态：
### 1. 对于正在运行的任务

* 存储位置： 直接存储在 CPU 的物理寄存器 RSP (x86_64) 或 ESP (x86_32) 中。
* CPU 指令（如 push, pop, call）直接操作这个寄存器。

### 2. 对于被切走（挂起）的任务
当内核执行进程切换（Context Switch）时，被换下的任务的内核栈指针需要找个地方存起来，以便下次换回来。

* 存储位置： 存储在该任务的 进程控制块 (PCB) 中。
* 在 Linux 内核中，具体的字段是：task_struct -> thread -> sp。

### 3. 切换过程的“接力”
当 Linux 调用 __switch_to（汇编部分为 switch_to）切换进程 A 和 B 时，发生了以下动作：

   1. 保存 A 的栈顶：内核将当前的 RSP 寄存器值压入 A 的内核栈，或者直接保存到 A->thread.sp 中。
   2. 加载 B 的栈顶：内核从 B->thread.sp 中读取之前保存的值，并将其写入物理寄存器 RSP。
   3. 接管 CPU：一旦 RSP 指向了 B 的内核栈，接下来的 ret 指令就会从 B 的栈中弹出返回地址。此时，CPU 正式进入进程 B 的执行流。

### 4. 关键点：TSS 在这里做什么？
虽然在两个内核任务切换时，CPU 不从 TSS 读取 RSP0，但 Linux 内核顺便会做一件事：

* 内核会更新 TSS 中的 RSP0 字段，将其设置为进程 B 的内核栈顶。
* 目的：这是为了“未雨绸缪”。如果进程 B 稍后返回了用户态，而在用户态运行时突然发生了一个中断，CPU 硬件需要通过 TSS 知道该跳回到进程 B 的哪个内核栈位置。

### 总结

* 活动任务：在 RSP 寄存器里。
* 睡眠任务：在 task_struct->thread.sp 内存里。
* TSS 的作用：仅作为“入口指南”，记录如果从用户态进入内核态，该用哪个栈。

### my trace(by Ben)
```c
#define switch_to(prev,next,last) \
	asm volatile(SAVE_CONTEXT						    \
		     "movq %%rsp,%P[threadrsp](%[prev])\n\t" /* save RSP */	  \
		     "movq %P[threadrsp](%[next]),%%rsp\n\t" /* restore RSP */	  \
		     "call __switch_to\n\t"					  \
		     ".globl thread_return\n"					\
		     "thread_return:\n\t"					    \
		     "movq %%gs:%P[pda_pcurrent],%%rsi\n\t"			  \
		     "movq %P[thread_info](%%rsi),%%r8\n\t"			  \
		     LOCK_PREFIX "btr  %[tif_fork],%P[ti_flags](%%r8)\n\t"	  \
		     "movq %%rax,%%rdi\n\t" 					  \
		     "jc   ret_from_fork\n\t"					  \
		     RESTORE_CONTEXT						    \
		     : "=a" (last)					  	  \
		     : [next] "S" (next), [prev] "D" (prev),			  \
		       [threadrsp] "i" (offsetof(struct task_struct, thread.rsp)), \
		       [ti_flags] "i" (offsetof(struct thread_info, flags)),\
		       [tif_fork] "i" (TIF_FORK),			  \
		       [thread_info] "i" (offsetof(struct task_struct, stack)), \
		       [pda_pcurrent] "i" (offsetof(struct x8664_pda, pcurrent))   \
		     : "memory", "cc" __EXTRA_CLOBBER)


context_switch
    switch_to
        __switch_to
            struct thread_struct *prev = &prev_p->thread,
                *next = &next_p->thread;
            int cpu = smp_processor_id();
            struct tss_struct *tss = &per_cpu(init_tss, cpu);
            tss->rsp0 = next->rsp0;
```

switch_to 前两行更新了 rsp，

ldt 用于 per process 的内存管理现已被 page table 和 CR3 替换了，那里也涉及 GDT 的修改，但这不是我们关心的重点


## cpu lgdt(by Ben)
```c
// include/asm-x86/desc_64.h
static inline void load_gdt(const struct desc_ptr *ptr)
{
	asm volatile("lgdt %w0"::"m" (*ptr));
}
```

## show current GDT(by Ben)

gdb cannot show GDT IDTR like registers

这是因为 GDB 本身并不直接支持通过 $name 访问 x86 的系统表寄存器（如 GDTR, IDTR, TR），这些寄存器不在标准远程调试协议（GDB Remote Serial Protocol）的普通寄存器列表中。

ctrl+a c: switch between qemu command mode and os tty

```
(qemu) info registers

CPU#0
RAX=0000000000000000 RBX=ffffffff8020b155 RCX=0000000000000000 RDX=ffffffff8058
RSI=0000000000000001 RDI=ffffffff804f4cc0 RBP=ffffffff80563f48 RSP=ffffffff8058
R8 =0000000000000000 R9 =0000000000ffff24 R10=0000000000000000 R11=000000000001
R12=0000000000000000 R13=ffffffffffffffff R14=ffffffff805900a0 R15=000000000000
RIP=ffffffff8020b182 RFL=00000246 [---Z-P-] CPL=0 II=0 A20=1 SMM=0 HLT=1
ES =0018 0000000000000000 ffffffff 00c09300 DPL=0 DS   [-WA]
CS =0010 0000000000000000 ffffffff 00a09b00 DPL=0 CS64 [-RA]
SS =0018 0000000000000000 ffffffff 00c09300 DPL=0 DS   [-WA]
DS =0018 0000000000000000 ffffffff 00c09300 DPL=0 DS   [-WA]
FS =0000 0000000000000000 ffffffff 00c00100
GS =0000 ffffffff8052a000 ffffffff 00c00100
LDT=0000 0000000000000000 ffffffff 00c00000
TR =0040 ffff810001005d00 0000206f 00008b00 DPL=0 TSS64-busy
GDT=     ffffffff80564000 00000080
IDT=     ffffffff805c7000 00000fff
CR0=8005003b CR2=0000000000516f90 CR3=0000000000201000 CR4=000006e0
DR0=0000000000000000 DR1=0000000000000000 DR2=0000000000000000 DR3=00000000000
DR6=00000000ffff0ff0 DR7=0000000000000400
EFER=0000000000000d01
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=0000000000000000 0000000000000000 XMM01=0000000000000000 0000000000000000
XMM02=0000000000000000 0000000000000000 XMM03=0000000000000000 0000000000000000
XMM04=0000000000000000 0000000000000000 XMM05=0000000000000000 0000000000000000
XMM06=0000000000000000 0000000000000000 XMM07=0000000000000000 0000000000000000
XMM08=0000000000000000 0000000000000000 XMM09=0000000000000000 0000000000000000
XMM10=0000000000000000 0000000000000000 XMM11=0000000000000000 0000000000000000
XMM12=0000000000000000 0000000000000000 XMM13=0000000000000000 0000000000000000
XMM14=0000000000000000 0000000000000000 XMM15=0000000000000000 0000000000000000
```


