---
title: "qt creator remote deploy and debug"
date: 2023-06-13T06:27:03+08:00
lastMod: 2023-06-13T06:27:03+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "remote gdb"
tags: ["qt", "gdb"]
---

## machines

* target (aarch64)
* host (AMD64)

cross compile on AMD64, run on aarch64

## softwares
### on target
* basic: gdbserver
* others: known from qt creator

### on host
* basic: gdb-multiarch

## config
### pro

```config
target.path=/tmp
INSTALLS += target
```

### kits
1. add debugger(gdb-multiarch)
2. config the kit with above debugger

