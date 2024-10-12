---
title: "Introduction to precompiled headers(PCH)"
date: 2024-10-08T12:04:22+08:00
lastMod: 2024-10-12T06:06:00+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "PCH 是一种加速编译的方式，本文介绍了什么是 PCH，以及如何使用 PCH"
tags: ["PCH", "GCC", "compile"]
---

## Introduction
在讲预编译头文件 PCH 的之前，我们简单回顾一下编译的流程，编译大致有以下几个步骤：

1. 预处理：将源代码中的宏、头文件等展开
2. 编译：将源代码转成汇编代码
3. 汇编：将汇编代码转成 object 文件
3. 链接：生成可执行文件

如果有一个头文件，内容特别多，而且在好多源代码中都被引入了，那是不是会导致：每一个源代码文件在编译的时候都要展开一大段，然后在编译每一个源代码文件的时候都要重新编译这一大段，最终导致许多重复展开重复编译。那么我们是否可以将这种重复动作优化一下呢？让他只做一次，然后下次谁要用，就直接去取就完了？这就是“预编译头文件（precompiled header, PCH）” 最初的想法。

## What is PCH?
这是 [Wikipedia 上对 PCH 的介绍](https://zh.wikipedia.org/zh-cn/预编译头):

> 预编译头（precompiled header）是程序设计时把头文件编译为中间格式（如目标文件），以节约在开发过程中编译器反复编译该头文件的开销。 C 语言、C++语言、Objective C 语言等都有类似的技术。

那这个中间格式实际又是什么呢？我们在下节 “如何生成 PCH 文件” 中会来实际看看它是什么文件。

## 如何生成 PCH 文件？
非常简单，和编译普通的源文件一样，只需对头文件做编译即可：

```sh
gcc blah.h
```

Tips:
    * `gcc`/`g++` 会根据扩展名来选择默认的编译选项，对 `h` 文件，会默认使用 `-x c-header`，我们也可以强制将一个 `c` 文件用 `-x c-header` 来编译，编译成 PCH （具体请 `man 1 gcc` 搜索 `-x` 参数）

我们会发现，经过上一条命令的执行，我们得到了 `blah.h.gch` 让我们来看看这个文件是什么：

```console
$ file blah.h.gch
blah.h.gch: GCC precompiled header (version 014) for C
```

好吧，哈哈，PCH 就是 PCH，它就是单独的一个文件类型，和 `.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped` 文件是不一样的。

## 如何使用 PCH 文件？
那既然 PCH 是一个单独的文件类型，那我们要怎么使用它呢？我们知道，头文件可以用 `#include` 来引入，object 是在链接期间作为输入文件引入的。那预编译头文件呢？

这就要找 [GCC 的官方文档](https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html) 了。文中提到，要使用 PCH 文件，你啥都不需做，就正常 `include` 原始的头文件就好，`gcc`/`g++` 在编译时，搜索头文件的时候会优先搜索同名带 `.gch` 后缀的预编译头文件。另外这个带 `.gch` 后缀的文件也可以是文件夹，里面可以放多个预编译头文件文件，`gcc`/`g++` 在编译时，会去这个目录下找任意一个可以用的预编译头文件。可以这么来理解，头文件有两种形式：文本形式和 PCH。我们来尝试使用一下预编译头文件。

### 当 `.gch` 是文件时
#### 修改原始头文件，添加 `#error` 宏
GCC 的官方文档，给出的验证方式是
1. 编译 PCH 文件
2. 修改原始头文件，添加 `#error` 宏
3. C 源文件中 `include` 正常引入该头文件
4. 编译，查看是会报 error 还是编译通过

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H
#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```console
$ gcc blah.h
$ echo '#error "PCH is not been used"' >> blah.h # blah.h 中添加 #error 宏
$ gcc blah.c
```

我们会发现，它确实直接编译通过了，这就说明预编译头文件生效了。当然这里引入一个新的问题，我需要使用 Makefile 或是其他方式，来更新预编译头文件，来避免头文件和预编译头文件不对应。

#### 使用 `gcc` 的 `-H` 参数

[stackoverflow 上的这个回答](https://stackoverflow.com/a/18593344/13033234) 介绍了一种更直接明了的方式：GCC 有一个 `-H` 参数，可以用它打印实际引用的头文件或预编译头文件。查询 GCC 的 Man pages，我们可以看到：

> -H Print the name of each header file used, in addition to other normal activities. Each name is indented to show how deep in the #include stack it is. Precompiled header files are also printed, even if they are found to be invalid; an invalid precompiled header file is printed with ...x and a valid one with ...! .

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H
#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```console
$ gcc    blah.h
$ gcc -H blah.c
! blah.h.gch
 blah.c
```

### 当 `.gch` 是文件夹时

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H
#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```


```console
$ mkdir blah.h.gch
$ gcc    blah.h -o  blah.h.gch/c
$ gcc -H blah.c
! blah.h.gch/c
 blah.c
```

也是可以正常使用的。

### 在 Qt 中使用 PCH

[Qt](https://doc.qt.io/qt-6/qmake-precompiledheaders.html) 中只需在 `.pro` 中加入以下两行，并在 `stable.h` 中放入你想预编译的头文件，Qt 对单个项目目前只支持一个全局的预编译头文件。

```pro
CONFIG   += precompile_header
PRECOMPILED_HEADER = stable.h
```

然后 `qmake` 会自动生成对应的 `Makefile`，完成以下几件事：
1. 递归地找到所有依赖的头文件，将它们放在 PCH 的 prerequisite，来做到当头文件更新时，PCH 会更新。
2. 在所有 `.o` 的 target 中通过 `-include` 的方式强制引入 PCH 文件
3. 将 PCH 文件，作为所有 `.o` 的 target 的 prerequisite

* Tips:
    + 需要注意的是，通过上面几条 `Makefile` 的规则，如果任意一个构成 PCH 的头文件变更，都会导致 PCH 更新，从而导致所有 .o 重编，反而使增量编译时间变长。这也是 Qt 问什么称这个文件为 `stable.h` 的原因


## 预编译头文件失效（invalid）
我们前面说到，“会去这个目录下找任意一个可以用的预编译头文件”，那什么时可用的，什么是不可用的呢？

### GCC/G++ 编译器不同
GCC 编译时，不能使用 G++ 编译出来的预编译头文件，反之亦然。例如：

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H
#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```console
$ g++ blah.h -o blah.h.gch
$ gcc -H blah.c
x blah.h.gch
. blah.h
```

### 头文件中的宏（例如 `#ifdef`）引入分支
头文件中有宏（例如 `#ifdef`）引入分支，编译 PCH 时的分支和使用 PCH 时的分支不同

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H

#ifdef blah_1
#define blah 1
#else
#define blah 2
#endif

#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```console
$ gcc -D blah_1 blah.h -o blah.h.gch

$ gcc -H blah.c
x blah.h.gch
. blah.h
```


## 解决失效问题
当然最直接的方式是该怎么样怎么样，GCC 编译就用 GCC 编译 PCH，编译 object 时的分支走哪个编译 PCH 时就走哪个。

但有一个问题：每换一个编译器，或每换一个宏（例如编译配置宏）都要重新编译预编译头文件，又或者需要自己拷来拷去。十分麻烦，有什么方式可以避免手动拷吗？把 `.gch` 改成文件夹，往里面放各种有效的头文件，我们上面说过：GCC 会去这个目录下找任意一个可以用的预编译头文件。也就是说，你可以在这个目录下放多版不同情况下的预编译头文件，让 GCC 自动找寻合适的预编译头文件，

### GCC/G++ 编译器不同
我们可以同时放 GCC/G++ 的预编译头文件，让 GCC/G++ 自己选择

```console
$ mkdir           blah.h.gch
$ gcc   blah.h -o blah.h.gch/c
$ g++   blah.h -o blah.h.gch/c++

$ gcc -H blah.c
! blah.h.gch/c
blah.c
$ g++ -H blah.c
x blah.h.gch/c
! blah.h.gch/c++
 blah.c
```


### 头文件中有宏（例如 `#ifdef`）引入分支

```c
// blah.h
#ifdef blah_1
#define blah 1
#else
#define blah 2
#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```console
$ gcc           blah.h -o blah.h.gch/c1
$ gcc -D blah_1 blah.h -o blah.h.gch/c2

$ gcc -H blah.c
! blah.h.gch/c1
 blah.c

$ gcc -D blah_1 -H blah.c
x blah.h.gch/c1
! blah.h.gch/c2
 blah.c
```


## 常见的错误
除了上述说的 `gcc -H` 和 `#error`，我们也可以使用 `-Winvalid-pch` 来让 GCC 吐出 PCH 失效问题的详细信息。
### `blah.h.gch: not for GNU C17 `
即上面说的 GCC 不能使用 G++ 编译的 PCH。

### `blah.h.gch: not used because xxx not defined`
即上面说的编译 PCH 和使用 PCH 时宏分支不同。

### `cc1plus: error: one or more PCH files were found, but they were invalid`
这种情况出现在使用 `-include` 强制引入头文件，当这个头文件只有 PCH 的形式而没有文本形式时，便会报这个错，和这个错在一起的还有
```
<command-line>: fatal error: test_pch: No such file or directory
```

```c
// blah.h
#ifndef BLAH_H
#define BLAH_H

#define blah 1

#endif
```

```c
// blah.c
#include "blah.h"
int main(){
	return 0;
}
```

```make
pch_fake_header = test_pch
pch_dir         = $(pch_fake_header).gch

old_external_macro = old
new_external_macro = new

target = main

help:
	printf "Use the two commands to test the two cases:\nmake demo_1\nmake demo_2\n"

demo_1: clean
	echo "macro changes"
	mkdir -p $(pch_dir)
	gcc -D external_macro=\"$(old_external_macro)\"                             -o $(pch_dir)/c blah.h
	gcc -D external_macro=\"$(new_external_macro)\" -include $(pch_fake_header) -o $(target)    main.c

demo_2: clean
	echo "gch generated with gcc"
	mkdir -p $(pch_dir)
	g++ -D external_macro=\"$(old_external_macro)\"                             -o $(pch_dir)/c blah.h
	gcc -D external_macro=\"$(new_external_macro)\" -include $(pch_fake_header) -o $(target)    main.c



main: pch
	gcc -include $(pch_fake_header) -D external_macro=\"$(new_external_macro)\" -o $(target) main.c

pch:
	mkdir -p $(pch_dir)
	gcc -D external_macro=\"$(old_external_macro)\" -o $(pch_dir)/c blah.h

clean:
	[ -e $(pch_dir) ] && rm -r $(pch_dir) || true
	rm -f $(target)
```

### `error: stray xxx in program`
这种情况一样出现在使用 `-include` 强制引入头文件的情况，当这个头文件只有 PCH 的形式而没有文本形式时，但上一次编译的 ELF 文件生成了，生成的名称正好是强制引入的头文件的头文件，便会报这个错，原因是，这次直接 include 了二进制文件。这种情况在 Qt 中出现过，Qt 配置 PCH 后，会将 PCH 文件放在 `TARGET.gch` 文件夹中，而该文件夹的目录和 `TARGET` 同级。

### PCH 有效但是并未使用
从 GCC 文档中看到，如果某个头文件要使用 PCH 的形式，必须在整个文件中第一行 C 代码前。而最常见的 `stdio.h` 中有 C 代码，所以，如果我们将自己的头文件放在 `stdio.h` 之后，GCC 是不会使用 PCH 的。
