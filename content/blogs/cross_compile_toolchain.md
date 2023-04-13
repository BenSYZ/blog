---
title: "cross_compile_toolchain"
date: 2023-04-11T06:53:21+08:00
lastMod: 2023-04-11T06:53:21+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "cross_compile_toolchain"
tags: ["cross_compile_toolchain"]
---

从 linaro 下载 [gcc cross build 5.4.1](https://releases.linaro.org/components/toolchain/binaries/5.4-2017.01/aarch64-linux-gnu/gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu.tar.xz)，解压之后在 Ubuntu 18.04 可以直接运行，在 centos6 上报这个错：

```
/opt/gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc: /lib64/libc.so.6: version `GLIBC_2.14' not found (required by /opt/gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
```

查阅得 centos6 自带 glibc 版本太老，需要升级。因此下载 [glibc-2.14](http://mirrors.aliyun.com/gnu/libc/glibc-2.14.tar.xz)，并编译

```
mkdir build
../configure --prefix="/opt/glibc-2.14"
make -j32
sudo make install
```

```
env LD_LIBRARY_PATH=/opt/glibc-2.14/lib/ /opt/gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc -v
```

可以了。

提供给别人时：可以把这两个放一起

```
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/include
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/share
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/lib
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/gcc-linaro-5.4.1-2017.01-linux-manifest.txt
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/bin
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/aarch64-linux-gnu
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/libexec
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/opt/glibc-2.14/
gcc-linaro-5.4.1-2017.01-x86_64_aarch64-linux-gnu_with_glibc-2.14/activate
```

参考 py-venv

active:

```bash
# This file must be used with "source bin/activate" *from bash*
# you cannot run it directly

deactivate () {
    # reset old environment variables
    if [ -n "${_OLD_VIRTUAL_PATH:-}" ] ; then
        PATH="${_OLD_VIRTUAL_PATH:-}"
        export PATH
        unset _OLD_VIRTUAL_PATH
    fi
    if [ -n "${_OLD_LD_LIBRARY_PATH:-}" ] ; then
        LD_LIBRARY_PATH="${_OLD_LD_LIBRARY_PATH:-}"
        export LD_LIBRARY_PATH
        unset _OLD_LD_LIBRARAY_PATH
    fi
    if [ -n "${_OLD_VIRTUAL_PYTHONHOME:-}" ] ; then
        PYTHONHOME="${_OLD_VIRTUAL_PYTHONHOME:-}"
        export PYTHONHOME
        unset _OLD_VIRTUAL_PYTHONHOME
    fi

    # This should detect bash and zsh, which have a hash command that must
    # be called to get it to forget past commands.  Without forgetting
    # past commands the $PATH changes we made may not be respected
    if [ -n "${BASH:-}" -o -n "${ZSH_VERSION:-}" ] ; then
        hash -r 2> /dev/null
    fi

    if [ -n "${_OLD_VIRTUAL_PS1:-}" ] ; then
        PS1="${_OLD_VIRTUAL_PS1:-}"
        export PS1
        unset _OLD_VIRTUAL_PS1
    fi

    unset VIRTUAL_ENV
    if [ ! "${1:-}" = "nondestructive" ] ; then
    # Self destruct!
        unset -f deactivate
    fi
}

# unset irrelevant variables
deactivate nondestructive

VIRTUAL_ENV="$(dirname $(readlink -f ${BASH_SOURCE[0]}))"
export VIRTUAL_ENV

_OLD_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
LD_LIBRARY_PATH="$VIRTUAL_ENV/opt/glibc-2.14/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH

_OLD_VIRTUAL_PATH="$PATH"
PATH="$VIRTUAL_ENV/bin:$PATH"
export PATH

# unset PYTHONHOME if set
# this will fail if PYTHONHOME is set to the empty string (which is bad anyway)
# could use `if (set -u; : $PYTHONHOME) ;` in bash
if [ -n "${PYTHONHOME:-}" ] ; then
    _OLD_VIRTUAL_PYTHONHOME="${PYTHONHOME:-}"
    unset PYTHONHOME
fi

if [ -z "${VIRTUAL_ENV_DISABLE_PROMPT:-}" ] ; then
    _OLD_VIRTUAL_PS1="${PS1:-}"
    PS1="(aarch64-linux-gcc-5.4.1) ${PS1:-}"
    export PS1
fi

# This should detect bash and zsh, which have a hash command that must
# be called to get it to forget past commands.  Without forgetting
# past commands the $PATH changes we made may not be respected
if [ -n "${BASH:-}" -o -n "${ZSH_VERSION:-}" ] ; then
    hash -r 2> /dev/null
fi
```

很奇怪的是这种方式（LD_LIBRARY_PATH 指向新的 glibc ）在 ubuntu 上会报 segmentation fault，查到[这个](https://stackoverflow.com/questions/847179/multiple-glibc-libraries-on-a-single-host/851229#851229)，但是这里的问题是为什么还是找不到，可是我这里找到了，而且生效了，但是报 segmentation fault 了。

> The absolute path to ld-linux.so.2 is hard-coded into the executable at link time, and can not be easily changed after the link is done

1. 改 linker (ld-linux.so)
2. 改 rpath


## 新编译
```sh
g++ main.o -o myapp ... \
   -Wl,--rpath=/path/to/newglibc \
   -Wl,--dynamic-linker=/path/to/newglibc/ld-linux.so.2
```

## 修改已用的

```
patchelf --set-interpreter /path/to/newglibc/ld-linux.so.2 --set-rpath /path/to/newglibc/ myapp
```

关于 PRE_LOADER
> The LD_PRELOAD environment variable is handled by the dynamic linker/loader (ld-linux.so), and affects the resolution of relocation records in the executable binary itself. In
https://stackoverflow.com/questions/37715245/how-to-check-if-a-linux-shared-library-has-been-preloaded-using-ld-preload

LD_PRELOAD (not LD_PRELOAD_PATH) is a list of specific libraries (files) to be loaded before any other libraries, whether the program wants it or not. LD_LIBRARY_PATH is a list of directories to search when loading libraries that would have been loaded anyway. On linux you can read man ld.so for more information about these and other environment variables that affect the dynamic linker.

https://stackoverflow.com/questions/14715175/what-is-the-difference-between-ld-preload-path-and-ld-library-path

LD_DEBUG=all

TODO!!!
