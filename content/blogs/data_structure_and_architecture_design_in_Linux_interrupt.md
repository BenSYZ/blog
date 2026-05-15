---
title: "中断的数据结构和架构设计"
draftDate: 2026-04-30T15:13:43+08:00
date: 2026-04-30T15:13:43+08:00
lastMod: 2026-04-30T15:13:43+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "本文介绍了在 Linux 中中断的架构设计，以及涉及到的各层的数据结构"
tags: [ "Linux kernel", "interrupt", "Linux kernel design" ]
---

## Introduction
[上一篇文章](./what_happens_before_do_IRQ/_index.md)我们着重介绍了 `do_IRQ` 前的调用栈，简单介绍了 `IRQ0xXY_interrupt` 是如何注册到全局中断表的。但还有很多问题没有解决：

* `irq = __get_cpu_var(vector_irq)[vector];`，vector 转 irq 的数据结构是怎么构造的？
* `request_irq` 是如何申请 irq 的？
* irq 和 vector 的关系是怎么够构造的？
* 从外设发出中断到我们通过 request_irq 注册的函数，期间涉及的数据结构有哪些？

这篇文章主要介绍了，注册中断时涉及的数据结构，它们的构造、存放位置，主要包括 `idt_descr`(`idt_table`)，`vector_irq` 和 `irq_desc`。

## `request_irq`
### 错误的想法
首先我们来看一下 `request_irq` 是如何申请 irq 的。我们知道在 `do_IRQ` 时，我们会用 `irq = __get_cpu_var(vector_irq)[vector];` 拿到 vector 对应的 irq，然后拿着这个 irq 去执行注册的中断函数。那么很简单直接的设计想法就是：

`request_irq` 时去更新 PER CPU DATA 区（`vector` 转 `irq`）和 APIC ？

驱动端这里改了绑定，设备硬件端也要改。或者说，在驱动注册时，硬件这边的改动也要注册。

* 驱动将特定的 IRQ 绑定到 vector
    * 更新 vector 和 IRQ 的绑定关系，更新 Per CPU 内存区
* 设备将特定的 IRQ 绑定到这个 vector
    * 更新 APIC

这样，设备 IRQ 给到 APIC 之后，APIC 会给正确的 vector 到

```mermaid
graph LR
A[device] -- IRQ --> B[APIC]
B -- vector --> C[CPU]
C -- vector --> D[Linux kernel]
D -- IRQ --> E[driver]
```

设备不能随便绑 IRQ 到某一个 vector 上，不然驱动那边不知道需要把驱动这边的 IRQ 绑到哪个 vector 上。

中断来时，内核将 vector 转成 IRQ，然后调用这个 IRQ 对应的回调函数，也就是驱动注册的函数。

### Linux 的设计
模块化设计，外设驱动只管外设，外设到外设驱动需要中断？中断是由中断设备（APIC）提供的，那就新增一个中断驱动（irq）。由中断驱动提供中断设备的细节。外设驱动不需要配置中断设备的东西。

分层设计：

| 层  | 硬件                    | 软件                                     |
|-----|-------------------------|------------------------------------------|
| 1   | APIC 设备               | irq 驱动                                 |
| 1→2 | APIC 对外提供固定的 pin | irq 对外提供 irq_desc 结构，用于外设注册 |
| 2   | 外设                    | 外设驱动往 irq_desc 注册                 |

中断来时，irq 驱动将 vector 转成 IRQ，然后调用这个 IRQ 对应的回调函数。我们可以将外设驱动注册到这个回调函数上，但考虑到共享中断，以及该中断设备仍然需要一些自己的操作（例如 level trigger 的 eoi），所以，内核的这个回调函数不是外设驱动的回调，而是 APIC 设备驱动（irq）自己的回调函数。具体来说就是 `irq_desc->handle_irq` 对应 irq 的回调，针对 APIC 设备的，和外设驱动无关。irq 的这个 `handle_irq` 回调，会去调用驱动注册的回调函数，位于 `irq_desc->action` 链表上的 `handler`。

<!--
另一个角度的看法是：irq 其实代表的就是 IRQ line 的，只是中间换了套协议（vector），也就是说内核将 irq 抽象出来供驱动使用。
-->

[GPIO 和中断](./gpio_and_interrupt.md)

```c
// include/asm-x86/hw_irq_64.h
#define IRQ0_VECTOR		(FIRST_EXTERNAL_VECTOR + 0x10)
// arch/x86/kernel/i8259_64.c
DEFINE_PER_CPU(vector_irq_t, vector_irq) = {
	[0 ... IRQ0_VECTOR - 1] = -1,
	[IRQ0_VECTOR] = 0,
	//...
};
```

```c
// include/linux/irq.h
struct irq_desc {
	//...
	irq_flow_handler_t	handle_irq;
	//...
	struct irqaction	*action;	/* IRQ action list */
	//...
};

// kernel/irq/handle.c
struct irq_desc irq_desc[NR_IRQS] __cacheline_aligned_in_smp = {
	[0 ... NR_IRQS-1] = { }
};
```

我们可以清楚地看到，这里有两层数据结构：

1. vector 转 irq，
2. 下一层的设计不是 irq 直接对函数指针，而是 irq 对 `struct irq_desc`，简单来说，它就是回调函数的集合

**request_irq 是绑定 irq 和函数指针，而不是绑定 irq 和 vector**。绑定关系是放在 `irq_desc` 中


```c
do_IRQ(...);
    vector = ~regs->orig_rax;
    /*
     * vector → irq 早写好的，不是在 request_irq 时写的
     */
    irq = __get_cpu_var(vector_irq)[vector];
    generic_handle_irq(irq);
        /*
         * 这里的 desc 描述的是 IRQ line， one irq_desc per irq
         * 存着这个 irq 对应的所有回调函数，位于 irqaction 中
         */
        struct irq_desc *desc = irq_desc + irq;
        desc->handle_irq(irq, desc); // handle_edge_irq
            /*
             * 这里的 irqaction 才是 request_irq 注册上去的链表
             */
            struct irqaction *action = desc->action;
            handle_IRQ_event(irq, action);
                do {
                    ret = action->handler(irq, action->dev_id);
                    // ...
                    action = action->next;
                } while (action);
```

## 注册的数据结构
这里涉及的数据结构很多：

* 上篇文章介绍的 `idt_table` 和 `idt_descr`
* `irq = __get_cpu_var(vector_irq)[vector];`
* `struct irq_desc *desc = irq_desc + irq;`

### `idt_descr`/`idt_table`：在全局内存中，制作 idt table；在 idtr 中记录 idt table 的地址
1. CPU 跳转位置由 load_idt 通过 ldit 指令配置 idtr，给出 Linux kernel 的处理函数表地址
2. Linux 处理函数表的更新由 `set_intr_gate` 负责

正常情况下，大部分的中断函数的入口是 `IRQ0xXY_interrupt`，该函数用于将 vector 压入栈，但部分 vector 的处理是特殊的，需要覆盖部分。

```c
// arch/x86/kernel/setup64.c 
struct desc_ptr idt_descr = { 256 * 16 - 1, (unsigned long) idt_table };


start_kernel();
  trap_init();
    set_intr_gate(0,&divide_error);
      _set_gate(&idt_table[nr], GATE_INTERRUPT, (unsigned long) func, 0, 0);
        memcpy(...); //
    cpu_init
        load_idt((const struct desc_ptr *)&idt_descr);
          asm volatile("lidt ...");
  init_IRQ();
    set_intr_gate();
      // for vector: FIRST_EXTERNAL_VECTOR, END, i, 0 to external_number
      _set_gate(&idt_table[nr], GATE_INTERRUPT, (unsigned long) interrupt[i], 0, 0);
```

这里在 `trap_init` 后，`idt_descr` 地址就已经存到 idtr 中了，对于外部的 `interrupt` 直接填到后续的地址就是了，不需要再 `lidt`。

多核直接拿来用：

```c
start_secondary();
  cpu_init();
    load_idt((const struct desc_ptr *)&idt_descr);
```

关于 start_secondary，详见 [smp](./smp/_index.md)

### `vector_irq`：在 per CPU data 的内存中，制作 vector → irq 的映射

```c
// init/main.c
start_kernel();
 rest_init();
  kernel_init();
    // arch/x86/kernel/smpboot_64.c, TODO:单核呢？
    smp_prepare_cpus(max_cpus);
      // arch/x86/kernel/io_apic_64.c
      setup_IO_APIC();
        setup_IO_APIC_irqs();
          for (apic = 0; apic < nr_ioapics; apic++)
          for (pin = 0; pin < nr_ioapic_registers[apic]; pin++)
            idx = find_irq_entry(apic,pin,mp_INT); // 遍历 mp_irq_entries, mp_irqs, Multi-Process 来自 ACPI
            irq = pin_2_irq(idx, apic, pin); // 根据 mp_irqs 拿
            add_pin_to_irq(irq, apic, pin);
            setup_IO_APIC_irq(apic, pin, irq, irq_trigger(idx), irq_polarity(idx));
              assign_irq_vector(irq, mask);
                __assign_irq_vector(irq, mask);
                  per_cpu(vector_irq, new_cpu)[vector] = irq; // 更新了 per cpu 区域
                  cfg = &irq_cfg[irq];
                  cfg->vector = vector;
                  cfg->domain = domain;
```

`irq_cfg` 是对于 APIC 状态的描述，用于 start_secondary (cpu_up 会调用它)时，获取 vector 和 irq 的映射关系。避免从其他 CPU 获取或者从 APIC 获取。

* `vector_irq` 是以 vector 为 index 的，快速寻找
* `irq_cfg` 是以 irq 为 index 的，这样可以使得不同的 irq 对应相同编号的 vector

```
  IRQ 100 RTE: { vector=0x41, dest=CPU0|CPU1 }   → 只发给 CPU0 或 CPU1
  IRQ 200 RTE: { vector=0x41, dest=CPU2|CPU3 }   → 只发给 CPU2 或 CPU3
```

```c
struct irq_cfg irq_cfg[NR_IRQS] __read_mostly = {
	[0]  = { .domain = CPU_MASK_ALL, .vector = IRQ0_VECTOR,  },
    //...
}
```


```c
// arch/x86/kernel/smpboot_64.c
start_secondary();
  cpu_init();
    load_idt((const struct desc_ptr *)&idt_descr);
  __setup_vector_irq(smp_processor_id());
    for (irq = 0; irq < NR_IRQS; ++irq) {
        if (!cpu_isset(cpu, irq_cfg[irq].domain)) // 这个 irq 不需要绑到我这个 core 上
            continue;
        vector = irq_cfg[irq].vector; // internal CPU vector/irq
        per_cpu(vector_irq, cpu)[vector] = irq;
    }
```

* 关于 start_secondary，详见 [smp](./smp/_index.md)
* 关于 per_cpu 详见 [cpu var](./cpu_var.md)

### `irq_desc`：在全局内存中，制作 APIC 上的 irq line 的抽象

```c
struct irq_desc irq_desc[NR_IRQS] __cacheline_aligned_in_smp = {
	[0 ... NR_IRQS-1] = {
		.status = IRQ_DISABLED,
		.chip = &no_irq_chip,
		.handle_irq = handle_bad_irq,
		.depth = 1,
		.lock = __SPIN_LOCK_UNLOCKED(irq_desc->lock),
#ifdef CONFIG_SMP
		.affinity = CPU_MASK_ALL
#endif
		// .action;	/* IRQ action list */
	}
};
```

包含两个内容，
* 对 handle_irq 的更新：该 irq line 真正的唯一的回调函数，多和 irq 本身相关
* 对 action 的更新：驱动注册到该 irq_line 的回调函数，和挂在 irq 上的设备相关

更新驱动通过 request_irq 注册的中断处理函数


#### `handle_irq`
对电平信号有不同的处理

[锁存器和触发器](./flip-flop.md)

我们可以将 APIC 的电平触发类比成 SR 锁存器，在于状态的维持与显式清除：

* 对于 SR 锁存器，S 置位后，Q 会持续输出，K 置位后，Q 才会回归 0。
* 对于 APIC 的电平触发，中断来时，中断保持，Linux 回馈 eoi 时，中断终止。

```c
// 上游查看上一节
// arch/x86/kernel/io_apic_64.c
setup_IO_APIC_irq();
  assign_irq_vector(irq, mask); // irq → vector
  ioapic_register_intr(irq, trigger);
```

```c
// arch/x86/kernel/io_apic_64.c
static void ioapic_register_intr(int irq, unsigned long trigger)
{
	if (trigger) {
		irq_desc[irq].status |= IRQ_LEVEL;
		set_irq_chip_and_handler_name(irq, &ioapic_chip,
					      handle_fasteoi_irq, "fasteoi");
	} else {
		irq_desc[irq].status &= ~IRQ_LEVEL;
		set_irq_chip_and_handler_name(irq, &ioapic_chip,
					      handle_edge_irq, "edge");
	}
}
```

```c
// kernel/irq/chip.c
void
set_irq_chip_and_handler_name(unsigned int irq, struct irq_chip *chip,
			      irq_flow_handler_t handle, const char *name)
{
	set_irq_chip(irq, chip);
        desc = irq_desc + irq;
        desc->chip = chip;
	__set_irq_handler(irq, handle, 0, name);
        desc = irq_desc + irq;
        desc->handle_irq = handle;
}
```

##### handle_fasteoi_irq(level trigger)

```c
handle_fasteoi_irq(irq, desc);
  spin_lock(&desc->lock);
  if (unlikely(desc->status & IRQ_INPROGRESS))
      goto out;
  desc->status |= IRQ_INPROGRESS;
  spin_unlock(&desc->lock);
  action_ret = handle_IRQ_event(irq, action);
out:
  desc->chip->eoi(irq); // ack_apic_level,
    irq_complete_move(irq);
      send_IPI_mask(cleanup_mask, IRQ_MOVE_CLEANUP_VECTOR);
    ack_APIC_irq();
      apic_write(APIC_EOI, 0);
        *((volatile unsigned int *)(APIC_BASE+reg)) = v;
```

这里没有关于屏蔽中断的操作，电平触发那不是会一直来中断，从而会一直调用 `handle_fasteoi_irq`？事实上，APIC 有两个：

* IOAPIC(IO APIC) 外设 APIC，作用：
    * 外设的电平触发给到 LAPIC 只会是一个
    * 多核间中断均衡
* LAPIC(Local APIC)
    * per_cpu

```
IO-APIC  pin asserted (high)
    │
    ├─ Remote IRR == 0 → 发一条消息给Local APIC，并置 Remote IRR = 1
    │
    │   ... 外设对 IO-APIC 的电平一直保持 high ...
    │   ... IO-APIC 不再给 L-APIC 发任何消息 ...
    │
    ├─ IO-APIC 收到 EOI → Remote IRR = 0
    │
    └─ pin 还是 high? → 再来一轮
```

也就是所谓的电平触发在 Local APIC 这里还是 spark trigger，并非是持续性地 assert interrupt line，不然 CPU 也不能执行下去了。

##### handle_edge_irq(edge trigger)

```c
handle_edge_irq(irq, desc);
  action_ret = handle_IRQ_event(irq, action);
```

#### `action`

```c
int request_irq(unsigned int irq, irq_handler_t handler,
		unsigned long irqflags, const char *devname, void *dev_id)
{
    // ...
	retval = setup_irq(irq, action);
    // ...
}
int setup_irq(unsigned int irq, struct irqaction *new){
    // ...

		/* add new interrupt at end of irq queue */
		p = &desc->action;
		old = *p;
		do {
			p = &old->next;
			old = *p;
		} while (old);
    // ...
}
```

可以看到 `request_irq` 只是将 irq_handler_t 挂到早就已经做好的 `irq_desc`

### summary
1. 中断来时，去全局区（`idt_descr`(`idt_table`)）将 vector 压入栈
2. do_IRQ，去 Per CPU 区（`vector_irq`）将 vector 转成 irq
3. do_IRQ，去全局区（`irq_desc`）找到 irq 对应的 action

### Q&A
#### 为什么 `idt_descr` 和 `irq_desc` 是全局的，但是 vector_irq 不是
* `idt_descr` 全局大概是因为有很多内部的 vector 例如除以 0 错误，`IRQ0xXY_interrupt` 都是通用的，没必要重建
* 然后在 vector_irq 这里分流，不同的核，对于同样的 vector 会对应不同的 irq
    * `vector_irq` 是以 vector 为 index 的，快速寻找
    * `irq_cfg` 是以 irq 为 index 的，这样可以使得不同的 irq 相同编号的 vector
* `irq_desc` 是全局的，所有的核共同编号 irq number 取决于 APIC 不是 CPU。一个 irq 对应一个功能（可以有多个，对应多个 irq_action，但所有核都是多个）。irq_action 所在的 irq_desc 是根据 irq
