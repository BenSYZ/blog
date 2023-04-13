---
title: "migrate from ubuntu16.04 to ubuntu20.04"
date: 2023-04-13T11:13:20+08:00
lastMod: 2023-04-13T11:13:20+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "migrate_from_ubuntu16.04 to ubuntu.04"
tags: ["Linux", "docker", "NVIDIA"]
---

公司服务器需要升级系统

1. 系统升级
2. docker 迁移


## 系统升级

1. download and extra to new root

```sh
curl https://cdimage.ubuntu.com/ubuntu-base/releases/focal/release/ubuntu-base-20.04.5-base-amd64.tar.gz | sudo tar -xzvf - -C /mnt
```

2. update and install grub and other needs

* `grub-efi-amd64` # 可能直接 grub 就好，不知道这种基础组建要怎么一下安装
* `ubuntu-minimal`
* `linux-image-generic`

3. 然后参考 archlinux wiki installation guide 做些必要的动作，不要忘了 `systemd-networkd` 和 `sshd`

特别关注 grub

* `grub-install`
* `grub-config`
* `efibootmgr` check 一下


## docker 迁移

### 1. `rsync`
```sh
# https://blog.lilydjwg.me/2013/12/29/rsync-btrfs-dm-crypt-full-backup.42219.html
rsync --archive --one-file-system --inplace --hard-links \
  --human-readable --numeric-ids --delete --delete-excluded \
  --acls --xattrs --sparse \
  --itemize-changes --verbose --progress \
  /var/lib/docker /mnt/var/lib
```

注意最后 不要变成 `/mnt/var/lib` 否则就变成 `/mnt/var/lib/docker/docker`....

### 2. Docker 复杂的点在 nvidia

Copy /etc/docker/

然后 docker 就 起不来了。日志

```log
E: Unable to locate package nvidia-container-toolkit-base
```

然后找到这个

https://gitlab.com/nvidia/container-toolkit/container-toolkit/-/tree/main/

> The NVIDIA Container Toolkit allows users to build and run GPU accelerated containers. The toolkit includes a container runtime library and utilities to automatically configure containers to leverage NVIDIA GPUs.


也就时说 container 要使用 `nvidia` 需要装 `nvidia-container-toolkit`(也就是 `/etc/docker/daemon.json` 中这行的作用 `"path": "nvidia-container-runtime"`)

安装分两个

1. nvidia driver https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#nvidia-drivers
2. nvidia-container-toolkit https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#installation-guide

其实是一篇文档...关于安装 `nvidia-container-toolkit` 的

#### 安装 `nvidia-container-toolkit`
##### pre-requests
注意左侧目录层次，Pre-Requisites 只包含两个 （我看走眼了 Container Device Interface (CDI) Support 这个不是必须的，这节说要装 `nvidia-container-toolkit-base` 不是很明白）

* NVIDIA Drivers （这个选用包管理器安装的 https://docs.nvidia.com/datacenter/tesla/tesla-installation-notes/index.html#package-manager , 并且不用管 post-installation steps 这个是 cuda 的，我们这里不需要 cuda，只要 driver)
* Platform Requirements (默认够了)

##### `nvidia-container-toolkit`
参照 https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#docker

这里有一点是 20.04 是指向 18.04 的源，文档里也有说，然后就 ok 了。


由于已开始 rsync 那里敲错了，之前的几个 container, image 都没找到，所以怀疑是迁移出了问题，然后就看到了这个。。。如何迁移？删除。。。不过，有可能是提交成一个 image 再新开 container 就可以了，由于我们以前用的是 nvidia-docker 2 的，就没关系了。

https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/migrating-from-1.0.html#migration-1-0
