---
title: "Vector Analysis 2"
date: 2021-09-16T00:30:00+08:00
draft: false
katex: true
author: "Ben"
description: "Vector analysis"
---

<!--
*
-->


Q1: Derive the formula for the volume of a sphere

$$
\begin{aligned}
V &=\int dV \\
    &= \int r^2\sin\theta dr d\theta d\phi\\
    &=\frac{1}{3} r^3|_{0}^{r} \times (-\cos\theta)|_{0}^{\pi}\times 2\pi\\
    &=\frac{4}{3}r^3
\end{aligned}
$$

$r^2\sin\theta$: it can also be derivative form Jacobi formula.

---

## Curvilinear Coordinates
### Spherical polar coordinates (SPC):
#### review

* 4 types integrate
* Curvilinear Coordinates
* $dl$, $dl_\theta$, $dl_\phi$ 对球坐标和柱坐标

#### Derivatives in SPC
##### 1. Gradient in the SPC
Definition of the gradient:

$$
df \equiv \frac{\partial f}{ \partial r} dr+ \frac{\partial f}{ \partial \theta} d\theta + \frac{\partial f}{ \partial \phi } d\phi
$$

We define $\nabla f$ as

$$
df = \nabla f \cdot  d\bold{l} = \nabla f \cdot ( dl_r \hat{r} + dl_\theta \hat{\theta} + dl_\phi \hat{\phi})
$$

because $dl$ defined as
$$
d\bold{l} \equiv dl_r \hat{r} + dl_\theta \hat{\theta} + dl_\phi \hat{\phi} = \hat{r} dr +  \hat{\theta}\sin\theta d\theta + \hat{\phi} r\sin\theta d\phi
$$

To meet the definition of $df$,

$$
\nabla f = \frac{\partial f}{ \partial r} \hat{r}+ \frac{1}{r}\frac{\partial f}{ \partial \theta} \hat{\theta} + \frac{1}{r\sin\theta} \frac{\partial f}{ \partial \phi} \hat{\phi}\\
= \frac{1}{h_1} dr + \frac{1}{h_2}d\theta + \frac{1}{h_3}d\phi
$$

##### 2. Divergence in the SPC
To get
$$
\nabla \cdot \bold{A}
$$

We need start from: $\int (\nabla \cdot \bold{A}) dV = \int_{\partial V} \bold{A} \cdot  d\bold{S}$

---

* $dS_z=\hat{x}dx \times \hat{y} dy=dxdy\hat{z}$

* So for a small cubic: $dS=dl_\phi dl_\theta\hat{r}+dl_r dl_\phi \hat{\theta} + dl_r dl_\theta\phi$

From the $\int \nabla \cdot \bold{A} dV = \int \bold{A} \cdot d\bold{S}$, we get $\nabla \cdot \bold{A} = \frac{1}{dV}(\bold{A} \cdot d\bold{S})$

1. 
$$
\nabla \cdot \bold{A} dV \equiv  \nabla \cdot \bold{A}  h_1 h_2 h_3 dr d\theta d\phi
$$
2. 
$$
\bold{A} \cdot d\bold{S} = \Delta_r(A_r dl_\theta dl_\phi) + \Delta_\theta(A_\theta dl_r dl_\phi) + \Delta_\phi (A_\phi dl_\theta dl_r)
$$
* 第一项是穿过$\theta \phi$ 面的通量

Then
$$
\nabla \cdot \bold{A} = \frac{1}{h_1h_2h_3} \left[\frac{\partial (A_r h_2 h_3)}{ \partial r} +\frac{\partial (A_\theta h_1 h_3)}{ \partial \theta} + \frac{\partial (A_\phi h_1h_2)}{ \partial \phi}  \right]
$$

Eg.
$$
\frac{1}{h_1h_2h_3} \frac{\partial A_rh_2h_3}{ \partial r} = \frac{1}{r^2\sin\theta} \frac{\partial \frac{1}{r^2} r^2\sin\theta}{ \partial r} 
=\frac{1}{r^2} \frac{\partial A_r r^2}{ \partial r} 
$$


##### 3. Curl in the SPC

矢量场中一个方向的分量和另外两个坐标有关，和标量场不同。比方说，沿着$y$ 轴向上，只有$y$ 在变，而此时，$x$ 方向的电场会变小。

To get $\nabla  \times \bold{A}$, we start from

$$
\int (\nabla \times  \bold{A}) \cdot  d\bold{S} = \oint_{\partial S} \bold{A} \cdot  d\bold{l}
$$

我们先只写 $r$ 方向的分量：

<img src="./figures/curl_in_SPC.svg" alt="curl_in_SPC" width=500>

1. 
$$
\nabla \times  A \cdot  d\bold{S}_r = \nabla \times A \ h_2h_3 d\theta d\phi \ \hat{\theta} \times \hat{\phi}
=(\nabla \times A)_r \ h_2h_3 d\theta d\phi
$$

2. 
$$
\bold{A}_r \cdot d\bold{l}_r = - \Delta_\phi(A_\theta h_2d\theta) + \Delta_\theta(A_\phi h_3 d\phi)
$$

---

$$
(\nabla \times \bold{A})_r = \frac{\bold{A}\cdot d\bold{l}}{dS}=- \frac{\Delta_\phi(A_\theta h_2) }{h_2h_3d\phi} + \frac{\Delta_\theta(A_\phi h_3)}{h_2h_3d\theta}
$$

* here the $dS=h_2h_3 d\theta d\phi$, which is a scalar.

So, curl in SPC

$$
\nabla \times \bold{A} = \frac{1}{h_1h_2h_3}
\begin{pmatrix}
h_1\hat{r} & h_2\hat{\theta} & h_3\hat{\phi}\\
\partial_r & \partial_\theta & \partial_\phi\\
h_1A_r & h_2A_\theta & h_3A_\phi
\end{pmatrix}
$$


---

Q2: Find the formula of curl in the **spherical polar coordinate**
$$
\nabla  \times A =
$$

---

## Dirac Delta Function

Consider the vector function $v=\frac{1}{r^2} \hat{r}$, from the very begin (problem 1.16):
$$
\begin{aligned}
\nabla \cdot \bold{v} &= \frac{\partial }{ \partial x} \left(\frac{x}{r^3}\right)
+ \frac{\partial }{ \partial y} \left(\frac{y}{r^3}\right)
+ \frac{\partial }{ \partial z} \left(\frac{z}{r^3}\right)\\
&=\frac{\partial }{ \partial x} \left[x(x^2+y^2+z^2)^{-\frac{3}{2}}\right]
+\frac{\partial }{ \partial y} \left[y(x^2+y^2+z^2)^{-\frac{3}{2}}\right]
+\frac{\partial }{ \partial z} \left[z(x^2+y^2+z^2)^{-\frac{3}{2}}\right]\\
&=()^{-\frac{3}{2}} + x(-\frac{3}{2})()^{-\frac{5}{2}}2x
+()^{-\frac{3}{2}} + y(-\frac{3}{2})()^{-\frac{5}{2}}2y
+()^{-\frac{3}{2}} + z(-\frac{3}{2})()^{-\frac{5}{2}}2z\\
&=3r^{-3}-3r^{-5}(x^2+y^2+z^2)=3r^{-3}-3r^{-3}=0
\end{aligned}
$$

or 

$$
\nabla \cdot \bold{v} = \frac{1}{r^2} \frac{\partial }{ \partial r} \left(r^2 \frac{1}{r^2}\right) = 0
$$

---

But

$$
\oint \bold{v} \cdot  d\bold{S}  = \int_{0}^{R} \left(\frac{1}{r^2}\hat{r}\right) \cdot  (R^2 \sin \theta d \theta d\phi \hat{r}) = 4\pi
$$
where,
$$
d\bold{S}=dl_\theta \hat{\theta} \times dl_\phi \hat{\phi} = R d\theta R \sin \theta d\phi \hat{r}
$$

The contradiction comes from the point $r = 0$, where $\frac{1}{r^2}$ blows up. So the point $r = 0$ contributes to the integration, giving the $4\pi$ result.

The **Answer** is that $\nabla \cdot \bold{v} =0$ everywhere *except* at the origin. The calculation of $\frac{\partial }{ \partial x} \left(\frac{x}{r^2}\right)$ at origin doesn't work. (or $\frac{\partial }{ \partial r} \left(r^2 \frac{1}{r^2}\right)$ not work at origin $\frac{0}{0}$).

---

* 空间角

$$
d\Omega \equiv \sin\theta d \theta d\phi
$$

* 来源：当r取R时，上面那个式子没有$r$了，就变成$\int d\Omega$了，或者说这个式子本身和$r$ 无关，或者说可以用在求flux上。

---


### Define Dirac function

#### 1D:

$$
\delta (x-a) =
\begin{cases}
0, &\text{ if } x\neq a \\
\infty &\text{ if }x = a \\
\end{cases}
\text{ and}
\int_{-\infty}^{+\infty} \delta(x-a)dx=1
$$

#### 3D

$$
\nabla \cdot \left(\frac{1}{r^2}\hat{\bold{r}}\right) \equiv  \frac{1}{4\pi}\delta(r)
$$

#### 性质：

$$
\int_{-\infty}^{+\infty} f(x) \delta(x-a) dx =f(a)
$$
$$
\delta(kx) = \frac{1}{|k|} \delta(x)
$$

LS变瘦了,原来的$1$，现在$\frac{1}{k}$ 就够了

$$
\int_{a}^{b} f(x) \delta[ g(x) - g(x_0) ] dx = f(x_j) / \left|\frac{dg}{dx}\right||_{x=x_j}
$$

## The Theory of Vector Fields
### Helmholtz's Theorem: 
Maxwell reduce the $E$ and $B$ into four equations, and this four equations are all about the divergence and the curl of $\bold{E}$ and $\bold{B}$. So here raise an important mathematical question: To what extent is a vector function determined by its divergence and curl?(一个向量函数在多大程度上是由它的散度和卷度决定的), 即给一个向量函数的散度和旋度，是否能得到这个向量函数。

事实上，是不行的，需要一个边界条件，Helmholtz theorem 告诉说给一些边界条件之后可以唯一确定这个函数。

---

数学的表示：
the *divergence* of $\bold{F}$ (which stands for $\bold{E}$ or $\bold{B}$) is a specified (scalar) function D,
$$
\nabla \cdot \bold{F} = D
$$

and the curl of $\bold{F}$ is a specified (vector) function $\bold{C}$,

$$
\nabla \times \bold{F}=\bold{C}
$$

当然 $\bold{C}$ must be divergenceless, $\nabla \cdot \bold{C}=0$，旋度的散度为$0$

---

Eg:
divergence =0 , curl = 0, 的解：$\bold{F}=\bold{0}$ $F=yz \hat{\bold{x}} + zx \hat{\bold{y}} + xy \hat{\bold{z}}$ $F=\sin x\cosh y \hat{\bold{x}} - \cos x \sinh \hat{\bold{y}}$

---

### Potentials
#### Curl-less field
If the curl of a vector field($\bold{F}$) vanishes(everywhere), then $\bold{F}$ can be written as gradient of a **Scalar potential**($V$):

$$
\nabla \times \bold{F} = 0\Leftrightarrow \bold{F} = - \nabla V.
$$

* Theorem 1 **Curl-less** ("**irrotational**") **fields**:(保守场)
    + $\nabla \times \bold{F} = 0$ everywhere
    + $\int_a^{b}  \bold{F} \cdot  d\bold{l}$ is independent of path, for any given end points.
    + $\oint\bold{F} \cdot  d\bold{l} =0$ for any closed loop
    + $\bold{F}$ is gradient of some scalar, $\bold{F} = -\nabla V$(+ any constant)

#### Divergence-less field
If the divergence of a vector field ($\bold{F}$) vanishes(everywhere), then $\bold{F}$ can be expressed as the curl of **Vector potential**(**A**):
$$
\nabla \cdot \bold{F} =0 \Leftrightarrow \bold{F} = \nabla \times \bold{A}
$$

* Theorem 2 **Divergence-less** (or "**solenoidal**") **fields**:
    + $\nabla \cdot  \bold{F} = 0$ everywhere
    + $\int  \bold{F} \cdot  d\bold{a}$ is independent of surface, for any given bound.
    + $\oint\bold{F} \cdot  d\bold{a} =0$ for any closed surface
    + $\bold{F}$ is curl of some scalar, $\bold{F} = -\nabla \times  \bold{A}$(+gradient of any scalar function)


Any vector field $\bold{F}$ can be written as gradient of a scalar of a scalar plus the curl of a vector:
$$
\bold{F} = -\nabla V + \nabla \times A
$$



Any vector field resolved into curl-free vector field and divergence-free vector field

* curl free: $\nabla \times (\nabla \phi) = 0$
* divergence free: $\nabla \cdot  (\nabla \times \vec{A}) = 0$

---

* $\nabla \cdot  F= D$ and $\nabla \times  F = C$
* physical: $\nabla \cdot (\nabla \phi) =\nabla \cdot E$ and $\nabla \times A = \vec{B}$



---

Q3: In the following, $p$ is a constant vector, and $r=x\hat{x}+y\hat{y}+z\hat{z}$
$$
\begin{aligned}
\nabla \cdot r\\
\nabla(r \cdot r)\\
\end{aligned}
$$
and
$$
\begin{aligned}
\nabla p \cdot r\\
\end{aligned}
$$

Solve:
$$
\nabla \cdot  r =\nabla \cdot r = 3
$$

$$
\nabla(r \cdot  r) = 2x\hat{x} +2y \hat{y} +2z \hat{z} = 2 r
$$


$$
\nabla p \cdot  r = \nabla (p_1x + p_2y+p_3z) = p_1
$$

