---
title: "about effective voltage and rms"
description: "about effective voltage and rms"
date: 2023-04-21T15:18:29+08:00
lastMod: 2023-04-21T16:54:56+08:00
code: true
mermaid: false
katex: true
draft: false
author: "Ben"
description: "about effective voltage and rms"
tags: ["physics", "formula"]
---

## Effective voltage of sinusoidal

In alternating current, to simplify the calculation, we often use effective voltage or current.

Since I haven't touched these knowledge points for a long time. I almost forgot how to get the effective voltage and what's the relation of effective voltage with rms (root mean square). So this is about the derivation of effective voltage.

Effective voltage is always used in calculating power or energy during a time. So we start from the expression of power, because voltage and current change with time, we first calculate the energy in time $dt$, then integrate.

$$
P(t) = U(t)^2 / R
$$

Energy expose in $dt$ is

$$
P(t) dt = \frac{1}{R} U(t)^2dt
$$

Integrate it:

$$
E_\text{one period} = \int_{0}^{T} \frac{1}{R} U(t)^2 dt
$$

While $U(t)$ is sinusoidal function in A.C we assume it to be $U(t) = U_\text{m}\sin(2\pi/T  \times t)$(when $t=T$, sine goes to its end of one period $2\pi$), in which $U_\text{m}$ is maximum voltage, or we can call it amplitude, thus,

$$
\begin{aligned}
E_\text{one period}
    &= \frac{U_\text{m}^2}{R} \int_{0}^{T} \sin^2(\frac{2\pi}{T}t) dt \\
    &=  \frac{U_\text{m}^2}{R}  \times \frac{1}{2} \int_{0}^{T} (1-\cos(2\frac{2\pi}{T}t)) dt \\
    &=  \frac{U_\text{m}^2}{2R}\left(T-0 + \frac{T}{4\pi} \sin(\frac{4\pi}{T} \times T) -0\right)  \\
    &=\frac{U_\text{m}^2}{2R} \times  T
\end{aligned}
$$

What is a effective voltage? With this we can treat AC as DC, thus

$$
E_\text{one period} = U_\text{eff}^2 / R  \times T
$$

Thus for a sinusoidal voltage, 

$$
U_\text{eff} =  \sqrt{R E_\text{one period}/T} = \sqrt{R \frac{U_\text{m}^2}{2R} \times T /T}
= \frac{1}{\sqrt{2}} U_m
$$


### RMS
If we not introduce sinusoidal, the effective U is
$$
U_\text{eff} =  \sqrt{R E_\text{one period}/T} = \sqrt{\frac{1}{T} \int_{0}^{T}U(t)^2dt}
$$

which happens to be the root mean square:

$$
U_\text{rms} \equiv  \sqrt{\frac{\sum U^2}{n}}
$$

or

$$
U_\text{rms} \equiv  \sqrt{ \frac{1}{T} \int_{0}^{T} U(t)^2 dt }
$$

So in general, if we want to calculate a quantity which is related to square periodic quantity, we can use the rms as its "average"(effective) variable to simplify it.

$$
z \propto Y(x)^2
$$

where $z$ is a quantity we want to get, $Y(x)$ is periodic, we can use its rms which does not vary with $x$, to simplify the calculation.

$$
z \propto Y_\text{rms}^2
$$

I feel that we've been talk rms is very useful, but now I cannot find it...

