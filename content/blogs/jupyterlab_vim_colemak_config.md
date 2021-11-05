---
title: "jupyter-lab里的vim，colemak 配置"
date: 2020-08-03T23:09:45+08:00
code: true
mermaid: false
draft: false
author: "Ben"
tags: ['jupyterlab', 'vim', 'colemak', 'python']
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

[上一篇文章](../jupyterlab_colemak) 介绍了 `jupyter-notebook` 怎样 `map`。`jupyterlab` 是新一代的 `jupyter-notebook，` 有点像`R-Studio`, 有更多的功能了。

## TL; DR
* install
```sh
# For jupyterlab 2.0
jupyter-labextension install @axlair/jupyterlab_vim
# vimrc
jupyter-labextension install jupyterlab-vimrc
```
* config
```JSON5
    "nmap": [
        ["n","j"],
        ["e","k"]
    ],
```


---
## 安装`jupyterlab`
```sh
pip install --user jupyterlab
# or
conda activate <env>
conda install jupyterlab
```

## 安装 extension
### 1. vim binding
```sh
# jupyterlab 1.0
jupyter labextension install jupyterlab_vim
# jupyterlab 2.0
jupyter-labextension install @axlair/jupyterlab_vim
```

### 2. vim keymap

```sh
jupyter-labextension install jupyterlab-vimrc
```

#### 配置
1. 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803225854570.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0JlblNZWg==,size_16,color_FFFFFF,t_70#pic_center =200x)
2. 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803225927188.png#pic_center)
3. 仅供参考
```JSON5
{
    // vimrc
    // jupyterlab-vimrc:vimrc
    // Settings to be applied to codemirror vim mode
    // *********************************************

    // imap commands
    // [lhs, rhs] - e.g. "imap": [["ii", "<Esc>"]]
    "imap": [
    ],

    // inoremap commands
    // [lhs, rhs]
    "inoremap": [
        []
    ],

    // nmap commands
    // [lhs, rhs]
    "nmap": [
        ["n","j"],
        ["e","k"]

    ],

    // nnoremap commands
    // [lhs, rhs]
    "nnoremap": [
        ["i","l"],
        ["m","n"],
        ["k","m"],
        ["f","e"],
        ["t","f"],
        ["l","u"],
        ["u","i"],
        
        ["N","J"],
        ["E","K"],
        ["I","L"],
        ["M","N"],
        ["K","M"],
        ["F","E"],
        ["T","F"],
        ["L","U"],
        ["U","I"],
        ["\'","'"]
    ],

    // vmap commands
    // [lhs, rhs]
    "vmap": [
        
    ],

    // vnoremap commands
    // [lhs, rhs]
    "vnoremap": [
        ["n","j"],
        ["e","k"],
        ["i","l"],
        ["k","n"],
        ["f","e"],
        ["t","f"],
        ["l","u"],
        ["u","i"],
        
        ["N","J"],
        ["E","K"],
        ["I","L"],
        ["K","N"],
        ["F","E"],
        ["T","F"],
        ["L","U"],
        ["U","I"],
        
        ["\'",":"]
    ]
}
```

其中需要**注意**的是
```JSON5
    "nmap": [
        ["n","j"],
        ["e","k"]
    ],
```

可能是因为 `jupyterlab_vim`, 会涉及到cell之间的跳转，所以需要使用`nmap`




