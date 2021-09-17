---
title: "Linux 图形界面缩写：X、dm、wm、de"
date: 30.12.2006"
draft: true
---

date: 2021-04-29T19:57:32+08:00

在搜索相关问题的时候我们经常会看到标题这几个缩写，X DM WM DE，经常会搞不清楚，因此本文便总结一下，参考来源
https://unix.stackexchange.com/questions/20385/windows-managers-vs-login-managers-vs-display-managers-vs-desktop-environment

## X DM WM DE
我们以启动过程中会涉及的几个名词为例，从底层到上层列出
1. X windows: **display server**
	提供图形环境，配置硬件相关，不是 tty 终端。
	+ 例如`Xorg`
	+ 配置文件夹 `/etc/X11`, 配置显卡，键盘鼠标等，和硬件比较接近（具体见 `man xorg.conf`)

2. dm: **display manager**
	就是登录界面，输入用户名、密码那个。
	+ 例如：
		- sddm
		- LightDM (Ubuntu 16.04 之前）
		- GDM(gnome DM, Ubuntu 16.04 之后）

2. wm: **window manager**
	窗口如何排布，比如浮动 (Windows 那种）, 平铺（窗口永远是占满屏幕）......
	+ 例如：
		- i3wm: 平铺为主
		- dwm: 平铺为主
		- awesome: 各种都有
		- sawfish (Gnome 2): 浮动
		- mutter (Gnome 3)：浮动

3. de: **desktop environment**
	是一个集成的环境，它包含各种软件：状态栏，开始菜单栏，系统设置等等。也会随带系统浏览器，文件管理器等等。
	当然它也得包含 window manager, display manager
	+ 例如
		- kde
		- Gnome
		- Xfce
		- 深度桌面环境

---
之间的联系：以 ubuntu 为例
1. 进入系统从黑黑的那个屏幕跳到紫色的时候，鼠标出现的时候，X 启动（差不多）(??? ps)
2. 跳出用户名，等待输入密码，这个是 display manager（登录界面）
3. 登录后的桌面是 desktop environment。window manager 包含在里面。

如果你装的是 DE，开箱即用，没啥好说。

如果你想看看纯粹的 window manager 是怎么样的，如果你不想那么多附带的没用的软件，想精简一些，那就使用 WM，见下一篇。


相关阅读：
* 鸟哥：http://cn.linux.vbird.org/linux_basic/0590xwindow.php#xwin
* archlinux wiki: https://wiki.archlinux.org/index.php/Display_manager_（简体中文）
* archlinux wiki: https://wiki.archlinux.org/index.php/Window_manager_（简体中文）
* archlinux wiki: https://wiki.archlinux.org/index.php/Desktop_environment_（简体中文）

