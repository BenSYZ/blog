---
title: "管理从源码安装的应用的link——stow"
date: 2020-07-30T13:15:47+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "stow 介绍"
tags: ['stow', 'source code', 'Linux']
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

Know from [here](https://www.ostechnix.com/an-easy-way-to-remove-programs-installed-from-source-in-linux/)

## Install Position(take `neovim` as example)

```
/usr/local/stow/nvim/
```

## Make Symbolic Link

```sh
cd /usr/local/stow/
sudo stow nvim
```

This command will make some link files at `/usr/local/bin/`, pointing to all the files in  `/usr/local/stow/nvim/bin` and `/usr/local/share/`, pointing to all the files in `/usr/local/stow/nvim/share` and so on.

### Example
For example, After `make install`, there will be `nvim` in `/usr/local/stow/nvim/bin/`, you could [make a link file](https://blog.csdn.net/BenSYZ/article/details/98105357) (like `vim`) in this directory, like following,
```sh
  ➜  ll /usr/local/stow/nvim/bin
 total 16M
drwxr-sr-x 2 root staff 4.0K xxx xx xx:xx .
drwxr-sr-x 5 root staff 4.0K xxx xx xx:xx ..
-rwxr-xr-x 1 root staff  16M xxx xx xx:xx nvim
lrwxrwxrwx 1 root staff    4 xxx xx xx:xx vim -> nvim

 ➜  ll /usr/local/stow/nvim/share
total 28K
drwxr-sr-x  7 root staff 4.0K xxx xx xx:xx .
drwxr-sr-x  5 root staff 4.0K xxx xx xx:xx ..
drwxr-sr-x  2 root staff 4.0K xxx xx xx:xx applications
drwxr-sr-x 31 root staff 4.0K xxx xx xx:xx locale
drwxr-sr-x  3 root staff 4.0K xxx xx xx:xx man
drwxr-sr-x  3 root staff 4.0K xxx xx xx:xx nvim
drwxr-sr-x  2 root staff 4.0K xxx xx xx:xx pixmaps

```

and then `stow` it
```sh
  ➜  ll /usr/local/bin
total 8.0K
drwxr-xr-x  2 root root 4.0K xxx xx xx:xx .
drwxr-xr-x 11 root root 4.0K xxx xx xx:xx ..
lrwxrwxrwx  1 root root   21 xxx xx xx:xx nvim -> ../stow/nvim/bin/nvim
lrwxrwxrwx  1 root root   20 xxx xx xx:xx vim -> ../stow/nvim/bin/vim

  ➜  ll /usr/local/share
total 24K
drwxr-xr-x  6 root root  4.0K xxx xx xx:xx .
drwxr-xr-x 11 root root  4.0K xxx xx xx:xx ..
lrwxrwxrwx  1 root root    31 xxx xx xx:xx applications -> ../stow/nvim/share/applications
lrwxrwxrwx  1 root root    25 xxx xx xx:xx locale -> ../stow/nvim/share/locale
drwxr-xr-x  2 root root  4.0K xxx xx xx:xx man
lrwxrwxrwx  1 root root    23 xxx xx xx:xx nvim -> ../stow/nvim/share/nvim
lrwxrwxrwx  1 root root    26 xxx xx xx:xx pixmaps -> ../stow/nvim/share/pixmaps

  ➜  ll /usr/local/share/man
total 8.0K
drwxr-xr-x 2 root root 4.0K xxx xx xx:xx .
drwxr-xr-x 6 root root 4.0K xxx xx xx:xx ..
lrwxrwxrwx 1 root root   30 xxx xx xx:xx man1 -> ../../stow/nvim/share/man/man1
```

## Remove Symbolic Link

```sh
sudo stow --delete neovim
```



