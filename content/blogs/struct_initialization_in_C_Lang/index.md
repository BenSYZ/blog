---
title: "struct initialization in C Lang"
date: 2023-05-20T17:49:48+08:00
lastMod: 2024-10-05T13:17:10+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "看内核源码关于任务初始化的时候看到一个很奇怪的写法：`struct task_struct init_task = INIT_TASK(init_task);` ..."
tags: ["Linux kernel", "C-Lang", "struct"]
---


## Introduction

看内核源码关于任务初始化的时候看到一个很奇怪的写法：`struct task_struct
init_task = INIT_TASK(init_task);`，一个没被定义变量怎么可以传给一个函数，用来
初始化它自己。

我们把原始代码用一下这个 demo 展示：

```c
struct myStruct {
  struct myStruct *self;
  int a;
  int b;
};

#define INIT_TASK(tsk){\
    .self = &tsk,      \
    .a = 0,            \
    .b = 1,            \
}

int main() {
    struct myStruct init_task = INIT_TASK(init_task);
    /*
     * 展开后：
     * struct myStruct init_task = { .self = &init_task, .a = 0, .b = 1, };
     */
    return 0;
}
```

## What's this?
1. `INIT_TASK` 是一个宏，由于宏是在编译时展开，所以 `init_task` 其实并不是作为
   参数被传入的，而是在这里展开。同时 `INIT_TASK` 只用了 `init_task` 的地址，而
   并没有用 `init_task` 这个结构体本身。
2. `{.self = &tsk, .a = 0, .b = 1,}` 是什么？结构体的初始化。


### 何时分配 `init_task` 的地址
那 `init_task` 的地址是在什么时候分配的呢？这个结构体的赋值有些复杂，因为涉及到
取址。我们看一个简单的例子：

```c
int main(){
    int a = 0;
    return 0;
}
```

```asm
push   %rbp
mov    %rsp,%rbp
movl   $0x0,-0x4(%rbp)
mov    $0x0,%eax
pop    %rbp
```

```gdb
(gdb) p &a
$1 = (int *) 0x7fffffffd5cc
(gdb) p $rbp
$2 = (void *) 0x7fffffffd5d0
# 基地址 rbp: 0x7fffffffd5d0
# movl   $0x0,-0x4(%rbp) 将 0x0 赋值给 rbp-4 也就是 0x7fffffffd5cc
```

所以这些栈上的变量的地址是基于 `rbp` 的，也就是说相对地址是由汇编代码决定的，而
汇编代码来自 C 的代码。也就是这份代码结构决定了变量 a 的地址的存在。

#### 验证：是否可以用函数来替代宏
既然我们说，宏（`INIT_TASK`）只使用了 `init_task` 的地址，而 `init_task` 的地址
是由这份代码结构决定的。也就是说看起来传参传的是一个未定的变量（`init_task`），
其实传的是一个固定的值（`&init_task`）。那这完全符合函数的操作。

```c
struct myStruct {
  struct myStruct *self;
  int a;
  int b;
};

struct myStruct FUNC_INIT_TASK(struct myStruct * tsk){
    tsk->self = tsk;
    tsk->a=0;
    tsk->b=0;
    return *tsk;
}

int main() {
    struct myStruct func_init_task = FUNC_INIT_TASK(&func_init_task);
    return 0;
}
```

### 结构体内部用结构体本身？
我们把 `&init_task` 赋给了 `init_task.self`，看起来有点像“鸡生蛋，蛋生鸡”的问题，
其实不然，就像上节中说的，`&init_task` 和 `init_task` 是两回事，对 `&init_task`
我们只是借用了 `init_task` 来取到这个地址。来看一个真正的“鸡生蛋，蛋生鸡”的代码：

```c
struct myStruct {
  struct myStruct self; // field has incomplete type 'struct myStruct' (ccls 2)
  int a;
  int b;
};

#define INIT_TASK(tsk){\
    .self = tsk,       \
    .a = 0,            \
    .b = 1,            \
}

int main() {
    struct myStruct init_task = INIT_TASK(init_task);
    return 0;
}
```

我们会发现，linter 直接在定义结构体的时候就报错了，因为它不知道 `myStruct` 有多
大，或者说这种定义本身就是一种矛盾：外部的 `myStruct` 一定比内部的 `myStruct`
大两个 `int` 的大小。

其实这个看似“鸡生蛋，蛋生鸡”的问题我们在数据结构中写链表的时候就用到了。

另外，对 64 bit 的 Linux 来说，这个 `struct myStruct *self` 的本质其实是 `long`。
关于各个变量的长度，有机会再聊聊，Linux 的 long 和 Windows 的 long 是不一样的。
（TODO）
