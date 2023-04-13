---
title: "[Documents] samba 使用手册"
date: 2022-01-29T07:04:13+08:00
lastMod: 2022-01-29T07:04:13+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "samba"
tag: ["samba", "documents"]
---

## samba 是什么
samba 就是一个文件共享软件，可以在 Linux 和 Windows 下使用。和ftp不同的是，他可以直接在远端服务器上修改，而不需要下载下来再上传。

## 如何使用 samba
samba 可以匿名也可以使用帐号密码登录，为了安全起见，并没有设置匿名访问，且默认密码并不能直接登录samba，所以在登录到samba之前我们需要先修改密码。

### 修改samba密码

不修改密码将无法登录。

* 修改原理：ssh 到samba服务器，然后默认执行修改自己samba密码的程序，程序执行完成后会退出，无法登录服务器
* 修改步骤如下

```shell
➤ ~ 修改samba密码
默认帐号密码是姓名拼音首字母
  ➜  ssh syz@192.168.3.113
syz@192.168.3.113's password: syz
# 密码和用户名一样
Last login: Sat Jan 29 15:03:28 2022 from 192.168.3.102
# samba 默认密码和用户名一样
Old SMB password: syz
# 设置 samba 的密码（记得设置得复杂一些，会有一定的复杂度的检查)
New SMB password: XXXXX
# 再输一次新密码
Retype new SMB password: XXXXX
Password changed for user syz
Connection to 192.168.3.113 closed.
```

### 登录samba
* Windows: 资源管理器的地址栏输入 `\\192.168.3.113` 即可登录访问。  
    若提示无法识别地址等，需要安装 samba client 服务。安装方式：`设置-应用-程序和功能(在最下面)-启用或关闭Windows功能-SMB 1.0/CIFS 文件共享支持-第二个客户端`

* Linux: `nautilus(文件管理)-Other Locations-Connect to Server- 输入 smb://192.168.3.113`。  
    我的nautilus似乎有些问题。所以用命令行挂载的
    1. 安装`cifs-utils`
    2. `sudo mount -t cifs -o username="syz",uid=$UID,gid=$GID,iocharset=utf8,rw,dir_mode=0775,file_mode=0664 //192.168.3.113/Documents /mnt`(写了个脚本)

