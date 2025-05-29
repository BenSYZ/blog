---
title: "install NVIDIA supported docker"
draftDate: 2025-04-28T11:50:29+08:00
date: 2025-04-28T11:50:29+08:00
lastMod: 2025-04-28T11:50:29+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "跟随官方文档，整理了 docker、NVIDIA 驱动和NVIDIA Container Toolkit 的安装配置"
tags: ["NVIDIA", "driver", "docker"]
---

## docker
[official doc](https://docs.docker.com/engine/install/ubuntu/)

### 装
```sh
# 拷自 official doc
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update

sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

### 配
#### user add to docker group
```sh
useradd -a -G docker <username>
```

重登

#### add proxy

```sh
systemctl edit docker
```

```conf
[Service]
Environment="HTTP_PROXY=http://localhost:8118"
Environment="HTTPS_PROXY=http://localhost:8118"
```


## NVIDIA
分两部分

* driver：host 机上要能操作 GPU ([official doc](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/index.html))
* container：可以将 GPU 传递到 docker 内 ([official doc](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html))


可以从 container 的 [official doc](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html) 入手。

### driver
container 第一节给了一个链接跳转到 [cuda-installation doc](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/)，我们不需要在 host 机上装 cuda，所以点右边 [4. Driver Installation](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/#driver-installation)，直接给了个 [tesla 的 driver installation doc](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/index.html)


#### 先下 keyring
```sh
# ubuntu2204/x86_64
wget https://developer.download.nvidia.com/compute/cuda/repos/<distro>/<arch>/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
```

#### 下
选择开源还是闭源驱动可以看 [5. Kernel Modules](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/index.html#kernel-modules)

新的是 open source 的 driver 的

```sh
sudo apt-get update
apt install nvidia-open
```

#### 验
```sh
nvidia-smi
reboot
```

### NVIDIA Container Toolkit
[official doc](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)

#### 装

```sh
# 拷自 official doc
curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | sudo gpg --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg \
  && curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    sudo tee /etc/apt/sources.list.d/nvidia-container-toolkit.list
sed -i -e '/experimental/ s/^#//g' /etc/apt/sources.list.d/nvidia-container-toolkit.list
sudo apt-get update
sudo apt-get install -y nvidia-container-toolkit
```


#### 配
#### nvidia-container-runtime
```sh
# 拷自 official doc
sudo nvidia-ctk runtime configure --runtime=docker
# /etc/docker/daemon.json
```

#### btrfs
`/etc/docker/daemon.json`

```json
{
    "storage-driver": "btrfs"
}
```

#### results
`/etc/docker/daemon.json`

```json
{
    "storage-driver": "btrfs",
    "runtimes": {
        "nvidia": {
            "args": [],
            "path": "nvidia-container-runtime"
        }
    }
}
```

#### 测
```sh
# 拷自 official doc
docker run --rm --runtime=nvidia --gpus all ubuntu nvidia-smi
```

`--runtime=nvidia` 通过这个将显卡传递到 docker 里
