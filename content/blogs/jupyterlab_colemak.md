---
title: "vim+colemak(keymap)+jupyterlab"
date: 2020-08-03T23:09:45+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "jupyterlab_colemak"
tags: ['jupyterlab', 'colemak', 'python']
---

我原先发于 [CSDN](https://blog.csdn.net/BenSYZ)

推荐使用 jupyter-lab, [见此](https://blog.csdn.net/BenSYZ/article/details/107775517)
## TL, DR
1. install this extension, and change the `vim .jupyter/custom/custom.js`, map `n` and `e`. Reason is that this extension can help you jump from cells to cells.

2. change the codemirror in notebook package.
```
~/.local/lib/python3.8/site-packages/notebook/static/components/codemirror/keymap/vim.js
```
My example is [here](https://github.com/BenSYZ/jupyter-vim-binding-colemak/tree/master/colemak)
## Essay
Vimer 很多，所以有作者开发了[jupyter-vim-binding](jupyter-vim-binding)。
colemaker 很少，而上面的 jupyter-vim-binding 的作者只提供了移动的API，不能做到映射全部的键位。

我本想修改这个插件来扩展它的功能。从 [codemirror](https://codemirror.net/) 官网下下了 codemirror 的代码，对照了jupyter-vim-binding 对`hi`(qwert: `hl`)方向的修改成功了，但是 colemak 向下是`n`键，所以还需把搜索键引入进来，同样的方法试了一下不行，发现源码中涉及的函数太多，只会照猫画虎。


弄了一天，终于搜对了关键字 “jupyter notebook 自定义 codemirror”， 谷歌对中文的支持不好，英文搜出来的修改的手段也太官方了，所以一直解决不了，百度搜到了[Jupyter Notebook 更改字体、字体大小、行高](https://www.cnblogs.com/ZhangHT97/p/13336975.html)。

nb! 直接修改 codemirror 在 jupyter 中的源码

```
~/.local/lib/python3.8/site-packages/notebook/static/components/codemirror/keymap/vim.js
```

（写这篇文章的时候想到，其实早该想到 jupyter 没有把 codemirror 当作插件，既然这么无从下手，那说明 codemirror 必定是随着 jupyter 一起装的）

到这里事情差不多解决了，突然想到既然这样，jupyter-vim-binding 就没有用了呀。事实告诉我的确如此。然而问题是按`n`(`j`)不能跳到下一个cell，想到jupyter-vim-binding 不是有这部份吗（在之前想扩展它的功能时看到的）再次启用这个插件，这次我使用了原版的插件。把我自己的部分去掉了。这次问题是真的解决了，啊，完美！！！

现在回想起来这个插件的作者不是不想继续扩展而是没必要啊。
