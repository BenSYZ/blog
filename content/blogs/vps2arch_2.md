---
title: "vps2arch"
date: 2021-09-30T19:13:30+08:00
draft: false
author: "Ben"
description: "I change my vps to archlinux from ubuntu, here is the steps I've done."
---


大部分按照[官方教程](https://wiki.archlinux.org/title/Install_Arch_Linux_from_existing_Linux_(简体中文))来的

下载 https://archlinux.org/download/

解压

```sh
tar xzf /tmp/archlinux-bootstrap-*-x86_64.tar.gz
```

大概率是要这条的，应该是意味着这是一个mount point，然后可以写了?

```sh
mount --bind /tmp/root.x86_64 /tmp/root.x86_64
```

chroot:

```sh
/tmp/root.x86_64/bin/arch-chroot /tmp/root.x86_64/
```

在chroot环境中导入keys

```sh
pacman-key --init
pacman-key --populate archlinux
```

更新数据库，下包到这个环境中。

```sh
pacman -Syu
pacman -S base-devel parted
```

在`chroot`环境中：
```
├─vda1 254:1    0     1M  0 part 
└─vda2 254:2    0    80G  0 part /
```

在官方教程里后面一节就是Installation Tips，说这个时候就去Installation guide#Partition the disks，事实上不是的。因为文件还在那个分区内，不能把分区抹掉。我们的操作是把`vda2` 挂载到`/mnt`，（上面显示的`/`似乎就是之前的`mount --bind`,把自己挂在在根分区，但有些问题的，`vda2`不是挂载在这里）,这样之后这个`/mnt`就是Installation guide中的`/mnt`了，但要注意不要删掉`tmp`(文件还在这里面)，`dev`,`proc`,`run` `sys`,其他全删掉。

后面就是Installation guide了

https://blog.inkuang.com/2020/114/



