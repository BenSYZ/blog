---
title: "为 Linux colemak 修改键"
date: 2020-02-26T09:17:44+08:00
draft: false
author: "Ben"
description: "修改键位为colemak，交换＜esc＞ ＜capslock＞键——archlinux(其他也略有涉略)"
code: true
tag: 'colemak, keyboard, Linux'
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

## Change keyboard layout as colemak, and ` <Esc>` with `<CapsLock>`

For Windows check [here](../keyboard_map_windows)

### TLDR

```sh
# tty
# /etc/vconsole.conf
KEYMAP = colemak
# or
loadkeys colemak

# graphic
setxkbmap us -variant colemak
```

### change to colemak
#### Temporary
```sh
# tty
loadkeys colemak

# graphic
setxkbmap us -variant colemak
```
[参考](https://www.cnblogs.com/BlackStorm/p/Ubuntu-Switch-Keyboard-Layouts-Such-As-Colemak-Workman-Norman.html)

#### Permanet
```sh
# tty (include graphic)
# /etc/vconsole.conf 
KEYMAP = colemak
```

The above way may not work for Ubuntu, so adding `setxkbmap us -variant colemak` in `~/.xinitrc` may worked. If not see the next section.

### Map `Esc` with `CapsLock`
```sh
setxkbmap us -variant colemak
xmodmap -pke > .xmodmap
```

A small tip: show the key code by `xev` 

Then exchange the key codes of `<esc>` and `<capslock>`  in `.xmodmap` file.

#### Temporary
```sh
xmodmap ./.xmodmap
```
#### Permanet
##### i3-wm
In `~/.config/i3/config`
```config
exec_always sleep 1; xmodmap ~/.xmodmap
```

##### Others include i3-wm
```sh
# /usr/share/X11/xkb/keycodes/evdev
# exchange the keycodes of <esc> and <capslock>
```
For ubuntu, you could try the following, I forget exactly
```sh
sudo dpkg-reconfigure keyboard-configuration
```
