---
title: "about nested struct and anonymous struct in c"
date: 2023-05-20T16:55:23+08:00
lastMod: 2023-05-20T16:55:23+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "c_struct"
tags: ["program", "c", "syntax"]
---

看书的时候没明白一个 macro 的用法，搜了一下，发现这个[问题](https://stackoverflow.com/questions/58346036/c-c-macro-parameter-containing-dot-member-access-operator)，发现看不懂他写的结构体的问题。自己尝试写了几个例子发现了 anonymous struct，觉得用法很奇怪，又搜到了这个[问题](https://stackoverflow.com/questions/38457109/c-how-to-access-different-types-of-anonymous-or-unnamed-nested-structs/38457240)，一下把我能想到的关于结构体的情况都写出来了

```c
struct s {
    int abc;

    struct {
        int a;
    };

    struct {
        int b;
    } intern;

    struct i {
        int c;
    };

    struct i2 {
        int d;
    } intern2;

    struct i3 {
        int e;
    };
    struct i3 intern3;
};

struct i3 AA;

int main(int argc, char const *argv[])
{
    struct s mystruct;

    mystruct.abc = 0;
    mystruct.a = 1;
    mystruct.intern.b = 2;
    mystruct.c = 3; // <-- does not compile
    mystruct.intern2.d = 4;
    mystruct.intern3.e = 5;
    AA.e=0;

    return 0;
}

```

两个回答都很好：

> Regarding anonymous structure, quoting C11, chapter §6.7.2.1, (emphasis mine)
>
> > An unnamed member whose type specifier is a structure specifier with no tag is called an anonymous structure; an unnamed member whose type specifier is a union specifier with no tag is called an anonymous union. The members of an anonymous structure or union are considered to be members of the containing structure or union. This applies recursively if the containing structure or union is also anonymous.

说明了 anonymous struct 访问内部变量这种奇怪的 `.` 法。

> Member c is declared inside inside strut i. And you are not creating any variable for struct i

这个回答看起来没说什么，其实说明了 `struct i` 在 struct 内部的声明（可能不叫声明）等同于放在外面，也解释了 warning 的问题。（其实不等同，不然不会有这个 warning 了）

[example.c](./res/example.c)


