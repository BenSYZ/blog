---
title: "通过修改注册表修改Windows键位为colemak"
date: 2020-07-30T14:55:29+08:00
lastMod: 2021-11-01T11:43:31+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "Windows 10 中使用colemak键盘布局（包括拼音）以及修改`<esc>` 和 `<Caps Lock>`"
tag: 'csdn, keyboard, Windows, colemak'
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

## TL; DR
### 1. 转 colemak
```reg
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
"Scancode Map"=hex:00,00,00,00,00,00,00,00,14,00,00,00,3a,00,01,00,01,00,3a,00,\
  21,00,12,00,19,00,13,00,22,00,14,00,24,00,15,00,26,00,16,00,16,00,17,00,15,\
  00,18,00,27,00,19,00,13,00,1f,00,1f,00,20,00,14,00,21,00,20,00,22,00,31,00,\
  24,00,12,00,25,00,17,00,26,00,18,00,27,00,25,00,31,00,00,00,00,00
```
### 2. `<Esc>` `<Caps Lock>` 互换

```reg
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
"Scancode Map"=hex:00,00,00,00,00,00,00,00,03,00,00,00,3a,00,01,00,01,00,3a,00,00,00,00,00,00
```

## 问题
[在Linux中修改键盘为colemak键盘很容易](../keyboard_map_linux)：

```sh
# 暂时
setxkbmap us -variant colemak
setxkbmap us -variant workman
setxkbmap us -variant norman

# Ubuntu
sudo dpkg-reconfigure keyboard-configuration

# Arch
loadkeys colemak
```

但是在Windows中就很麻烦了，有推荐在[这里](https://colemak.com/Windows)下载安装，然后修改这一项注册表`\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts\00000804` 但是我的使用体验并不好，所以直接修改注册表来映射。

## 解决

新建一个txt文档，填入下面内容后，修改扩展名为`reg`，然后双击，也可以手动添加项。

```reg
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
"Scancode Map"=hex:00,00,00,00,00,00,00,00,14,00,00,00,3a,00,01,00,01,00,3a,00,\
  21,00,12,00,19,00,13,00,22,00,14,00,24,00,15,00,26,00,16,00,16,00,17,00,15,\
  00,18,00,27,00,19,00,13,00,1f,00,1f,00,20,00,14,00,21,00,20,00,22,00,31,00,\
  24,00,12,00,25,00,17,00,26,00,18,00,27,00,25,00,31,00,00,00,00,00
```

上面这段注册表的作用是在`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout`项中新建了一个二进制值`Scancode Map`，它的值为等号后面的。具体格式请见 Old_Driver_Lee的[博客](https://blog.csdn.net/Ace_Shiyuan/article/details/81283065)。需要注意的是一开始的`14`是16进制，代表下面有20行。


另外键位码请看 古城月雨细的[博客](https://blog.csdn.net/lhdalhd1996/article/details/90741092)，在第二篇文章中也提到了作者提供的键位码是要把高低字节交换一下，就比方说作者提供的`Caps Lock 00 3A`，在修改注册表时需要写成`3A 00`。另外那篇文章只介绍了两个键位互换，所以会分不清楚，哪个是原来的码，哪个是实际要输入的码，经过尝试，发现后面的是原来的键位，前面的是我实际想要的输出的码。举个例子：

```reg
3a,00,01,00
```

这串是指，按的是`01 00`键，实际输出的是`3a 00`, 也就是按下`Escape` 输出的是`Caps Lock` 键

## `<Esc>` `<Caps Lock>` 互换
```reg
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
"Scancode Map"=hex:00,00,00,00,00,00,00,00,03,00,00,00,3a,00,01,00,01,00,3a,00,00,00,00,00,00
```

```reg
00,00,00,00,
00,00,00,00,
14,00,00,00,

3a,00,01,00,
01,00,3a,00,

21,00,12,00,
19,00,13,00,
22,00,14,00,
24,00,15,00,
26,00,16,00,
16,00,17,00,
15,00,18,00,
27,00,19,00,
13,00,1f,00,
1f,00,20,00,
14,00,21,00,
20,00,22,00,
31,00,24,00,
12,00,25,00,
17,00,26,00,
18,00,27,00,
25,00,31,00,
00,00,00,00


# 换过顺序的
A 1E 00 
B 30 00 
C 2E 00 
D 20 00 
E 12 00 
F 21 00 
G 22 00 
H 23 00 
I 17 00 
J 24 00 
K 25 00 
L 26 00 
M 32 00 
N 31 00 
O 18 00 
P 19 00 
Q 10 00 
R 13 00 
S 1F 00 
T 14 00 
U 16 00 
V 2F 00 
W 11 00 
X 2D 00 
Y 15 00 
Z 2C 00 
```

## 恢复
删除此注册表项
