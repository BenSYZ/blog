## Flexible Array Member (C99)

> C99 has a feature called the flexible array member. It lets you declare a structure for which the last member is an array with special properties. One special property is that the array doesn’t exist—at least, not immediately. The second special property is that, with the right code, you can use the flexible array member as if it did exist and has whatever number of elements you need. This probably sounds a little peculiar, so let’s go through the steps of creating and using a structure with a flexible array member.

<https://raw.githubusercontent.com/hmsjy2017/free-ebooks/master/C%20Primer%20Plus%20(6th%20Edition).pdf>

如果在栈上用，会导致踩内存

```c
//flexible array member
#include <stdio.h>
#include <string.h>
int main(){
    char a[2]="0\0";
    char b[11]="0123456789\0";
    printf("a=%s, b=%s\n", a, b);
    printf("addr_a=%p, addr_b=%p\n", &a, &b);
    memcpy(a,b,sizeof(b));              // warning
    printf("addr_a=%p, addr_b=%p\n", &a, &b);
    printf("a=%s, b=%s\n", a, b);
    return 0;
}
```

一般是在堆上

## sockaddr 的时候


上面的说法也不完全正确，一般用是用 `sockaddr_in` `sockaddr_in6` 设置值，然后地址强转成 (sockaddr*) 传入系统 API。所以值是在栈上的，但是内容是由正确的 type 占位的，并没有把一个大内存塞入小变量，不存在踩内存。

也就是内存不变，变的只是类型，对这块内存的标志，这样的好处是统一接口，当然也可以是 `void *` 不过难懂。

我们没有用 `sockaddr` 申请栈上的变量，而只是强制转成了 sockaddr

TODO:
sockaddr 和 Flexible Array Member 没什么关系（ https://stackoverflow.com/questions/32624847/what-is-the-purpose-of-the-sa-data-field-in-a-sockaddr ）
https://stackoverflow.com/questions/17185247/c-struct-array-member-without-specific-length

一般的用法是：
```
sockaddr_in a;
(sockaddr*)&a;
```

