---
title: "Linux 停转机械硬盘来省电"
date: 2020-02-26T09:32:00+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "tlp(auto) hdparm(manual)"
tag: 'Linux'
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

# hdd stop spin
## automatic, by `tlp`

```sh
# /etc/tlp.conf
# 具体可以看这个文件，下面两行只是我的设置，你的配置要自己修改
DISK_SPINDOWN_TIMEOUT_ON_AC="0 12"
DISK_SPINDOWN_TIMEOUT_ON_BAT="0 12"

# 设置开机启动
systemctl enable tlp
```

## manual, by `hdparm`
[https://blog.csdn.net/magaiou/article/details/92770845](https://blog.csdn.net/magaiou/article/details/92770845)

从 `man hdparm` 可知：

```sh
# -S 从1到240的值指定5秒的倍数，从而产生5秒到20分钟的超时。从241到251的值指定30分钟的1到11个单位，从而产生30分钟到5.5小时的超时。值252表示超时21分钟。
hdparm -S[num] /dev/hd*

# -y 强制IDE驱动器立即进入低功耗待机模式，通常会导致其降速。
hdparm -y /dev/hd*

# -Y 强制IDE驱动器立即进入最低功耗睡眠模式，导致其完全关闭。
hdparm -Y /dev/hd*
```

所以可以通过在`.zshrc`(或`.bashrc`) 中添加`alias` 来快速手动停转硬盘。

