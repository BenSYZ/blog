---
title: "Gerrit Replication"
draftDate: 2025-05-27T16:02:47+08:00
date: 2025-05-27T16:02:47+08:00
lastMod: 2025-05-27T16:02:47+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "Gerrit Replication 介绍和配置"
tags: ["Gerrit", "backup", "git" ]
---

[toc]

## Introduction
[Gerrit 官方文档](https://gerrit.googlesource.com/plugins/replication/+doc/master/src/main/resources/Documentation/config.md)

`replication` 是 Gerrit 提供的插件：当“主服务器”中的相关仓库更新时，会自动同步到“从服务器”，因而可以实现备份，只允许单向流动。它的实现是：用本地的 Unix 帐号，通过 git 向其他服务器 push “本地 Gerrit 工作目录下 git 子目录下的目录仓库”。

**注意**：

`replication` 的执行和 Gerrit 软件提供的 git 账户没有关系：

* master 端：它不是用 Gerrit 内部的用户来推送到远端的，而是用“启动 Gerrit 软件的 **本地帐号** ”
* slave 端：接收不是走 Gerrit 开放的 ssh 端口。而是走“本地账户”的 ssh 端口（不全对，可以参考 replication 后记）

由于我希望 slave 端接收到从 master 传过来的 git 后，亦能提供 clone fetch 等只读的服务。而这些服务是受限的，权限信息都在 All-Projects.git, All-Users.git 这两个仓库下，最简单的办法就是 slave 端也用 Gerrit 来发布，以避免权限转化和不统一。同时我还要配置 slave 端设成只读。

## 配置
### 启动两个 docker
启动两个 docker

```yaml
# docker-compose.yaml
version: '3'

services:
  gerrit-1:
    image: gerritcodereview/gerrit:3.4.0
    hostname: gerrit-1
    volumes:
       - volume-git-1:/var/gerrit/git
       - volume-index-1:/var/gerrit/index
       - volume-cache-1:/var/gerrit/cache
    ports:
       - "29418:29418"
       - "18080:8080"

  gerrit-2:
    image: gerritcodereview/gerrit:3.4.0
    hostname: gerrit-2
    volumes:
       - volume-git-2:/var/gerrit/git
       - volume-index-2:/var/gerrit/index
       - volume-cache-2:/var/gerrit/cache
    ports:
       - "29419:29418"
       - "22001:22"
       # - "18081:8080" # 由于我们将其配置成 replica，网页端口是无用的

volumes:
  volume-git-1:
  volume-index-1:
  volume-cache-1:
  volume-git-2:
  volume-index-2:
  volume-cache-2:
```

```sh
docker compose up
```

Gerrit docker 中的 site_path 在 `/var/gerrit/`，同时家目录也设置成了这个。

### slave 机的配置
#### 安装配置 `sshd`
##### 进入 slave Gerrit 2

对 slave 机器的配置，除特殊说明外均需要 root 权限。

```sh
docker exec --user root -it gerrit-gerrit-2-1 /bin/bash
```

##### 替换 `CentOS` 源

由于我用的是 `gerrit:3.4.0` 需要替换一下源

```sh
# https://serverfault.com/questions/1161816/mirrorlist-centos-org-no-longer-resolve
sed -i 's/mirror.centos.org/vault.centos.org/g' /etc/yum.repos.d/CentOS-*.repo
sed -i 's/^#.*baseurl=http/baseurl=http/g'      /etc/yum.repos.d/CentOS-*.repo
sed -i 's/^mirrorlist=http/#mirrorlist=http/g'  /etc/yum.repos.d/CentOS-*.repo
```

##### 安装 `openssh-server` 和 `passwd`

```sh
yum install -y openssh-server passwd
```

##### 配置密码

```sh
# 密码自定义
password="<gerrit_password>"
echo "$password" | passwd --stdin -f gerrit
# 要删除这个文件，否则 ssh 连上来会报 System is booting up. Unprivileged users are not permitted to log in yet
# https://unix.stackexchange.com/a/487937/442345
rm -f /run/nologin
```

##### 配置 `sshd`
Gerrit docker 的入口是 `/entrypoint.sh`，由于是使用 `gerrit` 用户启动这个脚本的，所以我们不能用 root 的 `sshd` 来启动，所以有两种方案：

###### 方案一： 将配置文件的所有权归属给 `gerrit`

由于我们使用的是 docker，故我们可以直接修改默认的 `sshd` 的 host keys

```sh
ssh-keygen -A
```

```sh
chown gerrit:gerrit /etc/ssh/sshd_config
chown gerrit:gerrit /etc/ssh/ssh_host*
# 修改 /etc/ssh/sshd_config 的端口、登录选项等信息（建议在配置完成密钥之后关闭密码登录）
```

在 `/entrypoint.sh` 的最上面添加 `/usr/sbin/sshd`，在重启 docker 的时候会生效。

###### 方案二：`gerrit` 启动自己的 sshd 使用自己的 `sshd_config`

当我们安装 Gerrit 在物理机器上时，可以由 `gerrit` 用户自己启动一个 `sshd`，

```sh
su gerrit # 切换到 gerrit 用户
mkdir -p ~/.ssh/etc/ssh
ssh-keygen -A -f ~/.ssh
mv ~/.ssh/etc/ssh/sshd_host* ~/.ssh
rmdir --ignore-fail-on-non-empty -p ~/.ssh/etc/ssh
cp /etc/ssh/sshd_config ~/.ssh/sshd_config
# 修改 ~/.ssh/sshd_config 的端口、登录选项等信息（建议在配置完成密钥之后关闭密码登录）
```

在 `/entrypoint.sh` 的最上面添加： `/usr/sbin/sshd -f /var/gerrit/.ssh/sshd_config`，在重启 docker 的时候会生效。

#### 配置 replica
如果在 Slave 上用 Gerrit 提供服务，需要配置只读，只允许从 master 复制到 slave

有两种方案，一种是配 [`$site_path/etc/All-Projects/project.config`](https://gerrit-review.googlesource.com/Documentation/config-gerrit.html#All-Projects-project.config)，可以覆盖 All-Projects.git 中的配置信息，还有一种就是配置 [`$site_path/etc/gerrit.config`](https://gerrit-review.googlesource.com/Documentation/config-gerrit.html#container.replica) 直接配置 replica，后者更合适。


```ini
[container]
	replica = true
```

```sh
docker cp --archive gerrit-gerrit-2-1:/var/gerrit/etc/gerrit.config  gerrit2.config
docker cp --archive gerrit2.config gerrit-gerrit-2-1:/var/gerrit/etc/gerrit.config
```

* Tips:

配置只读之后，ssh 的 `gerrit query` 等 `gerrit` 命令就没有了，网页也是 404 了。

```console
bash-4.4$ ssh -p 29419 admin@192.168.3.55 gerrit query status:open
fatal: gerrit: query: not found
````

#### 移走（或者删除） All-Project.git 和 All-Users.git

由于我们想做的是全量备份，这两个仓库直接用 master 的配置就好了，所以进入 `$site_path/git`，将里面所有的仓库移到别处备份（明确的话也可以直接删除）。

#### 重启 Gerrit
重启 Gerrit 以生效 `/entrypoint.sh` 和 `gerrit.config`

```sh
docker restart gerrit-gerrit-2-1
```

### master 机的配置
参考 [Gerrit 官方文档](https://gerrit.googlesource.com/plugins/replication/+doc/master/src/main/resources/Documentation/config.md)

https://groups.google.com/g/repo-discuss/c/pJHiB1Vki4U

#### 进入 master Gerrit 1

对 master 机器的配置，除特殊说明外均只需普通用户权限

```sh
docker exec -it gerrit-gerrit-2-1 /bin/bash
```

#### 打通 ssh 通讯

1. 配置 ssh 公钥密钥对，用于无密码传输，Gerrit 要求使用 `PEM` 的格式

```sh
ssh-keygen -m PEM -t rsa -C "your_email@example.com"
```

2. 将生成的公钥拷至 slave 服务器的本地用户的 `~/.ssh/authorized_keys` （在我们的例子中就是 `/var/gerrit/.ssh/authorized_keys`）文件中，详见 [ssh 免密登录](./rsa_ssh/)

Tips: 可以加这个前缀 `no-port-forwarding,no-X11-forwarding,no-agent-forwarding,no-pty`

3. 手动连接 slave 服务器，以添加 slave 服务器的 fingerprint 到 known_hosts

Tips: 可以用 `ssh_config` 来配置细节，另外，在公钥传输完成后，可以将密码登录禁用


#### 配置 `replication.config`
```ini
# $site_path/etc/replication.config
[remote "gerrit-2"]
url = ssh://gerrit@<my_host_ip_addr>:22001/var/gerrit/git/${name}.git
```

<!--
```sh
docker cp --archive replication.config gerrit-gerrit-1-1:/var/gerrit/etc/replication.config
```
-->

这里的 URL 就是 clone 时，或者说 push 时的 URL

* `<my_host_ip_addr>`：是 host 机的 IP 地址
* `22001`：是上面 docker 映射出来的内部的 22 号端口，而不是 Gerrit 的端口
* `/var/gerrit/git/${name}.git`：slave 机上的 `gerrit` 用户看到的仓库路径。

##### 重载配置

master 机这里这里也需要添加上面生成的 ssh key 到 Gerrit 的 admin 用户中，当然你也可以使用本地账户。

```sh
# 在 gerrit-1 内执行（如果用的是本地账户，则在物理机上执行）
ssh -p 29418 admin@127.0.0.1 gerrit plugin reload replication
```

### master 触发 replication
master 机这里用以下命令触发 replication

```sh
ssh -p 29418 admin@127.0.0.1 replication start
```

#### 调试 replication
##### 查看 replication 的状态

```sh
ssh -p 29418 admin@127.0.0.1 replication list --detail
```

##### 查看 replication 的日志

```
cd $site_path
cat logs/replication_log  |grep '^\[2025-'
```

### slave 没有权限
在重启配置完成后，有可能 ssh 到 slave 的 `gerrit` 提示 `admin@192.168.3.55: Permission denied (publickey).`。

解决方式是把 replica 关掉，进到 Welcome 网页（可以直接在机器上访问 docker 的 IP + 8080 的端口。点 `Install plugins`，点 `Done >> Go To Gerrit`。然后重新打开 replica 就可以了。

### slave 重建索引
如果没有配置 replica，网页上不会刷新更新的仓库，需要用以下命令刷新一下

```
ssh -p 22001 gerrit@<my_host_ip_addr> gerrit index start projects --force
```

## replication 后记

对 slave 端如果有 GitHub 等其他可以接收裸 git 的命令的也可以，甚至可能也可以走 Gerrit 提供的端口，只要 Gerrit 端口允许裸推。从我实验的情况来看，Gerrit replication 在连上 slave 服务器时会执行如下命令：

```
-c mkdir -p '/srv/git/gerrit/test-repo.git' && cd '/srv/git/gerrit/test-repo.git' && git init --bare && git symbolic-ref HEAD 'refs/heads/master'
-c git-upload-pack '/srv/git/gerrit/All-Projects.git'
-c git-receive-pack '/srv/git/gerrit/All-Projects.git'
```

