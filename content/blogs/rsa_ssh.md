---
title: "ssh 免密登录"
date: 2021-02-05T12:13:15+08:00
lastMod: 2021-02-05T12:13:15+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "使用 rsa 非对称密钥来实现 ssh 免密登录"
tags: ["rsa","ssh","newbie"]
---


## rsa简介
### rsa
#### [什么是rsa](http://www.baidu.com/link?url=E7VV9LiPjbQHlSeIcFqyi_sx7fSDsVr8zPdlhSn7GVcsXuA5uiwUEmqmHXNQoJyroo71V84SlMGwRAML75z7D8HCAxMS6Ovzlljv0_Pr8DUNGyzrqNQWbsqTpXBlfsDAQfUx78JGfySl5JN4km2Qka&wd=&eqid=f42870ad0000ace4000000055e393bea)
RSA加密算法是一种非对称加密算法。在公开密钥加密和电子商业中RSA被广泛使用。它的基本原理是利用大数难以质因数分解，它有一对数字：
1. 密钥([yao4](https://zhidao.baidu.com/question/1579778969145333380.html))：绝对**不能**透露给别人。
2. 公钥：是要给别人的。


具体原理请见阮一峰的[blog](http://www.ruanyifeng.com/blog/2013/06/rsa_algorithm_part_one.html)

#### rsa的用途
##### [身份验证](www.ruanyifeng.com/blog/2011/12/ssh_remote_login.html)

所谓"公钥登录"，原理很简单，就是用户将自己的公钥储存在远程主机上。登录的时候，远程主机会向用户发送一段随机字符串，用户用自己的私钥加密后，再发回来。远程主机用事先储存的公钥进行解密，如果成功，就证明用户是可信的，直接允许登录shell，不再要求密码。

##### [数字签名](https://www.ruanyifeng.com/blog/2011/08/what_is_a_digital_signature.html)

数字签名用于发行软件时防止别人篡改，具体流程是这样的：

1. 发行者对文件进行hash校验，并把这个校验值和进行私钥加密得到*数字签名*，大家通过*公布在网站上的公钥*解开这段密文（数字签名），也就是文件的hash值。
2. 下载者对文件进行hash校验，和1.中得到的hash值进行比对。

	上面两个步骤如何防止第三方修改，发行者加密的文件第三方当然可以通过公钥解密之后修改，但是你也无法生成（到时候用户要用公布在网站上的公钥解密的）*数字签名*（必须知道密钥）。

---

## ssh用rsa方式登录远程主机
### 为什么要用rsa登录
1. 方便，每次登录不用输密码。
2. 安全，禁用密码登录后，不怕别人猜到密码。

### 文件信息
本文会涉及两个文件夹，一个是在客户端（client），一个是服务器端（server）：

* rsa 密钥位置(**client**)：`~/.ssh/`
* ssh 配置文件(**server**)：`/etc/ssh/sshd_config`

client 和 server 是相对的概念，在一次连接中，主动寻求连接的是 client ，被连的是 server。

### 如何配置
#### client
1. **生成密钥对：rsa 和 rsa.pub**

```sh
ssh-keygen -t rsa
```

得到

```
Generating public/private rsa key pair.
# 输入rsa对（`id_rsa`和`id_rsa.pub`）名称（带路径）
Enter file in which to save the key (/home/user/.ssh/id_rsa):

# 输入密码，使用rsa密钥时（这里就是登录远程主机时）用的
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
```

将会生成

* `rsa`: 密钥
* `rsa_rsa.pub`: 公钥

2. **[将`rsa.pub`附加到server `~/.ssh/authorized_keys`的文件内](https://www.ruanyifeng.com/blog/2011/12/ssh_remote_login.html)**

```sh
# Following 2 commands are excuted on client
ssh-copy-id [-i ~/.ssh/rsa.pub] user@host
# or
ssh user@host 'mkdir -p .ssh && cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
```

一个粗暴的办法，复制client内的`id_rsa.pub`的内容，登录server，附加到`~/.ssh/authorized_keys`中

#### server

1. **`/etc/ssh/sshd_config` 的配置**

```sh
# 允许公钥登录
PubkeyAuthentication yes

# 由 client 生成的公钥，可以添加多个，多行，自己注释
AuthorizedKeysFile	.ssh/authorized_keys

# 是否允许用密码登录
# 禁用后，如果密钥丢失就不能登录远程主机了
PasswordAuthentication yes
```

可以找到相应的项并取消注释。

可能你的系统用的是第一版ssh协议需要添加 `RSAAuthentication yes` 这一项来允许rsa方式登录，或者在`/etc/ssh/sshd_config`中[添加](https://www.cnblogs.com/Leroscox/p/9627809.html)

```config
Protocol 2
```

*更多详细帮助见`man sshd_config`*

2. **[重启远程主机的ssh服务](https://www.ruanyifeng.com/blog/2011/12/ssh_remote_login.html)**

```sh
#ubuntu系统
service ssh restart

#debian系统
/etc/init.d/ssh restart
```

### 连接
```sh
ssh user@host -i .ssh/rsa
```

## 附：不用输入`name@ip`的方法

对 `~/.ssh/config` 进行配置配置

```sshconfig
Host ubuntu
	# 远程主机 ip， `ssh name@ip` 中的`ip`
	HostName X.X.X.X

	# 远程主机 端口号(port), 默认是22，termux: 8022
	Port 22

	# 登录名， `ssh name@ip` 中的`name`
	User name

	# rsa密钥，对应的公钥放在server上(若无此项，则用密码登录)
	IdentityFile ~/.ssh/id_rsa
```

* 更多参数详见帮助`man ssh_config`

这样就可以直接通过

```sh
ssh ubuntu
```

来登录远程主机

## 附：ssh-keygen 公钥如果丢失，由私钥生成公钥

```sh
ssh-keygen -y -f id_rsa > id_rsa.pub
```

生成的`id_rsa.pub` 相比于自动生成的少了最后一项，但是没关系因为`rsa.pub`的[格式](https://blog.csdn.net/weixin_34051201/article/details/92480568)：
一般分为三部分：密钥类型、base64编码后的密钥、注释(可选)。中间用空格分开。

**注意**：这里生成的是公钥，就如之前说的从公钥是几乎不可能找回私钥的

## 附：ssh 时的fingerprint作用

```sh
$ ssh localhost
The authenticity of host 'localhost (127.0.0.1)' can't be established.
ECDSA key fingerprint is SHA256:o2cGXCQtdhvWUXG8liYGWJrhVuP4YF9/E839KY1HqH4.
Are you sure you want to continue connecting (yes/no)?
```

这个是为了防止我连接的server不是我想连的server，有可能是假的。这个fingerprint是用来确认server的身份的。这个就是我们之前提到的rsa公钥，只不过这次的fingerprint是server的。

## 附：不要验证fingerprint
局域网内ip会变，验证不通过很不方便，可以这样配置`~/.ssh/config`
例如：
```conf
Host ubuntu
	...
	StrictHostKeyChecking no
```
