---
title: "systemd"
date: 2022-08-29T05:57:24+08:00
lastMod: 2022-08-29T05:57:24+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "systemd"
tags: ["systemd"]
---

## Unit
### `Requires`:
1. active other units listed in this options
2. no order, active units listed in this option simultaneously with this unit
3. exit, if the units listed in this option failed explicitly. (implicitly: failed before path check etc, or exist normally)

### `Wants`:
1. weak version of Requires, only active

### `BindsTo`:
2. Strong version of Requires, if units listed in this option stopped, this unit stop too

### `Before, After`
before or after starting stated, (not started)

