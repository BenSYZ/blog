---
title: "将服务器的 Ubuntu 系统换成 Arch Linux"
date: 2021-09-30T19:13:30+08:00
draft: false
author: "Ben"
description: 根据 Arch wiki, 通过 `chroot` 将服务器的 Ubuntu 系统换成 Arch Linux
tag: 'arch-chroot, Linux, Install Arch,'
---

## Introduction
这两天腾讯在搞活动，有便宜的服务器，于是买了，如之前的阿里云一样没有 Arch Linux 的系统镜像。

早就听说了有一个叫 `vps2arch` 的脚本可以将服务器上的 Ubuntu 系统转成其他系统，然后看 [Arch wiki](https://wiki.archlinux.org/title/Install_Arch_Linux_from_existing_Linux（简体中文）#从一个主机运行另一个 Linux 发行版) 的时候发现了不止它，还有好多，它还介绍了如何手动转到 Arch Linux。玩 Arch 的人怎么能不手动试试呢🤪（发现这个脚本是在原来阿里云过期之后的事了。然后一直没机会，虽然虚拟机上也行，但懒啊)

然而 Arch wiki 在后面[安装提示](https://wiki.archlinux.org/title/Install_Arch_Linux_from_existing_Linux（简体中文）#安装提示) 那节让我从 [Installation Guide](https://wiki.archlinux.org/title/Installation_guide_（简体中文）) 中的重新分区开始，继续后面的操作，我就这样试了，然后....🤦系统挂了。那个时候也没懂原理，一开始的想法是是否能够把系统全加载到内存中，然后就可以把原分区删了重新分区，然后又看到 wiki 是这样说的，就尽信了。

后来我找到了这篇[博客](https://blog.inkuang.com/2020/114/)，然后就成啦，原来后面的步骤是“挂载分区”而不是”建立分区“。

写博客的时候才发现，原来中文手册里说的就是“挂载”，而英文手册里是“分区” 😠 , 我把它改啦😏

## 原理
通过 `chroot` 进入 Arch Linux 系统，删除原 Ubuntu 系统文件，此时原来系统的内核还在内存中，我们通过 Arch Linux 系统向这个内核发送指令。 *内核* 和 *操作系统* 的关系可以看看我的这篇关于 [gpio]({{<ref "/blogs/gpio">}}) 的文章。

在这里操作系统可以简单理解为你能看到的各种文件，除了内核把一些内核空间的文件映射到文件系统内的文件，比方说 `/proc` `/sys` `/run` 下的各种文件等等，还有设备文件夹 `/dev` 下的内容也应该是内核生成出来的。我们通过 Arch linux 系统向原内核发送命令是怎么发送的，举个例子：我们可以通过 `ls` 查看当前目录下的文件，那这个命令对应的是这个命令文件本身 `/bin/ls` 和它所依赖的各种库 （通过 `ldd /bin/ls` 可以查看）这里的库类似于 python 的包。现在我们有 Arch 这个完整的 chroot 的系统，那当然可以向内核发送命令啦。

## 实验

**注意**: 记得对当前系统创建快照，因为有可能网不好，而你又还没配置好系统，那就没了....

按照 Arch wiki [Install Arch Linux from existing Linux](https://wiki.archlinux.org/title/Install_Arch_Linux_from_existing_Linux_（简体中文）)

### 1. 去[这里](https://archlinux.org/download/) 挑个镜像网站，下 bootstrap 的 tar 包，放到 `/tmp` ，解压：

```shell
# tar xzf /tmp/archlinux-bootstrap-...-x86_64.tar.gz
```

### 2. 编辑 `mirrorlist`
由于 bootstrap 中没有编辑器，你需要先编辑 `mirrorlist` ，选一个镜像网站

```shell
# vim /tmp/root.x86_64/etc/pacman.d/mirrorlist
```

如果忘记了，然后 Ubuntu 系统又删了，可以使用 `sed` 或者` echo >>`

```shell
# sed -i 's/#\(.*ustc.*\)/\1/p' /etc/pacman.d/mirrorlist
```

### 3. `chroot`
将这个目录绑到这个目录，大概率是要这条 `mount` 的，应该是意味着这是一个 mount point，这样在 `chroot` 的时候不会有警告。并 `chroot`

```shell
# mount --bind /tmp/root.x86_64 /tmp/root.x86_64
```

```shell
# /tmp/root.x86_64/bin/arch-chroot /tmp/root.x86_64/
```

`arch-chroot` 相对于 `chroot` 的区别在于，它把一些内核空间的目录挂载到了 `chroot` 环境中，可以看 Arch wiki 的 [方法一：使用 Bootstrap 镜像](https://wiki.archlinux.org/title/Install_Arch_Linux_from_existing_Linux_（简体中文）#安装提示)节

### 4. 初始化 `pacman` 密钥

```shell
# pacman-key --init
# pacman-key --populate archlinux
```

### 5. 把分区再挂到 `chroot` 的 `/mnt` 下

我们这个时候可以看一下分区表：

```sh
$ lsblk
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sr0     11:0    1 145.7M  0 rom
vda    254:0    0    80G  0 disk
|-vda1 254:1    0     1M  0 part
└-vda2 254:2    0    80G  0 part /etc/resolv.conf
                                 /mnt
```

`/etc/resolve.conf` 是 `arch-chroot` 导致的，这里的 `/mnt` 就是 Installation guide 中的 `/mnt` 了。

### 6. 删 Ubuntu 的系统文件

现在可以删除 Ubuntu 的系统文件了。(Ubuntu 也是 GNU 系统的一种） 但要注意不要删掉 `/tmp` , `/dev` , `/proc` , `/run` , `/sys` 。因为 `/tmp` 里有 bootstrap 解压后的文件，也就是我们现在在使用的 Arch Linux 的系统文件，后面 4 个是内核映射到文件系统的文件，也不能删，你也可以保留你的 `/home` 目录，如果需要保留其他根目录下的配置文件，记得备份，因为 arch 在安装的过程中也会覆盖。

当然你也可以不删，安装好之后再用 [pacman 列出来](https://wiki.archlinux.org/title/Pacman/Tips_and_tricks#Identify_files_not_owned_by_any_package)，不要在改了配置之后再删哦，有些类似 `/etc/hostname` 是不被 `pacman` 跟踪的

```shell
# find /etc /usr /opt | LC_ALL=C pacman -Qqo - 2>&1 >&- >/dev/null | cut -d ' ' -f 5-
```

### 7. 按照 [Installation guide](https://wiki.archlinux.org/title/Installation_guide_（简体中文）#安装)，安装 Arch Linux。

这里只是提一下需要哪些

1. 安装：`pacstrap /mnt base linux linux-firmware`
2. 分区：`genfstab -U /mnt >> /mnt/etc/fstab`
3. `arc:chroot /mnt`
3. 时区：`ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime` , `hwclock --systohc`
4. 文字编码：`/etc/locale.gen` `locale-gen`

5. grub:

```sh
pacman -s intel-ucode
grub-install --target=i386-pc /dev/vda
grub-mkconfig -o /boot/grub/grub.cfg`
```

6. 网络：`/etc/hostname` , `/etc/hosts` , `/etc/systemd/network/en.network` ，enable it

```conf
[Match]
Name=en*
# arch 是 ens5 这种的
# Ubuntu 的网卡名还是 eth0
# 不放心可以用 e*

[Network]
DHCP=ipv4
```

8. `pacman -S neovim openssh sudo` , enable ssh

9. 用户、密码、 `sudo` : `passwd` , `useradd -a -G wheel ben` , `passwd ben` , `visudo`

### 7. 重启
如果启动失败可以通过 VNC 的方式登录，可以看到 grub 等界面


