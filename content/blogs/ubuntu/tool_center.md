---
title: "ubuntu setting page"
date: 2021-12-15T02:21:50+08:00
lastMod: 2021-12-15T02:21:50+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "setting"
tag: ["tool_center"]
---


# Ubuntu 没有系统设置导致不能调节分辨率

[toc]

## 现象

setting-> devices -> displays 不能设置分辨率，很多其他设置也没有了

## 解决办法

### TL;DR

安装 `unity-control-center` 即其依赖

### 不是很成功的尝试(`xrandr`)

可以临时设置，但是放在启动脚本失败了，设置被覆写了

```sh
xrandr # 查看相关信息
xrandr --output HDMI-1 --mode 1920x720 # 设置例子
```

然后写在xorg的启动脚本中(`xprofile`, `~/.xsession`)，失败的原因大概是因为，xrandr 设置成了gdm的，但是gdm转交给gnome时，分辨率又被不知道哪里的东西给覆盖了。

### 安装 `unity-control-center`
#### 将主机上线后使用 apt 安装

1. 将主机上线
似乎是相机或安全问题，主机设置了固定ip，导致无法获取正确ip。  
通过将 `/etc/network/interfaces` 中的配置注释(用`#`号) 重启。

2. 安装

```shell
# apt update
# apt install unity-control-center
```

如果不行可以看一下 apt 安装源(`/etc/apt/sources.list`) 是否正常。

安装完记得把`/etc/network/interfaces`中的网络配置回来

#### 将 `unity-control-center` 及其依赖打包并安装

如果每个主机都要上线、apt下载略有麻烦，我们可以将 `unity-control-center` 和它的依赖一起拷下来然后写个脚本安装一下就好

1. 依赖哪些包可以通过查看之前使用 `apt` 安装时的输出信息(`Unpacking ...`这一行)来获知。(如果安装信息找不到，卸载后记得`autoremove`去掉依赖)
2. 安装包在 `/var/cache/apt/archives`，从这里拷贝安装包到一个文件夹
3. 写安装脚本(dpkg -i)，按照 1 中的输出信息的顺序来依次安装

```sh
#!/bin/bash

# install unity-control-center and it's dependencies
dpkg -i libunity-control-center1_15.04.0+18.04.20180216-0ubuntu1_arm64.deb activity-log-manager_0.9.7-0ubuntu26_arm64.deb libgeonames-common_0.2+17.04.20170220-0ubuntu1_all.deb libgeonames0_0.2+17.04.20170220-0ubuntu1_arm64.deb libnm-gtk0_1.8.10-2ubuntu3_arm64.deb libtimezonemap-data_0.4.5_all.deb libtimezonemap1_0.4.5_arm64.deb system-config-printer_1.5.11-1ubuntu2_all.deb
dpkg -i unity-control-center_15.04.0+18.04.20180216-0ubuntu1_arm64.deb

# 这两行似乎也不是必须，标注一些包是依赖一些包是手动安装的
# mark unity-control-center and it's dependencies
apt-mark auto libunity-control-center1 activity-log-manager libgeonames-common libgeonames0 libnm-gtk0 libtimezonemap-data libtimezonemap1 system-config-printer
apt-mark manual unity-control-center
```

Tips: 安装包名：`软件名_版本`，写卸载脚本时记得把版本号去除。

ps: 感谢阳阳同学


