---
title: "统一Linux Windows 时区"
date: 2020-02-26T10:05:47+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "把硬件时钟当作UTC"
tag: 'Windows, Linux, time'
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

## 思路
把硬件时间修改为UTC，system的修改为local time。

## windows
参考[https://blog.csdn.net/qq_40197828/article/details/79334158](https://blog.csdn.net/qq_40197828/article/details/79334158)
### 打开注册表编辑器

```cmd
regedit
```

### 新建表项

在 `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\TimeZoneInformation`中新建`RealTimeIsUniversal`项目


* 64 位系统，“QWORD（64位）值”。
* 32 位系统，“DWORD（32位）值”。

它的大体意思是，硬件时间被作为全球统一时间。


### 修改表项值
然后双击这一条目，弹出“编辑 QWORD（64位）值”的对话框，确保选中了“十六进制”，然后将“数值数据”改为“1”。最后点击“确定”。

### 可以备份下次直接注入注册表即可

## [linux](https://wiki.archlinux.org/index.php/Installation_guide)

按照官方的操作，好像市区还是有点问题。具体是那个命令我忘记了就是搞着搞着就可以了，大家如果知道是哪个还请在评论区告诉我
```sh
ls -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localetime
sudo hwclock --systohc
timedatectl set-local-rtc false
timedatectl set-timezone Asia/Shanghai
timedatectl status
```


后来发现其实windows和arch linux 没必要设置时区，因为它们都是把硬件时间当作当地时间，但是ubuntu就不一样了。
