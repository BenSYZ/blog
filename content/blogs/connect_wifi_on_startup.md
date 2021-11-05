---
title: "Linux开机自动连接网络"
date: 2020-02-26T10:26:09+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "connect_wifi_on_startup"
tags: ['Linux', 'wifi']
---

整合自: https://bbs.archlinux.org/viewtopic.php?id=178625

上一篇[文章](../connect_wifi_command)讲到如何用命令行连接wifi，大家可以去看看。

本文参考 [https://bbs.archlinux.org/viewtopic.php?id=178625](https://bbs.archlinux.org/viewtopic.php?id=178625)
## TL,DR

### `wpa_supplicant`
```sh
# config wpa_supplicant
#/etc/wpa_supplicant/wpa_supplicant-wlp5s0.conf

# enable wpa_supplicant
systemctl enable wpa_supplicant@wlp5s0
```
### `config systemd-network`

```sh
# config systemd-network
# /etc/systemd/network/00-wireless-dhcp.network
[Match]
Name=wlp5s0

[Network]
DHCP=yes

# enable systemd-networkd.service
systemctl enable systemd-networkd.service
```

## 需要工具
1. `wpa_supplicant`
2. `dhcpcd`

## 配置文件

1. `/etc/wpa_supplicant/wpa_supplicant-网卡名.conf`

2. `/etc/systemd/network/00-wireless-dhcp.network`

## 步骤
### 1. `wpa_supplicant`
#### 1.1 查看网卡名字

```sh
ip link
# 1: lo
# 2: enp4s0
# 3: wlp5s0
```

1. lo 是loop设备
2. enp4s0 是有线设备
3. wlp5s0 是无线设备


**注意我的网卡名是 `wlp5s0`，下文中的`wlp5s0` 都需替换成你的**

#### 1.2 生成 `wpa_supplicant.conf`文件
将生成的`wpa_supplicant.conf` 放到`/etc/wpa_supplicant/`文件夹下，并重命名，格式为：

```sh
wpa_supplicant-网卡名.conf
```

例如我的就是：

```sh
wpa_supplicant-wlp5s0.conf
```


##### 通过修改原始配置文件
`man wpa_supplicant.conf` 知道原始配置文件在

```sh
/usr/share/doc/wpa_supplicant/wpa_supplicant.conf
```

##### 通过`wpa_passphrase`生成配置文件

```sh
# wpa_passphrase MyNetwork SuperSecretPassphrase > /etc/wpa_supplicant/wpa_supplicant-wlp5s0.conf
```

##### 通过下面的格式
`priority` 越大优先级越高

```sh
country=CN
update_config=1
network={
	ssid="SSID"
	psk="Passwd"
	key_mgmt=WPA-PSK
	priority=9
	id_str="identify which wifi is"
}

network={
	ssid="SSID"
	psk="Passwd"
	key_mgmt=WPA-PSK
	priority=8
	id_str="identify which wifi is"
}
```
#### 1.3 用`systemctl`启动`wpa_supplicant`服务

```sh
systemctl enable wpa_supplicant@wlp5s0
```

**注意arch bbs 原回答多写了`.conf`**，后面也有跟帖指出了这个问题。

### 2. `systemd-networkd`
#### 2.1 配置systemd-network的[配置文件](https://www.centos.bz/2018/05/systemd-%E7%9A%84%E7%BD%91%E7%BB%9C%E7%AE%A1%E7%90%86/)
创建`/etc/systemd/network/00-wireless-dhcp.network`，archbbs中说这个名字随意。

```
[Match]
Name=wlp5s0

[Network]
DHCP=yes
```

#### 2.2 用`systemctl`设置启动服务`systemd-networkd.service`

```sh
systemctl enable systemd-networkd.service
```

## 回顾

在[1.3](#1.3)的`systemctl enable wpa_supplicant@wlp5s0`中，我们看到：

```sh
Created symlink /etc/systemd/system/multi-user.target.wants/wpa_supplicant@wlp5s0.service → /usr/lib/systemd/system/wpa_supplicant@.service.
```

查看`/usr/lib/systemd/system/wpa_supplicant@.service`，其中有一行：

```sh
ExecStart=/usr/bin/wpa_supplicant -c/etc/wpa_supplicant/wpa_supplicant-%I.conf -i%I
```

这里的`%I` 就是`systemctl enable wpa_supplicant@wlp5s0`中的`wlp5s0`，它通过`%I`来传递设备名，所以我们也就知道之前为什么要把配置文件这样命名了
```sh
/etc/wpa_supplicant/wpa_supplicant-网卡名.conf
```
