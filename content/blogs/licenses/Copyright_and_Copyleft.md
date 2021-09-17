---
title: "Copyright and Copyleft"
date: 2021-04-29T19:53:43+08:00
draft: false
---

Cover: Copyright or Copyleft

本作品采用 知识共享 署名 - 非商业性使用 4.0 国际许可协议进行许可。
![CC](https://licensebuttons.net/l/by-nc/4.0/88x31.png)


## Introduction

一直想写一些技术文章，现在微信公众号，CSDN，都要你声明你是否是原创。如果是不是原创，请贴上原文链接，以及确定是否征求过原作者同意，但是我们这种小白想写一些技术分享，想写一些心得体会怎么可能写出完全原创的技术文章。总归都是学习，然后有什么所得。可能本来就不应该写？以免产生像毕业论文那种垃圾？但是把我总想分享一下自己的经验，所以有些畏首畏脚，自从上次写过一篇 ssh 的文章（貌似还担心因为版权问题还删了），就再也不想写了。


有同样困惑和犹豫的当然不止我一个，因此在这段停滞不前的时间里我了解到了 Creative Commons 和其他 Copyleft 协议，现将其分享出来。

---

## [Copyleft](https://www.gnu.org/licenses/Copyleft.zh-cn.html)
### 著作权 (Copyright)
在讲 Copyleft 之前我们先讲一下 Copyright，[@_著作权_2021] 非专业，只能简单介绍一下。
Copyright © 版权，也被称为著作权。另外，我们经常会听到“保留所有权利”或者"All rights reserved"。后两者现如今已经不需要声明，著作权便意味着保留所有权利。

> 自 2000 年 8 月 23 日，尼加拉瓜成为最后一个加入《伯尔尼公约》的《布宜诺斯艾利斯公约》签署国，使得标注“保留所有权利”的要求过时。此日起，所有《布宜诺斯艾利斯公约》（唯一一个要求使用“保留所有权利”的版权条约）签署国都是《伯尔尼公约》（无需任何版权通知）的成员。

如果保留所有的权利的话，我们在对已有内容的重新整理上会变得束手束脚，对内容的编改都要和原作者联系，这极大的阻碍了知识的流通。

### 公有领域 ([Public Domain](https://www.gnu.org/philosophy/categories.html#Non-CopyleftedFreeSoftware))
和 Copyright 相对的是“公有领域”（Public Domain），用数学的方式说就是所有作品内有版权的作品的补集，它和有版权的作品是完全对立的。 公有领域是由 **不再具有版权期限** 或 **从未受版权法保护** 的作品组成。换句话来说[公有领域](https://en.wikipedia.org/wiki/Public_domain) 就是指 **不受版权保护** 的作品。下面是几个例子

* 公有领域的书籍
	公有领域的书籍是指没有版权的书籍，未经许可而创建的书籍或版权已过期 的书籍或被没收。在大多数国家和地区，版权保护期限作者去世后 70 年后的第一个元旦日（1 月 1 日）起失效。墨西哥和美国有例外，美国是在 1926 年之前出版的所有书籍和故事都属于公有领域。
* 公有领域的音乐
* 公有领域的的电影

正如前面所说，共有领域的作品不受版权保护，那我可以随便拿来用，不用遵守什么规则。打个比方我把我自己拍的照片或者写的文章放到公有领域里，那谁都可以拿来用，他可以稍微改一下，就可以在不经过我的同意下拥有版权 (Copyright)。这样的话只有我的作品还是开放给大家使用的，基于我的作品又可以回到有版权 (Copyright) 的领域，这样一来，又不利于知识的传播。

### 著佐权 (Copyleft)
我们看到前面两个方法对于知识的传播保护的效果都不好，于是便到了今天的重点 Copyleft。

Copyleft 最早是由王理瑱（美国华人计算机工程师）在 1976 年在 Palo Alto Tiny BASIC 系统中提到："@Copyleft ALL WRONGS RESERVED"来鼓励他人改编他的源代码并发表改编。我们看到这和 "@Copyright ALL RIGHTS RESERVED"，恶趣味满满。然后在 1985 年理查德·斯托曼在《GNU 宣言》（GNU /ɡnuː/ 是一个自由的操作系统）中这样描述 Copyleft:

> GNU **不在公共领域**。 每个人都可以修改和重新分发 GNU，但**不允许**任何分发者**限制**其进一步的分发。 也就是说，将不允许进行专有修改。 我想确保所有版本的 GNU 保持自由。

我们可以看到关键信息有几个：
1. 不在公共领域，即不放弃版权
2. 都可以修改和重新分发
3. 不允许限制

一个作品遵循 Copyleft，我们首先声明 **它是有版权** 的；然后，我们给它加上 **发布** 条款，这个条款就是一个法律声明，它赋予所有人有使用、修改和重新发布程序的代码及其衍生作品的权利，但要求在这个过程中保持发布规则不变。他针对的是发布行为。

如果你的作品完全不依赖于其他 Copyleft 的内容，那么他对你个人是没有影响的，也就是说你可以把之后基于这项作品的创作保有 Copyright（所有权利）。比方说修个图，改个内容，发布基于此的专有软件（不放出源代码），这些都是可以的，但是别人就不能了，除非你 *对其单独授权*，让他在之后的创作中保有 Copyright，因为你对这个原创内容是有版权的，只是你在对大众发布的时候放弃部分权利，并增加一些要求。这在之后的 Creative Common 中会更好理解。

更多有关 Copyleft 的内容，大家可以看看 gnu 官网的内容：

https://www.gnu.org/licenses/Copyleft.zh-cn.html

讲得很清晰。

我们经常会看到一些 Copyleft 的作品使用一个用这个符号“🄯”来表明该作品是 Copyleft 的。这样做在法律上是个常见错误。Copyleft 的作品实际也是被版权法保护的，因此即便是 Copyleft 的作品，也应该使用版权标志©，或者标明“版权所有”。如前所述，圆圈里套着一个开口向左的 C，这个符号经常被用来表明 Copyleft。但是这个符号并没有任何法律效力，也无法取代版权符号。它也许可以出于娱乐的目的，使用在图书封面，海报或者类似的材料上。但若要用在网页上，还需谨慎。

### 软件中的协议
Copyleft 是一种抽象的概念，它无法直接使用。实现它的方式就是就是一些法律文件或者叫许可证 (license)，在软件领域就有：
* GNU 通用公共许可证 (GNU General Public License 简写 GNU GPL)
* Affero 通用公共许可证（AGPL）”
* GNU 宽通用公共许可证（LGPL)
* GNU 自由文档许可证（FDL）
等等等等，世界上的开源许可证，大概有[上百种](https://www.gnu.org/licenses/license-list.html)。
其中最流行的有六种 GPL、BSD、MIT、Mozilla、Apache 和 LGPL，[Paul Bagwell](http://pbagwl.com/post/5078147450/description-of-popular-software-licenses) 用一幅图来表示这六种许可证的区别，阮一峰先生对其进行了[翻译](https://www.ruanyifeng.com/blog/2011/05/how_to_choose_free_software_licenses.html)，我便贴在这：

![6_licenses](https://www.ruanyifeng.com/blogimg/asset/201105/bg2011050101.png)






我们经常会看到一些 Copyleft 的作品使用一个用这个符号“🄯”来表明该作品是 Copyleft 的。这样做在法律上是个常见错误。Copyleft 的作品实际也是被版权法保护的，因此即便是 Copyleft 的作品，也应该使用版权标志©，或者标明“版权所有”。如前所述，圆圈里套着一个开口向左的 C，这个符号经常被用来表明 Copyleft。但是这个符号并没有任何法律效力，也无法取代版权符号。它也许可以出于娱乐的目的，使用在图书封面，海报或者类似的材料上。但若要用在网页上，还需谨慎。







Copyleft 是一种对程序进行版权保护的方法。它并不是放弃版权，因为这样会使 Copyleft 变得不可操作。


一些

如果源代码也在公有领域中，

前面说了，如果保留所有的权利的话，我们在对已有内容的重新整理上会变得束手束脚，对内容的编改都要和原作者联系，这极大的阻碍了知识的流通，所以为了促进知识共享，就有人提出了 Copyleft.

[Copyleft](https://www.gnu.org/licenses/copyleft.zh-cn.html) 是一种让程序或其它作品保持自由 (freedom)（不是免费）的通用方法，并要求对 Copyleft 程序的任何修改和扩展都保持自由。


知识共享组织的主要宗旨是使得著作物能更广为流通与改作，可使其他人据以创作及共享，并以所提供的许可方式确保上述理念。



简单来说就是作者对

分为 *著作人格权* 与 *著作财产权* 。

先说一说 Copyleft，Copyleft 是我很早的时候就接触到的，它是


### 其他创作的许可
这里我们主要讲 《知识共享许可协议》(Creative Commons 简写 CC)。因为我们前面说了，Copyright 和共有领域两个极端都不利于知识的传播，因此 Creative Commons 和其他 Copyleft 一样，它保留了部分权利，而释放其他大部分的权利。根据你选择保留的权利，它分为很多版。我们先说这些[权利](https://zh.wikipedia.org/wiki/ 知识共享许可协议)：

* 署名（Attribution，by）
* 相同方式共享（ShareAlike，sa）
* 非商业性使用（Noncommercial,nc ）
* 禁止演绎（No Derivative Works，nd)

注意：上面的四个要求是对这项创作的 **使用者** 的要求。

“署名”和“非商业性使用”很好理解，其中“相同方式共享”就是，*使用者*得用同样的或者更严格的 CC 协议来发布你（这项创作的使用者）的新的创作品。
[禁止演绎就是说](http://creativecommons.net.cn/faq/faq-user/) ：*使用者*不能对这份作品进行修改，包括翻译，使用者只能逐字逐句地复制原作品，不能改编或改变原作品。

混合搭配这些条件可产生 16 种组合。4 种组合是同时包括“ND”和“SA”的条款，相互排斥；还有一种没有包括任何条款，因此有 5 个组合无效。11 种有效组合中，因为 98% 的特许人要求署名，五种缺乏“BY”的条款不再使用。[七个常规许可证](https://zh.wikipedia.org/wiki/ 知识共享许可协议#七个常规许可证的使用):


|图标 |	说明 |	缩写|
|-|-|-|
|![CC0 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/6/69/CC0_button.svg/100px-CC0_button.svg.png) |	不受限制地在全球范围内发布内容 |	CC0|
|![CC-BY 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/1/16/CC-BY_icon.svg/100px-CC-BY_icon.svg.png) |	署名（BY） |	BY|
|![CC-BY-SA 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d0/CC-BY-SA_icon.svg/100px-CC-BY-SA_icon.svg.png) |	署名（BY）- 相同方式共享（SA） |	BY-SA|
|![CC-by-NC 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/9/99/Cc-by-nc_icon.svg/100px-Cc-by-nc_icon.svg.png) |	署名（BY）- 非商业性使用（NC） |	BY-NC|
|![CC-BY-NC-SA 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Cc-by-nc-sa_icon.svg/100px-Cc-by-nc-sa_icon.svg.png) |	署名（BY）- 非商业性使用（NC）- 相同方式共享（SA） |	BY-NC-SA|
|![CC-BY-ND 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/1/16/Cc-by-nd_icon.svg/100px-Cc-by-nd_icon.svg.png) |	署名（BY）- 禁止演绎（ND） |	BY-ND|
|![CC-BY-NC-ND 图标](https://upload.wikimedia.org/wikipedia/commons/thumb/f/f1/Cc-by-nc-nd_icon.svg/100px-Cc-by-nc-nd_icon.svg.png) |	署名（BY）- 非商业性使用（NC）- 禁止演绎（ND） |	BY-NC-ND|

再次强调，这个是针对 **使用者** 的，创作者不受其约束，并且由于创作者拥有版权 (Copyright)，他可以单独对一些人进行授权，使他们可以不必遵守那些对公众的要求。


如何选择 CC 协议呢？可以通过下面这个链接生成对应的 CC 协议

https://creativecommons.org/choose/

一些常见 Q&A 的链接在这里

http://creativecommons.net.cn/faq/faq-user/


### [总结](https://en.wikipedia.org/wiki/Copyleft#Types_and_relation_to_other_licenses)
|公共领域 和 等价 |	许可许可证 |	Copyleft（保护性许可） |	非商业 许可证 |	专有许可证 |	商业秘密 |
|-|-|-|-|-|-|-|
|对于软件 |	PD、Unlicense 、CC0 |	BSD 、MIT 、Apache |	GPL 、AGPL |	JRL 、AFPL |	专有软件 ，无公共许可证 |	私人内部软件|
|对于其他创意作品 |	PD、CC0 |	CC-BY |	CC-BY-SA |	CC-BY-NC |	版权 ，无公共许可证 |	未发表 |

前三者 自由，后三者 非自由的

## End
其实吧我这篇文章是一两天前就想写的，一直拖到现在，今天正好是世界知识产权日，趁这个日子发出来这篇文章，虽然好晚了。
