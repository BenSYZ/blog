---
title: "asm x86 arg"
draftDate: 2026-04-26T20:43:06+08:00
date: 2026-04-26T20:43:06+08:00
lastMod: 2026-04-26T20:43:06+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "asm x86 arg"
tags: [ "asm" ]
---


内核栈还是用户栈都是从高位往地位变化的，只是用户是从中间往下，内核是从顶上往下

## 传参

```c
// #include <stdio.h>
int fn( int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8){
	int minus_result=a8-a1;
	return minus_result;
	// printf("%d\n", minus_result);
}

int main(){
	int var=0xF;
	var = fn(0, 1, 2, 3, 4, 5, 6, 7, 8);
	return 0;
}
```

```asm
fn:
	pushq	%rbp
	movq	%rsp, %rbp

	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -28(%rbp)
	movl	%ecx, -32(%rbp)
	movl	%r8d, -36(%rbp)
	movl	%r9d, -40(%rbp)
	movl	32(%rbp), %eax
	subl	-24(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
main:
	pushq	%rbp
	movq	%rsp, %rbp

	subq	$16, %rsp
	movl	$15, -4(%rbp) # 0xF

# 把 8 7 6 压入栈，前 6 个用寄存器存
	pushq	$8
	pushq	$7
	pushq	$6
	movl	$5, %r9d
	movl	$4, %r8d
	movl	$3, %ecx
	movl	$2, %edx
	movl	$1, %esi
	movl	$0, %edi
# call fn
	call	fn
	addq	$24, %rsp
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	leave
	ret
```

因为这里是 32 位的 int，所以用的是 edi，如果改成 UL，就会是 rdi 了

```c
#define ull unsigned long
ull fn( ull a0, ull a1, ull a2, ull a3, ull a4, ull a5, ull a6, ull a7, ull a8){
	ull minus_result=a8-a1;
	return minus_result;
}

int main(){
	ull var=0xFUL;
	ull arg = 0xA5UL;
	var = fn(arg, arg, arg, arg, arg, arg, arg, arg, arg);
	return 0;
}
```

```asm
	movq	$165, -8(%rbp)
	movq	-8(%rbp), %r8
	movq	-8(%rbp), %rdi
	movq	-8(%rbp), %rcx
	movq	-8(%rbp), %rdx
	movq	-8(%rbp), %rsi
	movq	-8(%rbp), %rax
	pushq	-8(%rbp)
	pushq	-8(%rbp)
	pushq	-8(%rbp)
```


regparm


>  rdi is an actual register, which is different from rdx. rdi is the daddy of edi, grand-daddy of di, and the brother of rsi. rdi and rsi once stood for Destination and Source Index registers, but nowadays can generally be used like the other general-purpose registers. However, rdi doesn't have an equivalent to DX's 8-bit upper half (DH). – 
Iwillnotexist Idonotexist Commented Jun 10, 2014 at 0:09
> https://stackoverflow.com/q/24130930/13033234

### 栈上顺序

后面的参数，先压，压在最上面
前面的参数，后压，压在最下面

```
#define ull unsigned long
ull fn( ull a0, ull a1, ull a2, ull a3, ull a4, ull a5, ull a6, ull a7, ull a8){
	ull minus_result=a8-a1;
	ull s1 =1 ;
	ull s2 =2 ;
	ull s3 =3 ;
	ull s4 =4 ;
	ull s5 =5 ;
	ull s6 =6 ;
	ull s7 =7 ;
	ull s8 =8 ;
	ull s9 =9 ;
	ull s10=10;
	return minus_result;
	// printf("%d\n", minus_result);
}

int main(){
	ull var=0xFUL;
	ull arg = 0xA5UL;
	ull s1 =1 ;
	ull s2 =2 ;
	ull s3 =3 ;
	ull s4 =4 ;
	ull s5 =5 ;
	ull s6 =6 ;
	ull s7 =7 ;
	ull s8 =8 ;
	ull s9 =9 ;
	ull s10=10;
	var = fn(s1, s2, s3, s4, s5, s6, s7, s8, s9);
	int a=10;
	int b=10;
	a=a-b;
	return 0;
}

```



### TODO
mov    -0x28(%rbp),%r8     
mov    -0x30(%rbp),%rdi    
mov    -0x38(%rbp),%rcx    
mov    -0x40(%rbp),%rdx    
mov    -0x48(%rbp),%rsi    
mov    -0x50(%rbp),%rax    
push   -0x10(%rbp)         
push   -0x18(%rbp)         
push   -0x20(%rbp)         
mov    %r8,%r9             
mov    %rdi,%r8            
mov    %rax,%rdi           
call   0x555555555119 <fn>

下面这几坨干啥的
