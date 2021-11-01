---
title: "超级用户su没有权限修改所属"
date: 2019-07-16T14:07:27+08:00
draft: false
author: "Ben"
description: "su_no_perm"
tag: "Linux"
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

## 两种情况
### 1. [lsatter &chattr ](https://blog.csdn.net/FightingOning/article/details/88684618)
lsatter: 查看文件隐藏属性
chatter: 设置文件的隐藏属性

### 2.分区文件格式
我自己是上面两个命令都报错:

```sh
lsattr: 对设备不适当的 ioctl 操作 读取 File 的标志时
chattr: 对设备不适当的 ioctl 操作 读取 File 的标志时
or
lsattr: Inappropriate ioctl for device while reading flags on File
chattr: Inappropriate ioctl for device while reading flags on File
```

后来尝试将文件从fat32格式移动到ext4(Linux 根目录下的分区)，再直接换所属(chown)，发现就可以了。
