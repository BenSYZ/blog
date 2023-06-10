---
title: "gcc support __FILE_NAME__ macro"
date: 2023-06-02T07:48:40+08:00
lastMod: 2023-06-06T05:55:00+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "gcc_support_filename_macro"
tags: ["gcc", "macro", "c"]
---

## 2023-06-06 update:
windows does not support shell basename. Makefile native support `basname` like, it's `notdir`.

## contents

Since gcc-12, gcc native support `__FILE_NAME__` macro. [ref](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42579)

Here we provide some methods to make gcc version lower than 12 support `__FILE_NAME__`:

Makefile:

```make
CXX_FLAGS+=-D__FILE_NAME__=\"$(notdir $<)\"

test: test.c
	gcc-7 $(CXX_FLAGS) $<
```

Qt pro:

```config
DEFINES += '__FILE_NAME__=\\\"$(notdir $<)\\\"'
```

Some information of macro is at [here](../macro_in_gcc_Makefile_qmake).
