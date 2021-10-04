---
title: "vps 修改分区大小"
date: 2021-10-03T16:22:33+08:00
draft: true
author: "Ben"
description: "如何修改服务器的分区大小"
tag: vps, partition, Linux
---

本文参考：
https://blog.sourcepole.ch/2012/09/11/howto-shrink-a-remote-root-ext3-filesystem-on-debian-wheezy-3/

实现方法，卡在ramfs中的linuz，通过这个 linuz 来对分区进行调整。

拷贝 grub 的正常项目，并修改以`linux` 起头的那行中的`UUID`的值，随便什么值都行，因为我们的目的就是让引导不能继续

```conf
set UUID=2021-10-01
set ver=2021.10.10
set label=ARCH_202110
set isofile="/archlinux-$ver-x86_64.iso"

menuentry 'Arch ISO' --class arch --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-linux-fallback-bdc8b6ae-23bf-4301-aefe-bb42097a1aac'{
	search --no-floppy --fs-uuid --set=root bdc8b6ae-23bf-4301-aefe-bb42097a1aac
	echo	'Loading Linux linux ...'
	loopback loop $isofile
	linux (loop)/arch/boot/x86_64/vmlinuz-linux archisolabel=$label img_dev=/dev/disk/by-uuid/$UUID img_loop=$isofile earlymodules=loop
	echo	'Loading initial ramdisk ...'
	initrd (loop)/arch/boot/intel-ucode.img (loop)/arch/boot/x86_64/initramfs-linux.img
}

```

~~上面我把原来是`f3ee`结尾的`UUID`值修改成了`f3e2`这样这行就会报错了，~~，用 ISO 文件可以实现，原来的方法，会出现无法输入的情况，但这个 ISO 可以实现(ISO放在/下，挑一个[镜像](https://archlinux.org/download/)下载，修改上面的参数)。这个时候我们会进入 `initramfs` 的命令行，我们会发现提示符是`[rootfs ]#`，有可能会无法输入，重启一下。

分区缩小有两部
1. 将逻辑分区缩小，我们使用`e2fsck`和`resize2fs`来实现
2. 将物理分区缩小，我们使用`fdisk`

但是由于我们在 `initramfs` 中，上面这三个命令都没有，那怎么办呢？诶！这里没有但是原来的分区里有啊：

```sh
mount /dev/vda2 /mnt
cp /mnt/bin/fdisk /bin/
```

这个时候我们就可以执行`fdisk`了，然后你就会发现他说缺少库，和python一样，C也是需要相应的库支持的，我们可以通过`ldd <file>`查看这个文件依赖的所有动态库，这里我们就缺啥拷啥吧。等到没有报错之后，`fdisk`就可以正常执行了。同样的方法对`e2fsck` 和 `resize2fs`

## 调整分区大小
### 逻辑分区
```sh
umount /dev/vda2
# 需要先检查分区
e2fsck -p -f -C 0 /dev/vda2
# 调整逻辑分区到70G大小
/bin/resize2fs /dev/vda2 70G
```

这个时候当你把`/dev/vda2`挂载到目录后你会发现`df`和`fdisk`给到的值是不一样的。

### 调整物理分区

```sh
fdisk /dev/vda
```

删掉那个要调整的分区，重新新建一个分区，大小70G并 **保留分区标签**，（我那个时候调大了一点，因为我不知道会不会有计算上的差别，之后又重新扩大逻辑分区填满这个分区）。

send `ctrl alt del` 就可以快速重启了

