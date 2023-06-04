---
title: "macro set in gcc, Makefile, qmake"
date: 2023-06-02T07:44:40+08:00
lastMod: 2023-06-02T07:44:40+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "qmake_set_macro"
tag: ["macro", "Makefile", "gcc", "qt"]
---

## gcc
```sh
gcc -DHello=\"hey\" main.c
```

## Makefile
```make
CFLAGS = -DHello=\"hey\"
```

## qmake
```sh
qmake 'DEFINES += Hello=\\\"abc\\\"' test_macro.pro
```

```config
DEFINES += Hello=\\\"abc\\\"
```

关于 `\\\"` 的解释

* `\\` 输出makefile里的`\`
* `\"` 输出makefile里的`"`

