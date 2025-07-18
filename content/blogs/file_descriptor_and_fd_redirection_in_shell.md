---
title: "file descriptor and fd redirection in bash"
draftDate: 2025-07-18T16:18:39+08:00
date: 2025-07-18T16:18:39+08:00
lastMod: 2025-07-18T16:18:39+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "文件描述符和 bash 中文件描述符号的重定向，以及 bash 中的各种箭头：<, <<, <<<, >, >>, <(), >()"
tags: ["file descriptor", "Linux", "shell", "bash"]
---


## 文件描述符
文件描述符（file descriptor，简称 fd）在 Linux 中表示文件的句柄（file handle）但它到底是个啥呢？我们知道在 Linux 中，我们可以用 `open` 函数打开一个文件，返回的是 `int`，所以 fd 就是一个整型数组。我们可以在 `/proc/<pid>/fd` 中看到某个进程当前打开的文件。如果是磁盘上的文件，那就是一个 symbolic link 指向那个文件。当然还有其他管道文件。

那这个整型变量 fd 到底是什么呢？我们会发现 fd 的来源 `open` 其实不是普通的函数，它其实是一个系统调用，是 Linux 内核提供给应用程序的 API。在内核中 fd 这个整型是指该进程（`struct task`）打开的文件列表（（`struct task -> struct files_struct -> struct file *`））数组的下标。应用程序将来要操作这个文件只需要将这个 `int` 丢给操作系统，操作系统就会代替程序去读取写入实际的文件。

## 标准输入、标准输出和错误输出
在 Linux 中有三个较为特殊的文件描述符，它们是

* 标准输入（stdin）：`fd=0`
* 标准输出（stdout）：`fd=1`
* 标准错误输出（stderr）：`fd=2`

我们在用 `fprintf` 的时候会用到他们，因为对应用程序来说，往屏幕上输出东西，也非它所能做的，也需要操作系统这个代理来实现。因为他们太基础了，所以默认这三个是开给所有的进程的。

## bash 中的 fd
man bash 中 REDIRECTION 节有详细的介绍

* Tips: 在下面的例子中，我们可以用 `echo $$` 得到当前进程的 PID，在合适的地方 `sleep`，然后外部去访问 `/proc/<pid>/fd` 查看当前 `fd` 的情况。


### fd 的表示

* 输入：`[fd]<`
* 输出：`[fd]>`

### fd 的新建
#### 输入

`[fd]< input_file`

```sh
exec 3< /etc/os-release
while read -r -u 3 line;do
    echo "$line"
done
exec 3<&-
```

#### 输出
`[fd]> output_file`

```sh
echo stdout 1> ./stdout.txt
echo stdout 2> ./stderr.txt
```

### fd 的复制
由于大部分的程序，输入输出只有三个标准输入，标准输出，标准错误输出，那我如何将标准输出重定向到任意一个 fd 上呢？

取出 fd 中当前真正的输出对象(并将其赋值)

`bash` 中用 `&fd` 来表示真正的输入输出通道，这个 `&` 大概是指 fd 1 2 3 只是 alias，我要取出 alias 原本的值，也就是真正的输入输出通道，然后传递给其他 fd alias。（这么看来 bash 的 fd 和变量的 fd 不对应，& 符号太反人类了。。。~~不应是 `*1` `*2` 吗~~）

#### 输入
`[fd_dst]<&fd_src`

```sh
exec 3< /etc/os-release
cat 0<&3
exec 3<&-
```

`cat` 只接收标准输入，那么我可以用 `0<&3` 将 `fd3` 实质的输入通道传递给 `fd0`，接到 `fd0` 上。

#### 输出
`[fd_dst]>&fd_src`

```sh
exec 3> ./fd3.txt
echo stdout 1>&3
exec 3>&-
```

`echo` 输出到标准输出，所以我可以用 `1>&3` 将 `fd3` 实质的输出通道传递给 `fd1`，接到 `fd1` 的后面


### fd 的关闭
#### 输入
`[fd]<&-`

我们已经看到过好多次 `[fd]<&-` 它代表关闭这个 `fd`。

#### 输出
`[fd]>&-`

和输入一样 `[fd]>&-` 它代表关闭这个 `fd`。

### fd 的移动
相较于 复制，就是在后面加一个 `-`
#### 输入
`[fd_dst]&<fd_src-`

#### 输出
`[fd_dst]&>fd_src-`

这在交换 stdout stderr 时很有用，无需反复开关

```sh
exec 3>&1-  # stdout(fd1) 接到 fd3 上
exec 1>&2-  # stderr(fd2) 接到 fd1 上
exec 2>&3-  # stderr(fd3) 接到 fd2 上

{
    echo stdout
    echo stderr >&2
} > /dev/null
```

### Tips
##### 单行的重定向
上文中我们用 exec 去新建、修改 fd，我们也可以在单行中新建和修改 fd。注意： **Bash 中单行的重定向讲究顺序，从左到右执行**

```sh
echo stdout 3> ./fd3.txt 1>&3
```

先定义一个 `fd3`，在将 echo 的 `fd1` 重定向到 `fd3`，倘若是下面这样，第一步 `1>&3` 时 `&3` 还没有定义，就会报 `3: Bad file descriptor`

```sh
echo stdout 1>&3 3> ./fd3.txt
```

所以常见的将所有的输入输出全输出到 `/dev/null` 是需要 `>/dev/null 2>&1` 而不能 `2>&1 >/dev/null`，后者先将 `fd2` 接到 `fd1` 此时也就是标准输出的通道，再将标准输出接到 `/dev/null`

```sh
{
    echo stdout
    echo stderr >&2 # 重定向输出到 stdout
} >/dev/null 2>&1
```


##### 为避免冲突让 bash 分配 fd

```sh
exec {fd}< /etc/os-release
while read -r -u "${fd}" line;do
    echo "$line"
done
exec {fd}<&-
```


#### 其他各种箭头
##### `>>`: Output Append
`>>` 很常见，append 到文件上

##### `<<`: Here Documents
输入没有 append 的概念，bash 就将这个当作 Here Documents，最常见的就是生成一个 shell 脚本时可以用，免得一行一行 echo

```sh
cat << EOF > here_doc.sh
#!/bin/bash
echo "arg1=\$1"
echo test text
EOF
```

还可以输入给 bc


* 这里的 `EOF` 可以是任意字符串
* here documents 会展开变量，如果希望在生成的脚本中展开时这里需要 `\`，或者将 `EOF` 用单引号包起来，可以避免转义 `$1` 等变量

```sh
cat << 'EOF' > here_doc.sh
#!/bin/bash
echo "arg1=$1"
echo test text
EOF
```

##### `<<<`: Here String
上文中的输入输出的对象都是文件，here string 可以将后面的 string 直接输入到 stdin

```sh
cat <<< "here string"
wc <<< "here string"
```

##### `<(cmd)` 和 `>(cmd)`: Process Substitution
###### 输入 `<(cmd)`

如果想要将命令的结果输入到 stdin，当然最简单的是管道，但管道有一个限制，管道后面的进程在 sub-shell 里，里面变量的修改影响不到当前 shell。此时 `<(cmd)` 就有用了。Process Substitution 本质也是管道或者说也是 fd，只是将 `<(cmd)` 的输出做一个 fd，然后这个 fd 可以用 stdin `<` 来接收

```sh
ls -la <(echo bar)
```

```sh
nic_8168=()
while read -r nic;do
    nic_8168+=("$nic")
done < <( find /sys/bus/pci/drivers/r816{8,9}/*:*:*/net/ -maxdepth 1 -mindepth 1 -exec basename {} \; 2>/dev/null)
```

###### 输出 `>(cmd)`

如果想要将命令的结果输入给另一个程序，最简单的还是是管道，但管道只能接 stdout，当然你也可以像上面一样交换 fd1 fd2 和 stdout stderr 的对应。但更简单的方式是 `>(cmd)`

```sh
{
    echo stdout
    echo stderr1 >&2
    echo stderr2 >&2
} 2> >( grep -c stderr >&2)
```

