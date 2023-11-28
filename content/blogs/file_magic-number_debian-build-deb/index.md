---
title: "file、magic number 和 Debian 中 file 的跨大版本更新"
date: 2023-11-28T12:06:47+08:00
lastMod: 2023-11-28T16:56:18+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "debian_install_package_with_newer_version"
tags: ["debian_install_package_with_newer_version"]
description: "feh 没有认出一种 bmp 的格式，发现是 file 版本太旧，从而通过 apt source 安装高版本的 file"
tags: [ "file", "magic number", "feh", "debian", "deb", "apt", "gdb" ]
---


## Introduction

故事会，哈哈，可以跳过，简单来说就是 `feh` 没有认出一种 `bmp` 的格式。

我们在 3588 上使用了 RK 提供的 Debian 11 系统。我们使用 `eog` 也就是 gnome 带的图形查看软件，但是遇到了一个问题，如果我用 `eog` 打开一张图后，又去更新这张图，`eog` 是不会自动刷新的，它没有接入 `libnotify`。于是我从 `eog` 迁移到了 `feh`，由于 Debian 11 自带的 `feh` 版本过低只有 `3.6.3`，这版的 `feh` 也一样不支持 `libnotify`，于是我就尝试自己编译并打包 `feh-3.10`，打包是基于 arch 的 `PKGBUILD`，很是折腾，最后还是成功编译。最近遇到了一个问题：一张 `bmp` 的图片，`feh` 打不开，给我包这个错：

```log
feh WARNING: T002-001.bmp - Does not look like an image (magic bytes missing)
feh: No loadable images specified.
See 'feh --help' or 'man feh' for detailed usage information
```

然后我用 `imagemagick` 转了一下，`feh` 就能正常打开了。

```
convert T002-001.bmp T002-00X.bmp
```

嗯，这是为啥？于是我把两张图片拷到我自己的 arch 上，`feh` 都能打开。于是我用 `file` 查看了两张图，结果是不一样的，那问题就处在这了，看起来两张图的格式不一样。

```console
  ➜  file T002-001.bmp
T002-001.bmp: PC bitmap, Windows 3.x format, 5496 x 3672 x 8, image size 20181312, resolution 3780 x 3780 px/m, 256 important colors, cbSize 20182390, bits offset 1078
  ➜  file T002-00X.bmp
T002-00X.bmp: PC bitmap, Windows 98/2000 and newer format, 5496 x 3672 x 8, cbSize 25606550, bits offset 1162
```

那为啥我在我的 arch 上可以开，在主机的 Debian 上不行呢？一样的打包方式呀？于是同样的我在主机上 `file` 了这两个文件，结果很 amazing 啊，一张图是 `data`，一张图是 `Windows 98/2000 and newer format`，那看起来就是 `file` 没识别出这个 `Windows 3.x`。

```console
$ file T002-001.bmp
T002-001.bmp: data
$ file T002-00X.bmp
T002-00X.bmp: PC bitmap, Windows 98/2000 and newer format, 5496 x 3672 x 8
```

那是不是 `feh` 也是因为一样的问题识别不出来这个文件呢？但是 `eog` 又能正常打开啊。无奈只能各种搜，没搜到。那没办法了只能直接去看 `feh` 的源代码了，看这个报错是什么，惊喜地发现这个 `Waring` 只有一处，代码逻辑也很清楚，还是拿 `c` 写的，甚至 `config.mk` 里 `CFLAGS` 是带着 `-g`！那不直接 `gdb` 走起，拷 `feh` 源码到主机上，然后 `gdb --args feh T002-00X.bmp`：

```c
//src/imlib.c 
int feh_is_image(feh_file * file, int magic_flags)
{
...
	magic_setflags(magic, MAGIC_MIME_TYPE | MAGIC_SYMLINK | magic_flags);
	mime_type = magic_file(magic, file->filename);

	if (!mime_type) {
		return 0;
	}

	if (strncmp(mime_type, "image/", 6) == 0) {
		return 1;
	}
...
}
```


```gdb
(gdb) b feh_is_image
(gdb) layout src
(gdb) r
(gdb) n
...
```

最终发现问题出在这句上

```c
mime_type = magic_file(magic, file->filename);
```

对于那张正常的图，这里的返回值是 `image/bmp`，而对于那张打不开的图，这里的返回值是 `application/octet-stream`

```gdb
(gdb) p mime_type
$4 = 0x55555e0c00 "image/bmp"

(gdb) p mime_type
$2 = 0x55555b3350 "application/octet-stream"
```

ps. 这里有一点搞了我一会儿的是在 next 跳过赋值之后查看 `mime_type` 的值是 `0x0`，两张图都是，我直接好家伙，然后各种 `return` 让我一下摸不清头脑，有时是 `0`(`strncmp`) 是想要的，有时是 `1`(`feh_is_image`) 是想要的。后面是又往后跳了一下，应该是 return 回之前，print 了一下在发现哦这两个值不一样。这个应该和 `O1` 优化有关。

搜了一下 `application/octet-stream` 是啥。

> A MIME attachment with the content type application/octet-stream is a binary file.

[ref_link](https://docs.oracle.com/en/cloud/paas/integration-cloud/rest-adapter/support-application-octet-stream-mime-attachment-binary-payloads.html)

好家伙，这不就是 `file` 返回给我的那个 `data` 吗？没吊用。

然后我在 arch 上一跳转 `magic_file`，直接跳到这里了`/usr/include/magic.h`，`pacman` 的结果直接把我搞无语了：

```console
  ➜  pacman -Qo /usr/include/magic.h
/usr/include/magic.h is owned by file 5.45-1
```

啊这，找了半天原来一早 `file` 就告诉我了。原来 `feh` 用了 `file` 的库哇。我一开始还以为像这种软件不会像 shell 一样调用其他软件，没想到啊。当然 `feh` 没有直接使用 `file` 而是使用了 `file` 的库。或许这就是 "kiss" 的魅力吧。


## file 和 magic number

`file` 这个命令再熟悉不过了，在刚学 Linux 的时候我们就被告知扩展名在 Linux 上只是给人看的。文件真正的格式要用 `file` 来查看。但是 `file` 是如何实现的呢？从 [File 的 wiki](https://en.wikipedia.org/wiki/File_(command)) 找到了 [Magic Number](https://en.wikipedia.org/wiki/Magic_number_(programming))。magic number 原指在代码中突然出现的毫无意义的数字，（所谓黑魔法~）。而 file 这里的 magic number 是来自 Unix 用几个没有任何说明的常量来区别几种可执行文件，而后来得知，这两个 magic number 对应的是 PDP-11 的跳转指令。故事大概是这样的：

可执行文件的最前端有 8 bytes 的 header，这段 header 包含程序（text）的大小和初始化的值（global）。由于早期 Unix 没有分页机制（有 PIC），导致操作系统只能一整块地从文件系统中读取文件，然后跳转到文件的开头开始执行。但是由于 header 的存在，我需要跳转到真正的程序处，所以最前面的 16bit 是用于跳转到 8byte 这个offset 的位置，而[这个跳转指令](https://en.wikipedia.org/wiki/PDP-11_architecture#Branch_instructions)由分两部分：跳转指令（0x04）和偏移（0x07）：0000,0010|0000,0111)，这里偏移 7 是因为 `pc` 在第一个 byte 上，它需要向后跳转7个字节，也就是跳过 8 bytes 的 header。这样这个可执行文件可以正常执行了。

Unix 应该是支持 3 种格式的可执行文件：

1. the executable image contained relocatable memory references (normal)
2. the newly implemented paged read-only executable image
3. the separated instruction and data paged image

从第六版开始，Unix 支持分页机制：一整个文件看起来载入到一段连续的内存中，但其实，header 所在的页是位于内核 buffer 中，而用户空间的起始位就直接是真正代码段了，直接是从 0 开始的。但对不同的可执行文件，操作系统执行 `exec()` 时应该是有不同的执行逻辑。这个时候上面的 2 bytes 被当作一个特征来区分三种文件，这个时候所谓“指令”其实不再是指令，只是被当作一个特征符。而 Unix 在代码中去比较这两个字节的时候，直接拿了两个常数在对比，并且从来没有提到关于这个比较的含义，黑魔法出现了，哈哈。后来这种文件内容的特征指纹来表示文件类型的方式逐渐被大家接收，并逐渐扩展到所有文件。[wiki](https://en.wikipedia.org/wiki/Magic_number_(programming)#In_files) 上也列了一些常见文件类型的特征数据，比如 `png` 格式开头的 8 字节 `89 50 4E 47 0D 0A 1A 0A`。


`libmagic.so` 则包含了一个完整的数据库来记录这些特征段，从而使得 `file` 可以不借助后缀也能识别到文件类型。这个数据库在 `file` 源码的 `magic/Magdir` 目录中，以文件类型为名创建了很多文件，如 image，music。这些文件有特殊的格式，参见 `magic(5)`。


## 更新 Debian 上的 `libmagic.so` 和 `file`

了解了问题所在之后就是如何更新 `libmagic.so` 了，而由于 Debian 11 即`bullseye` 提供的 `file` 和 `libmagic1` 太旧了，查看了一下 Debian 12 即 `bookworm` 的 `libmagic1` 的版本和我的 arch 差不了太多，应该是可以 work 的。直接下载 `bookworm` 的 `libmagic1.deb` 不行，`libc` 版本不对，大概是因为 `bookworm` 的 `libc` 的版本有做升级。这次不想从 arch 的 `PKGBUILD` 折腾编译 Debian 的包了，想直接从 Debian 的源码编译，简单查看了一下发现 `file` 和 `libmagic1` 的依赖不多，单独更新这两个包应该不会有太大问题，`libc` 的版本不一致应该也可以解决。

### 更新源：

在 `/etc/apt/sources.list` 添加 `bookworm` 的 `deb-src`，就是复制 `bullseye` 的 `deb-src`，然后把 `bullseye` 改成 `bookworm`:

```sources.list
deb-src http://mirrors.ustc.edu.cn/debian bullseye main contrib
```

改成

```sources.list
deb-src http://mirrors.ustc.edu.cn/debian bookworm main contrib
```

改完的 `sources.list`：

```sources.list
#/etc/apt/sources.list
deb http://mirrors.ustc.edu.cn/debian bullseye main contrib
#deb-src http://mirrors.ustc.edu.cn/debian bullseye main contrib
deb http://mirrors.ustc.edu.cn/debian-security bullseye-security main contrib
#deb-src http://mirrors.ustc.edu.cn/debian-security bullseye-security main contrib
deb http://mirrors.ustc.edu.cn/debian bullseye-updates main contrib
#deb-src http://mirrors.ustc.edu.cn/debian bullseye-updates main contrib
deb http://mirrors.ustc.edu.cn/debian/ bullseye-backports main contrib
#deb-src http://mirrors.ustc.edu.cn/debian/ bullseye-backports main contrib

deb-src http://mirrors.ustc.edu.cn/debian bookworm main contrib
```

```sh
sudo apt update
```

### 下载源码并编译
[ref_link1](https://www.linuxfordevices.com/tutorials/debian/build-packages-from-source)，[ref_link2](http://rediceli.blogspot.com/2006/08/debian.html)

下载 `libmagic1` 源码：

```sh
mkdir libmagic1
cd libmagic1
apt source libmagic1
```

下载编译所需环境：

```sh
sudo apt -y build-dep libmagic1
```

编译打包：

```sh
cd file-5.44
dpkg-buildpackage
```


然后 file 和 libmagic1 等包全被打好了，一样好方便哇。然后就是简单的装 `deb` 的包了，dbgsym 的和 dev 的包没装：

```sh
sudo dpkg -i file_5.44-3_arm64.deb libmagic1_5.44-3_arm64.deb libmagic-mgc_5.44-3_arm64.deb
```

* ps. feh 也可以通过这种方式来打包诶。
* pps. 发现 debian 默认的 feh 是 disable libmagic 的，看了下代码 `feh_is_image` 这个函数是受这个宏控制的，所以我只要把 `feh` 的这个选项关了，`file` 也不需要更新了。（啊啊啊）

