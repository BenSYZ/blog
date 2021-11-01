---
title: "树莓派4B连不上wifi的一个意想不到的原因"
date: 2020-07-31T15:34:47+08:00
lastMod: 2020-09-25T11:45:08+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "USB3.0 conflicts WiFi 5G"
tag: 'raspberrypi, WiFi, wpa_supplicant'
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

## wpa_supplicant 无法配置wifi
输出结果如下：
```sh
  ➜  sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicatn/wpa_supplicant.conf
Successfully initialized wpa_supplicant
wlan0: Trying to associate with SSID 'xxxxxxx'
wlan0: CTRL-EVENT-ASSOC-REJECT bssid=00:00:00:00:00:00 status_code=16
wlan0: Trying to associate with SSID 'xxxxxxx'
wlan0: CTRL-EVENT-ASSOC-REJECT bssid=00:00:00:00:00:00 status_code=16
wlan0: Trying to associate with SSID 'xxxxxxx'
wlan0: CTRL-EVENT-ASSOC-REJECT bssid=00:00:00:00:00:00 status_code=16
wlan0: Trying to associate with SSID 'xxxxxxx'
wlan0: CTRL-EVENT-ASSOC-REJECT bssid=00:00:00:00:00:00 status_code=16
wlan0: CTRL-EVENT-SSID-TEMP-DISABLED id=1 ssid="xxxxxxx" auth_failures=1 duration=10 reason=CONN_FAILED
wlan0: Failed to initiate sched scan
wlan0: CTRL-EVENT-SSID-REENABLED id=1 ssid='xxxxxxx'
```
使用 `sudo iwlist wlan0 scan |grep SSID` 能搜到，但是就是连不上。

## 一个意想不到的原因
[`USB3.0` 会干扰wifi信号](https://www.zhihu.com/question/28422159)，之前插的口是上面那个3.0，这次插在了下面的。换到上面就可以了。

2020-07-30

---
2020-09-25更新

摊手，我现在也不行，换到2.0的口了。

以下内容翻译自[https://www.tp-link.com/ae/support/faq/1393/](https://www.tp-link.com/ae/support/faq/1393/)

1. 屏蔽 USB 3.0设备
将 USB 3.0设备放置在一个屏蔽的盒子中可以有效地减少影响。

2. 使用 USB 3.0屏蔽的延长电缆
如果 USB 3.0设备直接插入路由器，你可能需要使用屏蔽的扩展电缆。这将有助于消除 USB 3.0设备造成的干扰

3. 如果可以接受，使用 USB 2.0端口
如果你不需要 USB 3.0的高性能，你可以尝试将 USB 3.0设备插入路由器的 USB 2.0端口。在这种情况下，您将实现较低的数据速率，但消除所造成的干扰。
