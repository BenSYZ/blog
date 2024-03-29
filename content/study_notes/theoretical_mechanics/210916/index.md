---
title: "210916"
date: 2021-09-16T12:30:00+08:00
katex: true
draft: false
---


## Review
$F=ma$ 推出一些东西 动量定理...


最小做用量原理，路径积分


## Variational Method(变分法)
differential calculus ($d$,微分) $\to$ variational calculus($\delta$,变分)

泛函，把函数变一点，区别于之前的微分是$x+\delta x$

微分
$$
f(x+\delta x) = (x+\delta x)^2 = x^2 +2 \delta x x + (\delta x)^2
$$

$$
\frac{dx^2}{dx} =2x
$$

变分：
函数变化，basis change, eg. Laplace and zeta 作基底

Define: $\tilde{f}(x) = f(x)+\alpha \eta(x)$ as **a set of function**(泛函)

$f(x)$ change little can be written as:
$$
f(x) + \delta \eta(x)
$$
* treat $f(x)$ as variable, $f(x)$ self-increasing

<!--

$$
\delta \tilde{f}(x) = (f+d \alpha \eta)(x) - f(x) = d \alpha \eta(x) \equiv \delta f d \alpha
$$

-->
$$
\delta \tilde{f}(x) = (f+d \alpha \eta)(x) - f(x) = d \alpha \eta(x) \equiv \frac{\partial \tilde{f}}{ \partial x} d \alpha
$$

??? 有些问题，看一下讲义$f$ 自增然后呢？

* $f(x)$ change a little along the "direction" of function $\eta(x)$
    + $f(x)=x^2$, $\eta(x)=x^3-x^2$, the form of $f(x)$ is closing to $x^3$

----

### Partial derivative:
differential:
$$
df(x,y) = \frac{\partial f}{ \partial x} dx + \frac{\partial f}{ \partial y} dy
$$

variational, $F(x,\tilde{y})$:

$$
F(x,y+d \alpha\eta) = F(x,y) + \frac{\partial F}{ \partial \tilde{y}} d \alpha \eta = F(x,y) + \frac{\partial F}{ \partial \tilde{y}} \delta \tilde{y}
$$

so we can write:

$$
\delta F(x,\tilde{y}) = F(x,y+d \alpha \eta) - F(x,y) = \frac{\partial F}{ \partial \tilde{y}}  \delta \tilde{y}
$$


???

* **Note**: 微分变的是自变量x，自增，但是变分变的是函数的形式，

---

### Application:

Some problems:

* Brathistochrone curve 最速降线
* 等时降落问题
* Plateau 问题 肥皂泡
* 等周长
* 测地线
* 费马原理

---

#### brachistochrone curve(最速曲线)

two points(0,0), and $(x_0,y_0)$

* any block length = $dS=\sqrt{dx^2 + dy^2}$
* speed through this is $v=\sqrt{2gy}$

$$
T=\int dt = \int \frac{dS(y)}{v(y)} = \int_{0}^{x_0} \frac{\sqrt{1+y'^2}}{\sqrt{2gy}} dx
$$

同除以$dx$

What to do next is get the extreme value of the T.

we don't know the functions formula, so we change the value of function f, and to see if time will change or not.

Assume $F(y,y')=\sqrt{\frac{1+y'^2}{2gy}}$

$$
\delta T = \int_{0}^{x_0} \left[\frac{\partial F}{ \partial y} \delta y + \frac{\partial F}{ \partial y'} \delta y'\right] dx
$$

把第二项拆了，分部积分

---

分部积分：
$$
\int f(x) g'(x) dx = \int f(x) dg(x) = g(x)f(x) - \int g(x) f'(x)dx
$$


---

$$
\delta T = \left[\frac{\partial F}{ \partial y'}\delta y \right]_{0}^{x_0} +
\int_{0}^{x_0 }\left[ \frac{\partial F}{ \partial y} - \frac{d}{dx}\left(\frac{\partial F}{ \partial y'}\right)\right] \delta y dx
$$

* 第一项是0，原因$\delta y|_0=0$, $\delta y|_{x_0}=0$。而$\delta y$指的是新函数和原函数的差值，而我们的要求是初末相同，找中间的路径最优解。

![delta_xy0](./figures/delta_xy0.svg)


So formula in $[...]$ is 0, which is **Euler formula**(1st )(Euler Lagrange Formula)

$$
\frac{\partial F}{ \partial y} - \frac{d}{dx}\left(\frac{\partial F}{ \partial y'}\right) = 0
$$

* 时间积分to 位置积分
* 变分变微分

稍微变一下，**Euler formula**(2nd )

$$
\frac{\partial F}{ \partial x} - \frac{d}{dx} \left(F-y'\frac{\partial F}{ \partial y'} \right) = 0
$$

* 当$F$ 不显含$x$，括号里的那项是常数。

由于，$F(y,y') = \sqrt{\frac{1+y^2}{2gy}}$ and $\frac{\partial F}{ \partial y} - \frac{d}{dx} \left(F-y'\frac{\partial F}{ \partial y'} \right)=0$.

$$
\left(F-y'\frac{\partial F}{ \partial y'} \right)=C=\sqrt{\frac{1+y'^2}{2gy}}\left(1-y'\frac{y'}{1+y'^2}\right) \\
\Rightarrow y(1+y'^2) = c'
$$

Assume, $y'= \cot \alpha$, $2\alpha=\theta$, we have

$$
x=k(\theta - \sin \theta)\\
y=k(1-\cos\theta)
$$

$$
(x,y) = k(\theta,1) - k(\sin\theta,\cos\theta)
$$

一个圆在直线上的无摩擦滚动，旋轮线、最速曲线，等时线(球放在任意位置，落到底部的时间相同)，是真正的摆线


----

## The Principle of Least Action(最小做用量原理)

### From Newton to Lagrange

* Newton mechanisms:

    + 2 particles position 1D: two lines

<!--
, we don't know what will happen in the next time

2 positions, with extra $(x',y')$

-->


* Lagrange mechanisms:

    + two particles 1 line, to show their position change
    + *configuration space*
    + High dimension to see such system

(Hamilton: *phase space*)


$N$ particles with 3 coordinates $\rightarrow$ 1 point with $3N$ coordinates

#### Lagrange's equation

Define the **Lagrangian(拉格朗日量)** $\mathcal{L}$ and **action(做用量)** $\mathcal{S}$ as
$$
\mathcal{L}(x^{A},\dot{x}^{A}) = T(\dot{x}^{A}) - V(x^{A})
$$
* 动能减势能

$$
\mathcal{S}[x^{A}(t)] = \int_{t_i}^{t_f}\mathcal{L}(x^{A}(t),\dot{x}^{A}(t))dt
$$

* 之前是求$F$，然后求我们关心的$T$，从$0$ to $t_1$，使 $T$ 取极值($\delta T=0$)，以使 $F$ 是最优解

* x - t
* T - $\mathcal{S}$
* 






