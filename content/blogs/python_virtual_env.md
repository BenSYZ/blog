---
title: "python virtual env and venv"
date: 2023-06-19T06:59:23+08:00
lastMod: 2023-06-19T06:59:23+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "如何轻量化使用使用不同版本 python"
tags: ["python", "conda"]
---

pre introduction : stackoverflow [what-is-the-difference-between-venv-pyvenv-pyenv-virtualenv-virtualenvwrappe](https://stackoverflow.com/a/41573588/13033234)

原先位于 Tips write date: 2021-04-21T00:00:00+08:00

## intro
`anaconda` 和 `miniconda` 是很常用的多版本 python 的管理器，不过我有几点不满意

* 过于庞大，不够轻量，即使是 miniconda 好像也有 好几百兆
* 而且 conda channel 什么的整了好久都没整明白，
* 已经有 pip 了为何还要用 conda install

当然 conda 还提供了一些非 python 的内容，这是 pip 无法做到的，但是这又和 kiss 原则相违背了。

## softwares

1. `pyenv`: provide python version
2. `venv`: provide virtual environment

## Working flow example

### Configure

1. install another version of python

2023-06-19 update:

if you want to generate `libpython3.so`

```sh
env CONFIGURE_OPTS="--enable-shared"  pyenv install 3.10
```

2023-05-26 update:

```sh
pyenv install --patch 3.7.9 << alignment.patch
```

[alignment.patch src](https://github.com/pyenv/pyenv-virtualenv/issues/410)

```diff
--- Include/objimpl.h
+++ Include/objimpl.h
@@ -250,7 +250,7 @@
         union _gc_head *gc_prev;
         Py_ssize_t gc_refs;
     } gc;
-    double dummy;  /* force worst-case alignment */
+    long double dummy;  /* force worst-case alignment */
 } PyGC_Head;

 extern PyGC_Head *_PyGC_generation0;
--- Objects/obmalloc.c
+++ Objects/obmalloc.c
@@ -643,8 +643,8 @@
  *
  * You shouldn't change this unless you know what you are doing.
  */
-#define ALIGNMENT               8               /* must be 2^N */
-#define ALIGNMENT_SHIFT         3
+#define ALIGNMENT               16               /* must be 2^N */
+#define ALIGNMENT_SHIFT         4

 /* Return the number of bytes in size class I, as a uint. */
 #define INDEX2SIZE(I) (((uint)(I) + 1) << ALIGNMENT_SHIFT)
```

2. Add that python to `$PATH`

```sh
# default python be python 3.7.9
export PYENV_ROOT="$HOME/.pyenv/versions/3.7.9/"
export PATH="$PYENV_ROOT/bin:$PATH"
```

3. Create virtual environment, there will be python command in `.venv/3.6/bin`, but it is a link to current `python`.

```sh
# create virtual environment
cd ~/.venv
python -m venv 3.7
```

4. Active and deactivate it

```sh
source ~/.venv/3.7/bin/activate
deactivate
```

`export` in the 2nd is no needed this time.

### Reuse
Active and deactivate it

```sh
source ~/.venv/3.6/bin/activate
deactivate
```

you can check if it's enalbed by:
```
which python3
which pip3
```

### others
Set pyenv as default, not recommend:

```sh
# .zshrc
# pyenv
export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
if command -v pyenv 1>/dev/null 2>&1; then
  eval "$(pyenv init -)"
fi
```
