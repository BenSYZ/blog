---
title: "TCP Keep-Alive"
date: 2022-02-19T07:29:11+08:00
lastMod: 2022-02-19T07:29:11+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "tcp keep alive 学习"
tag: ["network", "qt"]
---


PACKET seq num-1
https://stackoverflow.com/questions/5855774/how-can-i-figure-out-if-a-packet-is-a-tcp-keep-alive

~/keep_alive.pcapng



SO_REUSEADDR

TODO

### 心跳和状态
当设了心跳，心跳超时后，是认为对方断线，这边不会保留 timewait


这边 keep alive 断开，另一边不知道，当连上又发，这边会发 RST

