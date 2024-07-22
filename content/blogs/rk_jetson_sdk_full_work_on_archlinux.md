---
title: "RK and Jetson SDK full work on ArchLinux"
date: 2024-01-15T06:47:39+08:00
lastMod: 2024-07-22T06:23:20+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "some problems of using RK and Jetson SDK on ArchLinux"
tags: ["arch", "jetson", "rk", "docker", "linux"]
---


## tl;dr

遇到的几个问题，和解决措施：

| Platform   | Problem                                         | Solution                                                                                                                             |
|------------|-------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------|
| RK         | `mknod failed`                                  | remount docker volume with `nodev,noexec`                                                                                            |
| ~~Jetson~~ | ~~`showmount -e` oom~~                          | ~~ulimit docker option add `--ulimit nofile=1024:524288`~~                                                                           |
| RK Jetson  | RK Maskrom and jetson  burn failed              | disable usbcore autosuspend, `echo -1 > /sys/module/usbcore/parameters/autosuspend`                                                  |
| Jetson     | `Error: ipv6: address already assigned.`        | no `ping6` command, add a ping6 in PATH                                                                                              |
| ~~Jetson~~ | ~~`mount.nfs: Connection timed out`~~           | ~~systemctl restart nfs-server~~ 看起来是我错误地添加 sudo 导致的，后面没再复现。                                                    |
| Jetson     | `failed to flash` (something like this)         | arch ssh does not support `dsa`, `tools/ota_tools/version_upgrade/ota_make_recovery_img_dtb.sh` 118 行附近，去除 dsa sshd key 的生成 |
| RK         | uboot make menuconfig failed                    | add `cc -> gcc-7` to PATH                                                                                                            |
| RK         | native `cp2102` driver does not support 1500000 | Use [AUR cp210x-overclock-dkms](https://aur.archlinux.org/packages/cp210x-overclock-dkms)                                            |


* 由于 docker 中 nfs 有问题（包括 service restart nfs），所以目前我没有使用 docker

其他一些 Ubuntu 和 Arch 兼容性的问题：

```sh
# Linux_for_Tegra/tools/kernel_flash/l4t_network_flash.func
enable_nfs_for_folder ()
{
    ...
    #chown root.root "${nfs_dir}"
    chown root:root "${nfs_dir}"
    ...
}
```

```sh
# Linux_for_Tegra/tools/kernel_flash/l4t_network_flash.func
network_prerequisite ()
{
    ...
    #service nfs-kernel-server restart
    systemctl restart nfs-server
    ...
}
```

## Introduction

因为我们用 RK 的板子，然后我在使用 Arch 时遇到一个问题，在板子是 `Maskrom` 状态的时候，我用 RK 提供的 `upgrade_tool` 不能烧录，会报段错误，提给 redmine 后，修复了段错误的问题，但是还是不能烧录，这个时候报下面的错误：

```log
14:40:29        Upgrade Tool v2.26
14:40:29        Current process in /home/ben/Downloads/upgrade_tool_v2.26_for_linux
14:40:29        Using /home/ben/Downloads/upgrade_tool_v2.26_for_linux/config.ini
14:40:29        Command Line: ./upgrade_tool uf /home/ben/nfs/ubuntu_local/benWork/images/RK/RK3588_20230907-1/rockdev/update.img
14:40:29        Current Device Location ID:13
14:41:12        Error:InitializeUsb-->open device failed,err=-1,device=0x11d19f0
14:41:12        Error:upgrade_firmware->Fail to create comm object!
```

反馈给 RK 无果后，此事搁置了。最近又在用 nvidia jetson 的板子，Nvidia 提供了烧录的 [docker](https://catalog.ngc.nvidia.com/orgs/nvidia/containers/jetson-linux-flash-x86)(Arch 上 docker 有点问题，`showmount -e` 会 oom，要做 ulimit 限制 nofile) 又是不能烧录， `flash.sh` 报的错如下

```log
...
[   0.2263 ] tegrarcm_v2 --new_session --chip 0x23 0 --uid --download bct_br br_bct_BR.bct --download mb1 mb1_t234_prod_aligned_sigheader.bin.encrypt --download psc_bl1 psc_bl1_t234_prod_aligned_sigheader.bin.encrypt --download bct_mb1 mb1_bct_MB1_sigheader.bct.encrypt
[   0.2275 ] BR_CID: 0x80012344705DE7C8200000000EFD80C0
[   0.2351 ] Sending bct_br
[   0.2351 ] ERROR: might be timeout in USB write.
Error: Return value 3
Command tegrarcm_v2 --new_session --chip 0x23 0 --uid --download bct_br br_bct_BR.bct --download mb1 mb1_t234_prod_aligned_sigheader.bin.encrypt --download psc_bl1 psc_bl1_t234_prod_aligned_sigheader.bin.encrypt --download bct_mb1 mb1_bct_MB1_sigheader.bct.encrypt
Reading board information failed.
Error: failed to generate images
Cleaning up...
Keeping working dir at /l4t/Linux_for_Tegra/temp_initrdflash/bootloader0 and /tmp/tmp.9slBpS2jmL
```

嘿，RK 的无果，Nvidia 的可以看看，当然我这个时候没有意识到两者有关联性，最后看到了这条 [post](https://forums.developer.nvidia.com/t/cannot-flash-jetson-agx-orin-jetpack-5-1-1/253705/15)，说禁用 usbcore 的 autosuspend。`echo -1 > /sys/module/usbcore/parameters/autosuspend`，然后真的，重新启动 jetson 的板子后，就能烧录了，太牛了。这个时候突然回想起来，usbcore 的 autosuspend？那 RK 的是不是也可以了，马上一试，果然，Maskrom 也能烧录了。爽啊，从此，我 Arch 的一条腿被接上来呃，原来说残次品是因为，RK 的首选支持是 Windows ，Jetson 的首选支持是 Ubuntu，但是我哪个都没用，就很尴尬。


## work space creation
### RK
参考 《Rockchip_Developer_Guide_Linux_Docker_Deploy_CN.pdf》

```dockerfile
FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN \
sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/' /etc/apt/sources.list \
&& apt-get update -y && apt-get upgrade -y \
&& apt-get install -y curl repo \
&& apt-get install -y bc binfmt-support bison bsdmainutils build-essential bzip2 chrpath cmake cpio cpp-aarch64-linux-gnu \
debianutils device-tree-compiler diffstat expat expect fakeroot fdisk file flex g++ gawk gcc gcc-multilib git git-core \
g++-multilib gpgv2 iputils-ping libegl1-mesa libelf-dev libgmp-dev libgucharmap-2-90-dev liblz4-tool libmpc-dev \
libsdl1.2-dev libssl-dev live-build make ncurses-dev net-tools patchelf python2 python3 python-is-python3 python3-git \
python3-jinja2 python3-pexpect python3-pip qemu-user-static rsync socat ssh strace sudo texinfo time tree unzip vim \
wget xterm xz-utils zstd \
&& useradd -c 'ben user' -m -d /home/kenxes -s /bin/bash ben && sed -i -e '/\%sudo/ c \%sudo ALL=(ALL) NOPASSWD: ALL' /etc/sudoers && usermod -a -G sudo ben \
&& mkdir /home/ben/ \
&& su ben -c 'pip3 install pyelftools -i http://pypi.mirrors.ustc.edu.cn/simple/ --trusted-host pypi.mirrors.ustc.edu.cn' \
&& echo "docker image build complete" \
&& echo "set timezone" \
&& ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime \
&& echo Asia/Shanghai > /etc/timezone \
&& echo "extra dependencies for buildroot" \
&& git clone https://salsa.debian.org/live-team/live-build.git --depth 1 -b debian/1%20230131 \
&& rm -rf live-build/manpages/po/* \
&& make install -j8 -C live-build \
&& rm -rf live-build \
&& apt-get -y autoclean && apt-get -y autoremove && rm -rf /var/lib/apt/lists/*
```

#### use the container
```zsh
make_docker_rockchip_run(){
    if [ "$1" = "RK3588" ] || [ "$1" = "RK3399" ];then
        platform="$1"
    else
        platform="RK3588"
        echo "use default platform $platform" >&2
    fi
    proxy_server="172.17.0.1:8119"
    env_all=("USER=ben" "http_proxy=http://$proxy_server" "https_proxy=http://$proxy_server")
    docker run --rm --privileged -it -u ben \
            --ulimit nofile=1024:524288 \
            -v "$PWD":/"$platform"/ \
            rockchip_build \
            env "${env_all[@]}" /bin/bash -c "cd $platform; \
                                              bash ./remount.sh; \
                                              exec /bin/bash"
}
```


```bash
#!/bin/bash
# remount.sh
read -r dev mount_point mount_options < <(mount -l | grep "$PWD" | sed -n 's/\(.*\) on \(.*\) type .* (\(.*\))/\1 \2 \3/p' )
mount_options="${mount_options/nodev,/}"
mount_options="${mount_options/noexec,/}"
mount_options="remount,""$mount_options"

mount -l | grep "$PWD"
echo "mount $dev $mount_point" -o "$mount_options"

echo -n "do it? "
read -r doit

if [ "$doit" = "y" ];then
    sudo mount "$dev" "$mount_point" -o "$mount_options"
fi
mount -l | grep "$PWD"
```

### Jetson

Arch 中没有 `ping6` 这个命令，所以需要做一个，记得添加到 PATH 里

```sh
# /usr/local/bin/ping6
#!/bin/sh
ping -6 "$@"
```

有一个 bug: `chown root.root "${nfs_dir}"`

```sh
# Linux_for_Tegra/tools/kernel_flash/l4t_network_flash.func
enable_nfs_for_folder ()
{
    ...
    #chown root.root "${nfs_dir}"
    chown root:root "${nfs_dir}"
    ...
}
```

* `nfs-kernel-server` -> `nfs-server`
* `service` -> `systemctl`

```sh
# Linux_for_Tegra/tools/kernel_flash/l4t_network_flash.func
network_prerequisite ()
{
    #service nfs-kernel-server restart
    systemctl restart nfs-server
}
```

nfs 可能还有一个问题，会报超时，需要重新 export 一下，可以通过 `exportfs -s` 查看两个 ipv6 的 share，然后重新 export 一下，我前几次遇到了，后面再次烧录就没遇到了:

```sh
sudo exportfs -o rw,nohide,insecure,no_subtree_check,async,no_root_squash '[fc00:1:1::/48]:/r35.3.1/Linux_for_Tegra/rootfs'
sudo exportfs -o rw,nohide,insecure,no_subtree_check,async,no_root_squash '[fc00:1:1::/48]:/r35.3.1/Linux_for_Tegra/tools/kernel_flash'
```
