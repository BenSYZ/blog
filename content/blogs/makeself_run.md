---
title: "makeself 和 .run 文件"
date: 2023-06-12T11:05:23+08:00
lastMod: 2023-06-12T11:05:23+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "what is .run file"
tags: ["makeself", "run", "sh"]
---

## 什么是 makeself 以及 .run 文件是什么

简单来说，makeself 把一些文件打成压缩包，然后把这个压缩包打包成 POSIX sh 脚本。

很多巨大 `xxx.run` 文件或者 `.sh` 就是 `makself` 打出来的，它不是简单的一个 shell 脚本，它自包含了压缩包。(当然 Linux 中扩展名无关紧要，只是 .run 通常是)`NVIDIA-Linux-xxx.run` 就是用这个打出来的。印象当中 `Mathematica` 的 `Mathematica_13.2.1_LINUX.sh` 好像也是这个打出来的。

### 如何使用 makeself

## 如何打包：
如下目录结构

```tree
.
└── dir
    ├── file1
    ├── file2
    └── install.sh
```

```sh
makeself dir test_makeself.run v0.1 ./install.sh
```

就会在当前目录生成 `./test_makeself.run`

更多参数详见 man.

## 如何查看包

**警告**：这个脚本是很随意的，所以下面的参数只是通常情况下可能支持的参数。请一定要确保脚本是来自于可信源，否则有可能你只是想查看帮助，实际上执行了恶意程序。

### 帮助
```
./test_makeself.run --help
```

### 查看解包后执行的脚本和参数

```console
  ➜  ./test_makeself.run --info
Identification: v0.1
Target directory: dir
Uncompressed size: 4 KB
Compression: gzip
Encryption: n
Date of packaging: Mon Jun 12 19:23:34 CST 2023
Built with Makeself version 2.5.0
Build command was: /usr/bin/makeself \
    "dir" \
    "test_makeself.run" \
    "v0.1" \
    "./install.sh"
Script run after extraction:
     ./install.sh
dir will be removed after extraction
```

### 解包

```sh
./test_makeself.run --noexec --target extract_dir
```

```tree
extract_dir
├── file1
├── file2
└── install.sh
```

