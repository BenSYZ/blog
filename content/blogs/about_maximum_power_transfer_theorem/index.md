---
title: "about maximum power transfer theorem"
date: 2023-04-21T14:03:48+08:00
lastMod: 2023-09-30T14:57:54+08:00
code: true
mermaid: false
katex: true
draft: false
author: "Ben"
description: "about maximum power transfer theorem"
tags: ["physics"]
---

This is a read note of <https://en.wikipedia.org/wiki/Maximum_power_transfer_theorem>

## Maximum Efficiency
* Assumption:
    * direct current
    * load is pure resistor
* notation:
    + $P_\text{L}$ power of load
    + $R_\text{L}$ resistance of load.
    + $R_\text{S}$ resistance of source(e.g battery)

The efficiency is:

$$
\eta = \frac{P_\text{L}}{P_\text{total}}
= \frac{U_{\text{L}} \times I}{U_{\text{emf}} \times I}
= \frac{R_{\text{L}} \times I}{(R_\text{L} + R_\text{s}) \times I}
= \frac{1}{1+R_\text{s}/R_\text{L}}
$$

So when $R_\text{L}$ goes to $\infty$, $\eta$ will goes to $1$ . Which is as we already know about max efficiency.

## Maximum power transfer
But what if we do not care about efficiency($\eta$), we just want the power goes to maximum. What is the *power*? Energy provided per time ($P$). It's something like speed. While efficiency is like something capacity, like how much you have, like a volume quantity. So it would be better to redefine the efficiency from its initial meaning:
$$
\eta = \frac{E_\text{used}}{E_\text{provided}} 
$$

which is equal to $P_\text{L}/P_\text{total}$, as we can dividing time ($T$) both to numerator and denominator.

As we have know the difference between these two words, power($P$) and efficiency($\eta$). We will discuss the problem of this section name, which is that under what condition will the power be at its maximum?

$$
P_\text{L} = U_\text{L}  \times  I
$$

When $R_\text{L}$ goes from $0$ to $\infty$, $U_\text{L}$ will increase, but at the mean time, $I$ will decrease. So we cannot get the maximum of $P_\text{L}$ at first glance. Thus, it comes to numerical calculation:

Hold the previous assumptions:

* direct current
* load is pure resistor

$$
P_\text{L} = I ^2 R_\text{L}
= (\frac{U_\text{emf}}{R_\text{L} + R_\text{L}})^2 R_\text{L}
= \frac{U_\text{emf}}{R_\text{L}+2R_\text{s} + R_\text{s}^2 /R_\text{L}}
$$

$P_\text{L}$ max $\Rightarrow$ denominator to minumum

$$
\frac{d (\text{deno})}{dR_\text{L}} =1- \frac{R\text{s}^2}{R_\text{L}^2}
$$

Extreme point of original formula is when the above formula equals to $0$, which is when $R_\text{s} = R_\text{L}$ ($R>0$, omit the negative one). When $R_\text{L} < R_\text{s}$ the derivative is negative, So the original denominator decrease first, then increase. It means that when $R_\text{L} = R_\text{s}$, the original formula goes to it's maximum, which is
$$
P_\text{L} = \frac{U_\text{emf}}{4 R_\text{L}}
$$

While, at this time the efficiency $\eta = \frac{1}{2}$.

Besides, when the efficiency is maximum, the power is $0$

Here is a picture of "maximum power transfer graph" from Wikipedia:

![](./figures/Maximum_Power_Transfer_Graph.svg)

## AC case (TODO)
in AC we could use complex number to describe the impedance of capacitor and inductance, whose effects is shift the phases.

and we can convert the voltage to be effect voltage.
