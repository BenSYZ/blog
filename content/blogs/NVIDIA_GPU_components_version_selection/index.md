---
title: "NVIDIA GPU components version selection"
draftDate: 2025-08-29T17:13:00+08:00
date: 2025-09-02T16:17:32+08:00
lastMod: 2025-09-02T16:17:32+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "nvidia components version selection"
tags: ["NVIDIA", "CUDA", "PyTorch", "PCIe"]
---

[toc]

## Big Picture

| Components Level | Examples                            |
|------------------|-------------------------------------|
| PyTorch          | `import torch`                      |
| CUDA Toolkit     | `nvcc`                              |
| CUDA driver      | `libcuda.so`                        |
| device driver    | `nvidia.ko`                         |
| GPU hardware     | Architecture and Compute Capability |

各个层之间以 ABI 对接

![[CUDA-components (by NVIDIA)](https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/_images/CUDA-components.png)](./figures/CUDA-components.png)

## GPU hardware and Compute Capability(CC)

> Compute Capability (CC) defines the hardware features and supported
> instructions for each NVIDIA GPU architecture. 

Compute Capability 其实就是指令集，可能高等级的指令集比低等级的指令更多。硬件定下来了，指令集也就定下来了。

| Compute Capability Major Revision Number | NVIDIA GPU Architecture           |
|------------------------------------------|-----------------------------------|
| 12                                       | NVIDIA Blackwell GPU Architecture |
| 11                                       | NVIDIA Blackwell GPU Architecture |
| 10                                       | NVIDIA Blackwell GPU Architecture |
| 9                                        | NVIDIA Hopper GPU Architecture    |
| 8                                        | NVIDIA Ampere GPU Architecture    |
| 7                                        | NVIDIA Volta GPU Architecture     |
| 6                                        | NVIDIA Pascal GPU Architecture    |
| 5                                        | NVIDIA Maxwell GPU Architecture   |
| 3                                        | NVIDIA Kepler GPU Architecture    |

| Compute Capability | NVIDIA GPU Architecture        | Based On                      |
|--------------------|--------------------------------|-------------------------------|
| 7.5                | NVIDIA Turing GPU Architecture | NVIDIA Volta GPU Architecture |
| 8.9                | NVIDIA Ada GPU Architecture    |                               |

> Compared to Ampere, Ada delivers more than double the FP16, BF16, TF32, INT8, and INT4 Tensor TFLOPS, and
> also includes the Hopper FP8 Transformer Engine, delivering over 1.45 PetaFLOPS of tensor processing in the
> RTX 6000 Ada Generation.

[Ref](https://images.nvidia.com/aem-dam/Solutions/geforce/ada/nvidia-ada-gpu-architecture.pdf)

关于 Blackwell，从 “Compute Capability 和显卡”中我们也可以看到，10 是 for data center， 11 是 Jetson， 12 是 GeForce/RTX

---

* Refs:
    * [compute-capability](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#compute-capability)
    * [NVIDIA Ada GPU architecture (compute capability 8.9)](https://docs.nvidia.com/cuda/ada-compatibility-guide/#building-applications-using-cuda-toolkit-11-8)
    * [Blackwell architecture (compute capability 10.0)](https://docs.nvidia.com/cuda/blackwell-compatibility-guide/#building-applications-using-cuda-toolkit-12-8)

### ABI 兼容性
对于 cubin (cuda binary) 用的 CC 版本和运行时 GPU 的 CC，也就是编译时的 CC 和运行时的 CC，这两种 ABI：

* 大版本之间是不兼容的
* 小版本之间，低版本的 cubin 可以跑到高版本 GPU 上

---

* Refs:
    * <https://docs.nvidia.com/cuda/ampere-compatibility-guide/>

### Compute Capability 和显卡

| Compute Capability | Data Center                                  | GeForce/RTX                                             | Jetson            |
|--------------------|----------------------------------------------|---------------------------------------------------------|-------------------|
| 12 (Blackwell)     | NVIDIA RTX PRO 6000 Blackwell Server Edition | NVIDIA RTX PRO 6000 Blackwell Workstation Edition       |                   |
|                    |                                              | NVIDIA RTX PRO 6000 Blackwell Max-Q Workstation Edition |                   |
|                    |                                              | NVIDIA RTX PRO 5000 Blackwell                           |                   |
|                    |                                              | NVIDIA RTX PRO 4500 Blackwell                           |                   |
|                    |                                              | NVIDIA RTX PRO 4000 Blackwell                           |                   |
|                    |                                              | GeForce RTX 5090                                        |                   |
|                    |                                              | GeForce RTX 5080                                        |                   |
|                    |                                              | GeForce RTX 5070 Ti                                     |                   |
|                    |                                              | GeForce RTX 5070                                        |                   |
|                    |                                              | GeForce RTX 5060 Ti                                     |                   |
|                    |                                              | GeForce RTX 5060                                        |                   |
|                    |                                              | GeForce RTX 5050                                        |                   |
| 11.0               |                                              |                                                         | Jetson T5000      |
|                    |                                              |                                                         | Jetson T4000      |
| 10 (Blackwell)     | NVIDIA GB200                                 |                                                         |                   |
|                    | NVIDIA B200                                  |                                                         |                   |
| 9 (Hopper)         | NVIDIA GH200                                 |                                                         |                   |
|                    | NVIDIA H200                                  |                                                         |                   |
|                    | NVIDIA H100                                  |                                                         |                   |
| 8.9 (Ada)          | NVIDIA L4                                    | NVIDIA RTX 6000 Ada                                     |                   |
|                    | NVIDIA L40                                   | NVIDIA RTX 5000 Ada                                     |                   |
|                    |                                              | NVIDIA RTX 4500 Ada                                     |                   |
|                    |                                              | NVIDIA RTX 4000 Ada                                     |                   |
|                    |                                              | NVIDIA RTX 4000 SFF Ada                                 |                   |
|                    |                                              | NVIDIA RTX 2000 Ada                                     |                   |
|                    |                                              | GeForce RTX 4090                                        |                   |
|                    |                                              | GeForce RTX 4080                                        |                   |
|                    |                                              | GeForce RTX 4070 Ti                                     |                   |
|                    |                                              | GeForce RTX 4070                                        |                   |
|                    |                                              | GeForce RTX 4060 Ti                                     |                   |
|                    |                                              | GeForce RTX 4060                                        |                   |
|                    |                                              | GeForce RTX 4050                                        |                   |
| 8.7                |                                              |                                                         | Jetson AGX Orin   |
|                    |                                              |                                                         | Jetson Orin NX    |
|                    |                                              |                                                         | Jetson Orin Nano  |
| 8.6                | NVIDIA A40                                   | NVIDIA RTX A6000                                        |                   |
|                    | NVIDIA A10                                   | NVIDIA RTX A5000                                        |                   |
|                    | NVIDIA A16                                   | NVIDIA RTX A4000                                        |                   |
|                    | NVIDIA A2                                    | NVIDIA RTX A3000                                        |                   |
|                    |                                              | NVIDIA RTX A2000                                        |                   |
|                    |                                              | GeForce RTX 3090 Ti                                     |                   |
|                    |                                              | GeForce RTX 3090                                        |                   |
|                    |                                              | GeForce RTX 3080 Ti                                     |                   |
|                    |                                              | GeForce RTX 3080                                        |                   |
|                    |                                              | GeForce RTX 3070 Ti                                     |                   |
|                    |                                              | GeForce RTX 3070                                        |                   |
|                    |                                              | GeForce RTX 3060 Ti                                     |                   |
|                    |                                              | GeForce RTX 3060                                        |                   |
|                    |                                              | GeForce RTX 3050 Ti                                     |                   |
|                    |                                              | GeForce RTX 3050                                        |                   |
| 8 (Ampere)         | NVIDIA A100                                  |                                                         |                   |
|                    | NVIDIA A30                                   |                                                         |                   |
| 7.5 (Turing)       | NVIDIA T4                                    | QUADRO RTX 8000                                         |                   |
|                    |                                              | QUADRO RTX 6000                                         |                   |
|                    |                                              | QUADRO RTX 5000                                         |                   |
|                    |                                              | QUADRO RTX 4000                                         |                   |
|                    |                                              | QUADRO RTX 3000                                         |                   |
|                    |                                              | QUADRO  T2000                                           |                   |
|                    |                                              | NVIDIA T1200                                            |                   |
|                    |                                              | NVIDIA T1000                                            |                   |
|                    |                                              | NVIDIA T600                                             |                   |
|                    |                                              | NVIDIA T500                                             |                   |
|                    |                                              | NVIDIA T400                                             |                   |
|                    |                                              | GeForce GTX 1650 Ti                                     |                   |
|                    |                                              | NVIDIA TITAN RTX                                        |                   |
|                    |                                              | GeForce RTX 2080 Ti                                     |                   |
|                    |                                              | GeForce RTX 2080                                        |                   |
|                    |                                              | GeForce RTX 2070                                        |                   |
|                    |                                              | GeForce RTX 2060                                        |                   |
| 7.2                |                                              |                                                         | Jetson AGX Xavier |
|                    |                                              |                                                         | Jetson Xavier NX  |
| 7 (Volta)          | NVIDIA V100                                  | Quadro GV100                                            |                   |
|                    |                                              | NVIDIA TITAN V                                          |                   |
| 6.2                |                                              |                                                         | Jetson TX2        |
| 6.1                | Tesla P40                                    | Quadro P6000                                            |                   |
|                    | Tesla P4                                     | Quadro P5200                                            |                   |
|                    |                                              | Quadro P5000                                            |                   |
|                    |                                              | Quadro P4200                                            |                   |
|                    |                                              | Quadro P4000                                            |                   |
|                    |                                              | Quadro P3200                                            |                   |
|                    |                                              | Quadro P3000                                            |                   |
|                    |                                              | Quadro P2200                                            |                   |
|                    |                                              | Quadro P2000                                            |                   |
|                    |                                              | Quadro P1000                                            |                   |
|                    |                                              | Quadro P620                                             |                   |
|                    |                                              | Quadro P600                                             |                   |
|                    |                                              | Quadro P500                                             |                   |
|                    |                                              | Quadro P400                                             |                   |
|                    |                                              | P620                                                    |                   |
|                    |                                              | P520                                                    |                   |
|                    |                                              | NVIDIA TITAN Xp                                         |                   |
|                    |                                              | NVIDIA TITAN X                                          |                   |
|                    |                                              | GeForce GTX 1080 Ti                                     |                   |
|                    |                                              | GeForce GTX 1080                                        |                   |
|                    |                                              | GeForce GTX 1070 Ti                                     |                   |
|                    |                                              | GeForce GTX 1070                                        |                   |
|                    |                                              | GeForce GTX 1060                                        |                   |
|                    |                                              | GeForce GTX 1050                                        |                   |
| 6 (Pascal)         | Tesla P100                                   | Quadro GP100                                            |                   |
| 5.3                |                                              |                                                         | Jetson Nano       |
| 5.2                | Tesla M60                                    | Quadro M6000 24GB                                       |                   |
|                    | Tesla M40                                    | Quadro M6000                                            |                   |
|                    |                                              | Quadro M5000                                            |                   |
|                    |                                              | Quadro M4000                                            |                   |
|                    |                                              | Quadro M2000                                            |                   |
|                    |                                              | Quadro M5500M                                           |                   |
|                    |                                              | Quadro M2200                                            |                   |
|                    |                                              | Quadro M620                                             |                   |
|                    |                                              | GeForce GTX TITAN X                                     |                   |
|                    |                                              | GeForce GTX 980 Ti                                      |                   |
|                    |                                              | GeForce GTX 980                                         |                   |
|                    |                                              | GeForce GTX 970                                         |                   |
|                    |                                              | GeForce GTX 960                                         |                   |
|                    |                                              | GeForce GTX 950                                         |                   |
|                    |                                              | GeForce GTX 980M                                        |                   |
|                    |                                              | GeForce GTX 970M                                        |                   |
|                    |                                              | GeForce GTX 965M                                        |                   |
|                    |                                              | GeForce 910M                                            |                   |
| 5 (Maxwell)        |                                              | Quadro K2200                                            |                   |
|                    |                                              | Quadro K1200                                            |                   |
|                    |                                              | Quadro K620                                             |                   |
|                    |                                              | Quadro M1200                                            |                   |
|                    |                                              | Quadro M520                                             |                   |
|                    |                                              | Quadro M5000M                                           |                   |
|                    |                                              | Quadro M4000M                                           |                   |
|                    |                                              | Quadro M3000M                                           |                   |
|                    |                                              | Quadro M2000M                                           |                   |
|                    |                                              | Quadro M1000M                                           |                   |
|                    |                                              | Quadro K620M                                            |                   |
|                    |                                              | Quadro M600M                                            |                   |
|                    |                                              | Quadro M500M                                            |                   |
|                    |                                              | NVIDIA NVS 810                                          |                   |
|                    |                                              | GeForce GTX 750 Ti                                      |                   |
|                    |                                              | GeForce GTX 750                                         |                   |
|                    |                                              | GeForce GTX 960M                                        |                   |
|                    |                                              | GeForce GTX 950M                                        |                   |
|                    |                                              | GeForce 940M                                            |                   |
|                    |                                              | GeForce 930M                                            |                   |
|                    |                                              | GeForce GTX 850M                                        |                   |
|                    |                                              | GeForce 840M                                            |                   |
|                    |                                              | GeForce 830M                                            |                   |
| 3.7                | Tesla K80                                    |                                                         |                   |
| 3.5                | Tesla K40                                    | Quadro K6000                                            |                   |
|                    | Tesla K20                                    | Quadro K5200                                            |                   |
|                    |                                              | Quadro K610M                                            |                   |
|                    |                                              | Quadro K510M                                            |                   |
|                    |                                              | GeForce GTX TITAN Z                                     |                   |
|                    |                                              | GeForce GTX TITAN Black                                 |                   |
|                    |                                              | GeForce GTX TITAN                                       |                   |
|                    |                                              | GeForce GTX 780 Ti                                      |                   |
|                    |                                              | GeForce GTX 780                                         |                   |
|                    |                                              | GeForce GT 730                                          |                   |
|                    |                                              | GeForce GT 720                                          |                   |
|                    |                                              | GeForce GT 705*                                         |                   |
|                    |                                              | GeForce GT 640 (GDDR5)                                  |                   |
|                    |                                              | GeForce 920M                                            |                   |
| 3.2                |                                              |                                                         | Tegra K1          |
|                    |                                              |                                                         | Jetson TK1        |
| 3 (Kepler)         | Tesla K10                                    | Quadro K5000                                            |                   |
|                    |                                              | Quadro K4200                                            |                   |
|                    |                                              | Quadro K4000                                            |                   |
|                    |                                              | Quadro K2000                                            |                   |
|                    |                                              | Quadro K2000D                                           |                   |
|                    |                                              | Quadro K600                                             |                   |
|                    |                                              | Quadro K420                                             |                   |
|                    |                                              | Quadro 410                                              |                   |
|                    |                                              | Quadro K6000M                                           |                   |
|                    |                                              | Quadro K5200M                                           |                   |
|                    |                                              | Quadro K5100M                                           |                   |
|                    |                                              | Quadro K500M                                            |                   |
|                    |                                              | Quadro K4200M                                           |                   |
|                    |                                              | Quadro K4100M                                           |                   |
|                    |                                              | Quadro K3100M                                           |                   |
|                    |                                              | Quadro K2200M                                           |                   |
|                    |                                              | Quadro K2100M                                           |                   |
|                    |                                              | Quadro K1100M                                           |                   |
|                    |                                              | NVIDIA NVS 510                                          |                   |
|                    |                                              | GeForce GTX 770                                         |                   |
|                    |                                              | GeForce GTX 760                                         |                   |
|                    |                                              | GeForce GTX 690                                         |                   |
|                    |                                              | GeForce GTX 680                                         |                   |
|                    |                                              | GeForce GTX 670                                         |                   |
|                    |                                              | GeForce GTX 660 Ti                                      |                   |
|                    |                                              | GeForce GTX 660                                         |                   |
|                    |                                              | GeForce GTX 650 Ti BOOST                                |                   |
|                    |                                              | GeForce GTX 650 Ti                                      |                   |
|                    |                                              | GeForce GTX 650                                         |                   |
|                    |                                              | GeForce GT 740                                          |                   |
|                    |                                              | GeForce GTX 880M                                        |                   |
|                    |                                              | GeForce GTX 870M                                        |                   |
|                    |                                              | GeForce GTX 780M                                        |                   |
|                    |                                              | GeForce GTX 770M                                        |                   |
|                    |                                              | GeForce GTX 765M                                        |                   |
|                    |                                              | GeForce GTX 760M                                        |                   |
|                    |                                              | GeForce GTX 680MX                                       |                   |
|                    |                                              | GeForce GTX 680M                                        |                   |
|                    |                                              | GeForce GTX 675MX                                       |                   |
|                    |                                              | GeForce GTX 670MX                                       |                   |
|                    |                                              | GeForce GTX 660M                                        |                   |
|                    |                                              | GeForce GT 755M                                         |                   |
|                    |                                              | GeForce GT 750M                                         |                   |
|                    |                                              | GeForce GT 650M                                         |                   |
|                    |                                              | GeForce GT 745M                                         |                   |
|                    |                                              | GeForce GT 645M                                         |                   |
|                    |                                              | GeForce GT 740M                                         |                   |
|                    |                                              | GeForce GT 730M                                         |                   |
|                    |                                              | GeForce GT 640M                                         |                   |
|                    |                                              | GeForce GT 640M LE                                      |                   |
|                    |                                              | GeForce GT 735M                                         |                   |
|                    |                                              | GeForce GT 730M                                         |                   |
| 2.1                |                                              | NVIDIA NVS 315                                          |                   |
|                    |                                              | NVIDIA NVS 310                                          |                   |
|                    |                                              | NVS 5400M                                               |                   |
|                    |                                              | NVS 5200M                                               |                   |
|                    |                                              | NVS 4200M                                               |                   |
|                    |                                              | Quadro 2000                                             |                   |
|                    |                                              | Quadro 2000D                                            |                   |
|                    |                                              | Quadro 600                                              |                   |
|                    |                                              | Quadro 4000M                                            |                   |
|                    |                                              | Quadro 3000M                                            |                   |
|                    |                                              | Quadro 2000M                                            |                   |
|                    |                                              | Quadro 1000M                                            |                   |
|                    |                                              | GeForce GTX 560 Ti                                      |                   |
|                    |                                              | GeForce GTX 550 Ti                                      |                   |
|                    |                                              | GeForce GTX 460                                         |                   |
|                    |                                              | GeForce GTS 450                                         |                   |
|                    |                                              | GeForce GTS 450*                                        |                   |
|                    |                                              | GeForce GT 730 DDR3,128bit                              |                   |
|                    |                                              | GeForce GT 640 (GDDR3)                                  |                   |
|                    |                                              | GeForce GT 630                                          |                   |
|                    |                                              | GeForce GT 620                                          |                   |
|                    |                                              | GeForce GT 610                                          |                   |
|                    |                                              | GeForce GT 520                                          |                   |
|                    |                                              | GeForce GT 440                                          |                   |
|                    |                                              | GeForce GT 440*                                         |                   |
|                    |                                              | GeForce GT 430                                          |                   |
|                    |                                              | GeForce GT 430*                                         |                   |
|                    |                                              | GeForce 820M                                            |                   |
|                    |                                              | GeForce 800M                                            |                   |
|                    |                                              | GeForce GTX 675M                                        |                   |
|                    |                                              | GeForce GTX 670M                                        |                   |
|                    |                                              | GeForce GT 635M                                         |                   |
|                    |                                              | GeForce GT 630M                                         |                   |
|                    |                                              | GeForce GT 625M                                         |                   |
|                    |                                              | GeForce GT 720M                                         |                   |
|                    |                                              | GeForce GT 620M                                         |                   |
|                    |                                              | GeForce 710M                                            |                   |
|                    |                                              | GeForce 705M                                            |                   |
|                    |                                              | GeForce 610M                                            |                   |
|                    |                                              | GeForce GTX 580M                                        |                   |
|                    |                                              | GeForce GTX 570M                                        |                   |
|                    |                                              | GeForce GTX 560M                                        |                   |
|                    |                                              | GeForce GT 555M                                         |                   |
|                    |                                              | GeForce GT 550M                                         |                   |
|                    |                                              | GeForce GT 540M                                         |                   |
|                    |                                              | GeForce GT 525M                                         |                   |
|                    |                                              | GeForce GT 520MX                                        |                   |
|                    |                                              | GeForce GT 520M                                         |                   |
|                    |                                              | GeForce GTX 485M                                        |                   |
|                    |                                              | GeForce GTX 470M                                        |                   |
|                    |                                              | GeForce GTX 460M                                        |                   |
|                    |                                              | GeForce GT 445M                                         |                   |
|                    |                                              | GeForce GT 435M                                         |                   |
|                    |                                              | GeForce GT 420M                                         |                   |
|                    |                                              | GeForce GT 415M                                         |                   |
|                    |                                              | GeForce 710M                                            |                   |
|                    |                                              | GeForce 410M                                            |                   |
| 2                  | Tesla C2075                                  | Quadro Plex 7000                                        |                   |
|                    | Tesla C2050                                  | Quadro 6000                                             |                   |
|                    | Tesla C2070                                  | Quadro 5000                                             |                   |
|                    | Tesla M2050                                  | Quadro 4000                                             |                   |
|                    | Tesla M2070                                  | Quadro 4000 for Mac                                     |                   |
|                    | Tesla M2075                                  | Quadro 5010M                                            |                   |
|                    | Tesla M2090                                  | Quadro 5000M                                            |                   |
|                    |                                              | GeForce GTX 590                                         |                   |
|                    |                                              | GeForce GTX 580                                         |                   |
|                    |                                              | GeForce GTX 570                                         |                   |
|                    |                                              | GeForce GTX 480                                         |                   |
|                    |                                              | GeForce GTX 470                                         |                   |
|                    |                                              | GeForce GTX 465                                         |                   |
|                    |                                              | GeForce GTX 480M                                        |                   |
| 1.3                | Tesla C1060                                  | Quadro FX 5800                                          |                   |
|                    | Tesla S1070                                  | Quadro FX 4800                                          |                   |
|                    | Tesla M1060                                  | Quadro FX 4800 for Mac                                  |                   |
|                    |                                              | Quadro FX 3800                                          |                   |
|                    |                                              | Quadro CX                                               |                   |
|                    |                                              | Quadro Plex 2200 D2                                     |                   |
|                    |                                              | GeForce GTX 295                                         |                   |
|                    |                                              | GeForce GTX 285                                         |                   |
|                    |                                              | GeForce GTX 285 for Mac                                 |                   |
|                    |                                              | GeForce GTX 280                                         |                   |
|                    |                                              | GeForce GTX 275                                         |                   |
|                    |                                              | GeForce GTX 260                                         |                   |
| 1.2                |                                              | Quadro 400                                              |                   |
|                    |                                              | Quadro FX 380 Low Profile                               |                   |
|                    |                                              | NVIDIA NVS 300                                          |                   |
|                    |                                              | Quadro FX 1800M                                         |                   |
|                    |                                              | Quadro FX 880M                                          |                   |
|                    |                                              | Quadro FX 380M                                          |                   |
|                    |                                              | NVIDIA NVS 300                                          |                   |
|                    |                                              | NVS 5100M                                               |                   |
|                    |                                              | NVS 3100M                                               |                   |
|                    |                                              | NVS 2100M                                               |                   |
|                    |                                              | GeForce GT 240                                          |                   |
|                    |                                              | GeForce GT 220*                                         |                   |
|                    |                                              | GeForce 210*                                            |                   |
|                    |                                              | GeForce GTS 360M                                        |                   |
|                    |                                              | GeForce GTS 350M                                        |                   |
|                    |                                              | GeForce GT 335M                                         |                   |
|                    |                                              | GeForce GT 330M                                         |                   |
|                    |                                              | GeForce GT 325M                                         |                   |
|                    |                                              | GeForce GT 240M                                         |                   |
|                    |                                              | GeForce G210M                                           |                   |
|                    |                                              | GeForce 310M                                            |                   |
|                    |                                              | GeForce 305M                                            |                   |
| 1.1                |                                              | Quadro FX 4700 X2                                       |                   |
|                    |                                              | Quadro FX 3700                                          |                   |
|                    |                                              | Quadro FX 1800                                          |                   |
|                    |                                              | Quadro FX 1700                                          |                   |
|                    |                                              | Quadro FX 580                                           |                   |
|                    |                                              | Quadro FX 570                                           |                   |
|                    |                                              | Quadro FX 470                                           |                   |
|                    |                                              | Quadro FX 380                                           |                   |
|                    |                                              | Quadro FX 370                                           |                   |
|                    |                                              | Quadro FX 370 Low Profile                               |                   |
|                    |                                              | Quadro NVS 450                                          |                   |
|                    |                                              | Quadro NVS 420                                          |                   |
|                    |                                              | Quadro NVS 295                                          |                   |
|                    |                                              | Quadro Plex 2100 D4                                     |                   |
|                    |                                              | Quadro FX 3800M                                         |                   |
|                    |                                              | Quadro FX 3700M                                         |                   |
|                    |                                              | Quadro FX 3600M                                         |                   |
|                    |                                              | Quadro FX 2800M                                         |                   |
|                    |                                              | Quadro FX 2700M                                         |                   |
|                    |                                              | Quadro FX 1700M                                         |                   |
|                    |                                              | Quadro FX 1600M                                         |                   |
|                    |                                              | Quadro FX 770M                                          |                   |
|                    |                                              | Quadro FX 570M                                          |                   |
|                    |                                              | Quadro FX 370M                                          |                   |
|                    |                                              | Quadro FX 360M                                          |                   |
|                    |                                              | Quadro NVS 320M                                         |                   |
|                    |                                              | Quadro NVS 160M                                         |                   |
|                    |                                              | Quadro NVS 150M                                         |                   |
|                    |                                              | Quadro NVS 140M                                         |                   |
|                    |                                              | Quadro NVS 135M                                         |                   |
|                    |                                              | Quadro NVS 130M                                         |                   |
|                    |                                              | Quadro NVS 450                                          |                   |
|                    |                                              | Quadro NVS 420                                          |                   |
|                    |                                              | Quadro NVS 295                                          |                   |
|                    |                                              | GeForce GTS 250                                         |                   |
|                    |                                              | GeForce GTS 150                                         |                   |
|                    |                                              | GeForce GT 130*                                         |                   |
|                    |                                              | GeForce GT 120*                                         |                   |
|                    |                                              | GeForce G100*                                           |                   |
|                    |                                              | GeForce 9800 GX2                                        |                   |
|                    |                                              | GeForce 9800 GTX+                                       |                   |
|                    |                                              | GeForce 9800 GTX                                        |                   |
|                    |                                              | GeForce 9600 GSO                                        |                   |
|                    |                                              | GeForce 9500 GT                                         |                   |
|                    |                                              | GeForce 8800 GTS                                        |                   |
|                    |                                              | GeForce 8800 GT                                         |                   |
|                    |                                              | GeForce 8800 GS                                         |                   |
|                    |                                              | GeForce 8600 GTS                                        |                   |
|                    |                                              | GeForce 8600 GT                                         |                   |
|                    |                                              | GeForce 8500 GT                                         |                   |
|                    |                                              | GeForce 8400 GS                                         |                   |
|                    |                                              | GeForce 9400 mGPU                                       |                   |
|                    |                                              | GeForce 9300 mGPU                                       |                   |
|                    |                                              | GeForce 8300 mGPU                                       |                   |
|                    |                                              | GeForce 8200 mGPU                                       |                   |
|                    |                                              | GeForce 8100 mGPU                                       |                   |
|                    |                                              | GeForce GTX 285M                                        |                   |
|                    |                                              | GeForce GTX 280M                                        |                   |
|                    |                                              | GeForce GTX 260M                                        |                   |
|                    |                                              | GeForce 9800M GTX                                       |                   |
|                    |                                              | GeForce 8800M GTX                                       |                   |
|                    |                                              | GeForce GTS 260M                                        |                   |
|                    |                                              | GeForce GTS 250M                                        |                   |
|                    |                                              | GeForce 9800M GT                                        |                   |
|                    |                                              | GeForce 9600M GT                                        |                   |
|                    |                                              | GeForce 8800M GTS                                       |                   |
|                    |                                              | GeForce 9800M GTS                                       |                   |
|                    |                                              | GeForce GT 230M                                         |                   |
|                    |                                              | GeForce 9700M GT                                        |                   |
|                    |                                              | GeForce 9650M GS                                        |                   |
|                    |                                              | GeForce 9700M GT                                        |                   |
|                    |                                              | GeForce 9650M GS                                        |                   |
|                    |                                              | GeForce 9600M GT                                        |                   |
|                    |                                              | GeForce 9600M GS                                        |                   |
|                    |                                              | GeForce 9500M GS                                        |                   |
|                    |                                              | GeForce 8700M GT                                        |                   |
|                    |                                              | GeForce 8600M GT                                        |                   |
|                    |                                              | GeForce 8600M GS                                        |                   |
|                    |                                              | GeForce 9500M G                                         |                   |
|                    |                                              | GeForce 9300M G                                         |                   |
|                    |                                              | GeForce 8400M GS                                        |                   |
|                    |                                              | GeForce G210M                                           |                   |
|                    |                                              | GeForce G110M                                           |                   |
|                    |                                              | GeForce 9300M GS                                        |                   |
|                    |                                              | GeForce 9200M GS                                        |                   |
|                    |                                              | GeForce 9100M G                                         |                   |
|                    |                                              | GeForce 8400M GT                                        |                   |
|                    |                                              | GeForce G105M                                           |                   |
| 1                  | Tesla C870                                   | Quadro FX 5600                                          |                   |
|                    | Tesla D870                                   | Quadro FX 4600                                          |                   |
|                    | Tesla S870                                   | Quadro Plex 2100 S4                                     |                   |
|                    |                                              | GeForce GT 420*                                         |                   |
|                    |                                              | GeForce 8800 Ultra                                      |                   |
|                    |                                              | GeForce 8800 GTX                                        |                   |
|                    |                                              | GeForce GT 340*                                         |                   |
|                    |                                              | GeForce GT 330*                                         |                   |
|                    |                                              | GeForce GT 320*                                         |                   |
|                    |                                              | GeForce 315*                                            |                   |
|                    |                                              | GeForce 310*                                            |                   |
|                    |                                              | GeForce 9800 GT                                         |                   |
|                    |                                              | GeForce 9600 GT                                         |                   |
|                    |                                              | GeForce 9400GT                                          |                   |

---

* Refs:
    * <https://developer.nvidia.com/cuda-gpus>
    * <https://developer.nvidia.com/cuda-legacy-gpus>
* Related links:
    * <https://en.wikipedia.org/wiki/CUDA#GPUs_supported>

## 设备驱动（Device Driver)（内核模块）
NVIDIA 的设备驱动不是一个设备一个驱动，而是独立的，一个大版本一个分支。一个大版本的设备驱动会支持一系列的设备，大版本更新迭代就会逐渐放弃对旧版设备的支持。就像 515 分支 README 中会提到：

> GeForce and Workstation support is still considered alpha-quality.

对于一个特定硬件，我们需要找到哪个驱动大版本支持该硬件，然后下载该大版本驱动中的最新小版本。

> Open GPU kernel modules are supported only on Turing and newer generations.

[Ref](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/index.html#kernel-modules)

> The NVIDIA open kernel modules can be used on any Turing or later GPU (see the table below).

[Ref](https://github.com/NVIDIA/open-gpu-kernel-modules)

GeForce 系列中，20 系列是 Turing 架构的，所以，从 20 系列起的显卡，可以使用 Open GPU kernel modules


### 设备驱动版本的选择

[该目录](https://download.nvidia.com/XFree86/Linux-x86_64/)下，有所有的驱动版本。在各个版本下的 `README/supportedchips.html` 页面中，有记录特定版本的驱动支持哪些型号的 GPU，例如，[这](https://download.nvidia.com/XFree86/Linux-x86_64/580.76.05/README/supportedchips.html)是 580.76.05 版本的驱动支持的 GPU 列表。（不要使用 run 来安装，会搞乱系统的，安装时还是使用包管理器来安装）

当然，我们也可以去下载的 `Supported Products` 里面查看支持的 GPU 型号，相关链接如下

* Linux:
    * <https://www.nvidia.com/en-us/drivers/unix/>
    * <https://www.nvidia.com/en-us/drivers/details/252613/>
    * <https://www.nvidia.com/en-us/drivers/details/226760/>
* Windows:
    * <https://www.nvidia.cn/drivers/details/253167/>

这些界面的入口都在 [此](https://www.nvidia.com/en-us/drivers/)

我们可以看到 RTX 20/30/40/50 系列都能使用 580 版本的驱动。


### Ubuntu 上的驱动版本依赖

```
nvidia-open
  nvidia-driver-570-open
    nvidia-kernel-source-570-open(dkms ko source)
```

### 旧版 GPU 驱动的选择
上文说到 Turing 架构起都能使用 nvidia-open，那么旧版的呢？[Arch Wiki](https://wiki.archlinux.org/title/NVIDIA) 这里有个表

| GPU family                                               | Driver                                                                                   | Status                                                                               |
|----------------------------------------------------------|------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------|
| Turing (NV160/TUXXX) and newer                           | nvidia-open for linux, nvidia-open-lts for linux-lts, nvidia-open-dkms for any kernel(s) | Recommended by upstream Current, supported Possible power management issue on Turing |
| Maxwell (NV110/GMXXX) through Ada Lovelace (NV190/ADXXX) | nvidia for linux, nvidia-lts for linux-lts, nvidia-dkms for any kernel(s)                | Current, supported                                                                   |
| Kepler (NVE0/GKXXX)                                      | nvidia-470xx-dkms                                                                        | Legacy, unsupported                                                                  |
| Fermi (NVC0/GF1XX)                                       | nvidia-390xx-dkms                                                                        |                                                                                      |
| Tesla (NV50/G80-90-GT2XX)                                | nvidia-340xx-dkms                                                                        |                                                                                      |
| Curie (NV40/G70) and older                               | No longer packaged                                                                       |                                                                                      |

这里的 GPU Family 就是指的 NVIDIA GPU architecture.

如何知晓自己的 GPU 的 GPU Family 呢？Arch Wiki 也说明了，首先找到自己的 GPU 型号

```console
lspci -k -d ::03xx

0d:00.0 VGA compatible controller: NVIDIA Corporation GK208B [GeForce GT 730] (rev a1)
                                                      ------ ----------------
                                                     code name  GPU Model
```

有两种方式：

* 根据 GPU 型号查上面的表，可以看到他的 CC 是 3.5，是基于 Kepler 架构的，所以我们需要装的是 `nvidia-470xx-dkms`
* 根据 code name，去 [nouveau codenames](https://nouveau.freedesktop.org/CodeNames.html) 找属于那个 GPU Family，可以看到是属于 `NVE0 family (Kepler)`，所以我们需要装的是 `nvidia-470xx-dkms`

#### 关于 Code Name
`drivers/gpu/drm/nouveau/nvkm/engine/device/base.c` 中是有关于 code name 的信息的，但是 `lspci` 不是从这里取的，我们通过 `strace` 查看到会去读 `/sys/bus/pci/devices/0000:0d:00.0/device`，然后将具体的设备对应到什么名字是在这里查的 `/usr/share/hwdata/pci.ids`。修改这个文件中的名字，`lspci` 确实会变成我们修改后的名字。

而如果这个文件中没有对应的名字，`lspci` 就直接显示 device id 到 `lspci` 的结果中了，就像 Ubuntu 中没有 5090 D 的信息，就会直接显示 `2b87`，如果新增 `2b87  GB202 [GeForce RTX 5090 D]` 到该文件中，就会更新啦。

PCI ID 可以去 [NVIDIA/open-gpu-kernel-modules](https://github.com/NVIDIA/open-gpu-kernel-modules) 找。

## CUDA 驱动（`libcuda.so`）
* CUDA：Compute Unified Devices Architecture
* CUDA core 就是 GPU 的渲染核心，只是 nvidia 提供了访问这些核心的 API 即 cuda。 [refs](https://superuser.com/a/856434/1227634)

> CUDA is a driver that lets other apps use the GPU for processing things other than graphics. Like for doing math processing and encoding. It doesn't affect graphics performance.

[Ref](https://www.reddit.com/r/macgaming/comments/3nguex/follow_up_questionsorry_what_is_the_difference/)

`libcuda.so` 是对 Device Driver 的一层封装，以提供 CUDA API。一般只用于管理（是吗？？？）

> While the CUDA driver API provides (CUDA Driver API) a low-level programming interface for applications to target NVIDIA hardware.

* [CUDA Driver API](https://docs.nvidia.com/cuda/cuda-driver-api/index.html)
* [Ref](https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#cuda-toolkit-versioning)


```c
// By ChatGPT
// gcc -I /usr/local/cuda-12.8/targets/x86_64-linux/include  a.c -lcuda
// nvcc cu_driver_version.c -o cu_driver_version -lcuda
#include <stdio.h>
#include <cuda.h>

int main() {
    CUresult res;
    int driverVersion = 0;

    // Initialize the CUDA driver API
    res = cuInit(0);
    if (res != CUDA_SUCCESS) {
        printf("Failed to initialize CUDA driver API (error code %d)\n", res);
        return -1;
    }

    // Get CUDA driver version
    res = cuDriverGetVersion(&driverVersion);
    if (res == CUDA_SUCCESS) {
        int major = driverVersion / 1000;
        int minor = (driverVersion % 1000) / 10;
        printf("CUDA Driver Version: %d (Major %d, Minor %d)\n", driverVersion, major, minor);
    } else {
        printf("Failed to get CUDA driver version (error code %d)\n", res);
        return -1;
    }

    return 0;
}
```

### CUDA 驱动版本的选择

*CUDA 驱动* 版本是需要和 *设备驱动* 版本统一的。

只有以下三种系统支持“旧版设备驱动” + “新版cuda驱动”

* NVIDIA Data Center GPUs.
* Select NGC Server Ready SKUs of RTX cards.
* Jetson boards.

![[forward-compatibility-upgrade-path(by NVIDIA)](https://docs.nvidia.com/deploy/cuda-compatibility/_images/forward-compatibility-upgrade-path.png)](./figures/forward-compatibility-upgrade-path.png)

---

* Refs:
    * <https://docs.nvidia.com/deploy/cuda-compatibility/forward-compatibility.html>
* Related links:
    * <https://hackmd.io/@JiangJianGuo/HJYOaAWfA>

### Ubuntu 上的驱动版本依赖

```
nvidia-open
  nvidia-driver-570-open
    libnvidia-compute-570 (libcuda.so)
```

所以这里我们可以看到 `libcuda.so` 和 `nvidia.ko` 都是 `nvidia-driver-570-open` 的依赖，而且看到版本号要求是 `=`，所以 *CUDA 驱动* 版本是需要和 *设备驱动* 版本统一的。

#### 其他依赖

```
nvidia-open
  nvidia-driver-570-open
    libnvidia-gl-570 (= 570.172.08-0ubuntu1)
    nvidia-dkms-570-open (= 570.172.08-0ubuntu1)            <- /usr/src 中提供 patch
    nvidia-kernel-common-570 (= 570.172.08-0ubuntu1)
      nvidia-firmware-570 (= 570.172.08-0ubuntu1)
      nvidia-modprobe (>= 570.172.08-0ubuntu1)
    nvidia-kernel-source-570-open (= 570.172.08-0ubuntu1)   <- nvidia.ko and other ko
    libnvidia-compute-570 (= 570.172.08-0ubuntu1)           <- libcuda.so
    libnvidia-extra-570 (= 570.172.08-0ubuntu1)
    nvidia-compute-utils-570 (= 570.172.08-0ubuntu1)
    libnvidia-decode-570 (= 570.172.08-0ubuntu1)
    libnvidia-encode-570 (= 570.172.08-0ubuntu1)
    nvidia-utils-570 (= 570.172.08-0ubuntu1)
    xserver-xorg-video-nvidia-570 (= 570.172.08-0ubuntu1)
    libnvidia-fbc1-570 (= 570.172.08-0ubuntu1)
```

## CUDA Toolkit
CUDA toolkit 提供了 `nvcc` 等各种上层应用。

CUDA toolkit 的版本依赖 CUDA 驱动的版本

* API: not compatible(old toolkit may not compile with new cuda driver )
* ABI: compatible(old toolkit can run on new cuda driver)( since cuda 10 )

> Starting with CUDA 11, the toolkit versions are based on an industry-standard semantic versioning scheme: .X.Y.Z, where:
>     .X stands for the major version - APIs have changed and binary compatibility is broken.
>     .Y stands for the minor version - Introduction of new APIs, deprecation of old APIs, and source compatibility might be broken but binary compatibility is maintained.
>     .Z stands for the release/patch version - new updates and patches will increment this.

---

* Refs
    * <https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#cuda-compatibility-and-upgrades>


### CUDA Toolkit 版本的选择

Table 2 CUDA Toolkit and Minimum Required Driver Version for CUDA Minor Version Compatibility

| CTK Version | CUDA Driver Range for Minor Version Compatibility |
|-------------|---------------------------------------------------|
| 13.x        | `580<= CUDA driver version`                       |
| 12.x        | `525<= CUDA driver version <580`                  |
| 11.x        | `450<= CUDA driver version <525`                  |

---

* Refs
    * <https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html#id8>

Table 3 CUDA Toolkit and Corresponding Driver Versions

| CUDA Toolkit                                      | Linux x86_64 Driver Version | Windows x86_64 Driver Version |
|---------------------------------------------------|-----------------------------|-------------------------------|
| CUDA 13.0 GA                                      | >=580.65.06                 | N/A                           |
| CUDA 12.9 Update 1                                | >=575.57.08                 | >=576.57                      |
| CUDA 12.9 GA                                      | >=575.51.03                 | >=576.02                      |
| CUDA 12.8 Update 1                                | >=570.124.06                | >=572.61                      |
| CUDA 12.8 GA                                      | >=570.26                    | >=570.65                      |
| CUDA 12.6 Update 3                                | >=560.35.05                 | >=561.17                      |
| CUDA 12.6 Update 2                                | >=560.35.03                 | >=560.94                      |
| CUDA 12.6 Update 1                                | >=560.35.03                 | >=560.94                      |
| CUDA 12.6 GA                                      | >=560.28.03                 | >=560.76                      |
| CUDA 12.5 Update 1                                | >=555.42.06                 | >=555.85                      |
| CUDA 12.5 GA                                      | >=555.42.02                 | >=555.85                      |
| CUDA 12.4 Update 1                                | >=550.54.15                 | >=551.78                      |
| CUDA 12.4 GA                                      | >=550.54.14                 | >=551.61                      |
| CUDA 12.3 Update 1                                | >=545.23.08                 | >=546.12                      |
| CUDA 12.3 GA                                      | >=545.23.06                 | >=545.84                      |
| CUDA 12.2 Update 2                                | >=535.104.05                | >=537.13                      |
| CUDA 12.2 Update 1                                | >=535.86.09                 | >=536.67                      |
| CUDA 12.2 GA                                      | >=535.54.03                 | >=536.25                      |
| CUDA 12.1 Update 1                                | >=530.30.02                 | >=531.14                      |
| CUDA 12.1 GA                                      | >=530.30.02                 | >=531.14                      |
| CUDA 12.0 Update 1                                | >=525.85.12                 | >=528.33                      |
| CUDA 12.0 GA                                      | >=525.60.13                 | >=527.41                      |
| CUDA 11.8 GA                                      | >=520.61.05                 | >=520.06                      |
| CUDA 11.7 Update 1                                | >=515.48.07                 | >=516.31                      |
| CUDA 11.7 GA                                      | >=515.43.04                 | >=516.01                      |
| CUDA 11.6 Update 2                                | >=510.47.03                 | >=511.65                      |
| CUDA 11.6 Update 1                                | >=510.47.03                 | >=511.65                      |
| CUDA 11.6 GA                                      | >=510.39.01                 | >=511.23                      |
| CUDA 11.5 Update 2                                | >=495.29.05                 | >=496.13                      |
| CUDA 11.5 Update 1                                | >=495.29.05                 | >=496.13                      |
| CUDA 11.5 GA                                      | >=495.29.05                 | >=496.04                      |
| CUDA 11.4 Update 4                                | >=470.82.01                 | >=472.50                      |
| CUDA 11.4 Update 3                                | >=470.82.01                 | >=472.50                      |
| CUDA 11.4 Update 2                                | >=470.57.02                 | >=471.41                      |
| CUDA 11.4 Update 1                                | >=470.57.02                 | >=471.41                      |
| CUDA 11.4.0 GA                                    | >=470.42.01                 | >=471.11                      |
| CUDA 11.3.1 Update 1                              | >=465.19.01                 | >=465.89                      |
| CUDA 11.3.0 GA                                    | >=465.19.01                 | >=465.89                      |
| CUDA 11.2.2 Update 2                              | >=460.32.03                 | >=461.33                      |
| CUDA 11.2.1 Update 1                              | >=460.32.03                 | >=461.09                      |
| CUDA 11.2.0 GA                                    | >=460.27.03                 | >=460.82                      |
| CUDA 11.1.1 Update 1                              | >=455.32                    | >=456.81                      |
| CUDA 11.1 GA                                      | >=455.23                    | >=456.38                      |
| CUDA 11.0.3 Update 1                              | >= 450.51.06                | >= 451.82                     |
| CUDA 11.0.2 GA                                    | >= 450.51.05                | >= 451.48                     |
| CUDA 11.0.1 RC                                    | >= 450.36.06                | >= 451.22                     |
| CUDA 10.2.89                                      | >= 440.33                   | >= 441.22                     |
| CUDA 10.1 (10.1.105 general release, and updates) | >= 418.39                   | >= 418.96                     |
| CUDA 10.0.130                                     | >= 410.48                   | >= 411.31                     |
| CUDA 9.2 (9.2.148 Update 1)                       | >= 396.37                   | >= 398.26                     |
| CUDA 9.2 (9.2.88)                                 | >= 396.26                   | >= 397.44                     |
| CUDA 9.1 (9.1.85)                                 | >= 390.46                   | >= 391.29                     |
| CUDA 9.0 (9.0.76)                                 | >= 384.81                   | >= 385.54                     |
| CUDA 8.0 (8.0.61 GA2)                             | >= 375.26                   | >= 376.51                     |
| CUDA 8.0 (8.0.44)                                 | >= 367.48                   | >= 369.30                     |
| CUDA 7.5 (7.5.16)                                 | >= 352.31                   | >= 353.66                     |
| CUDA 7.0 (7.0.28)                                 | >= 346.46                   | >= 347.62                     |


* Refs:
    * <https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html#id9>

## PyTorch
> PyTorch is an open-source deep learning framework that’s known for its flexibility and ease-of-use. This is enabled in part by its compatibility with the popular Python high-level programming language favored by machine learning developers and data scientists.
>
> PyTorch is a fully featured framework for building deep learning models, which is a type of machine learning that’s commonly used in applications like image recognition and language processing. 

[Ref](https://www.nvidia.com/en-us/glossary/pytorch/)

### PyTorch 版本的选择
从安装方式来看，PyTorch 对 CUDA Toolkit 的小版本都是有要求的，虽然同一版本的 PyTorch 可以跑在不同的 CUDA Toolkit 上，但是安装的二进制都是不同的，所以这里是强依赖。就像我们目前安装 2.7.0 的 PyTorch，它的版本号是 `2.7.0+cu128`。显然是限定了 Toolkit 的版本。

NVIDIA CUDA Toolkit version

| PyTorch    | 8.0 | 9.0 | 9.2 | 10.0 | 10.1 | 10.2 | 11.0 | 11.1 | 11.2 | 11.3 | 11.6 | 11.7 | 11.8 | 12.1 | 12.4 | 12.6 | 12.8 | 12.9 |
|------------|-----|-----|-----|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|
| v2.8       |     |     |     |      |      |      |      |      |      |      |      |      |      |      |      | O    | O    | O    |
| v2.7.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    |      |      | O    | O    |      |
| v2.7.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    |      |      | O    | O    |      |
| v2.6.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    |      | O    | O    |      |      |
| v2.5.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    | O    |      |      |      |
| v2.5.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    | O    |      |      |      |
| v2.4.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    | O    |      |      |      |
| v2.4.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    | O    |      |      |      |
| v2.3.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.3.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.2.2     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.2.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.2.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.1.2     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.1.1     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.1.0     |     |     |     |      |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |
| v2.0.1     |     |     |     |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |      |
| v2.0.0     |     |     |     |      |      |      |      |      |      |      |      | O    | O    |      |      |      |      |      |
| v1.13.1    |     |     |     |      |      |      |      |      |      |      | O    | O    |      |      |      |      |      |      |
| v1.13.0    |     |     |     |      |      |      |      |      |      |      | O    | O    |      |      |      |      |      |      |
| v1.12.1    |     |     |     |      |      |      |      |      | O    | O    | O    |      |      |      |      |      |      |      |
| v1.12.0    |     |     |     |      |      |      |      |      | O    | O    | O    |      |      |      |      |      |      |      |
| v1.11.0    |     |     |     |      |      |      |      |      | O    | O    |      |      |      |      |      |      |      |      |
| v1.10.1    |     |     |     |      |      |      |      | O    | O    |      |      |      |      |      |      |      |      |      |
| v1.10.0    |     |     |     |      |      |      |      | O    | O    |      |      |      |      |      |      |      |      |      |
| v1.9.1     |     |     |     |      |      | O    |      | O    |      |      |      |      |      |      |      |      |      |      |
| v1.9.0     |     |     |     |      |      | O    |      | O    |      |      |      |      |      |      |      |      |      |      |
| v1.8.2 LTS |     |     |     |      |      | O    |      | O    |      |      |      |      |      |      |      |      |      |      |
| v1.8.1     |     |     |     |      | O    | O    |      | O    |      |      |      |      |      |      |      |      |      |      |
| v1.8.0     |     |     |     |      |      | O    |      | O    |      |      |      |      |      |      |      |      |      |      |
| v1.7.1     |     |     | O   |      | O    | O    | O    |      |      |      |      |      |      |      |      |      |      |      |
| v1.7.0     |     |     | O   |      | O    | O    | O    |      |      |      |      |      |      |      |      |      |      |      |
| v1.6.0     |     |     | O   |      | O    | O    |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.5.1     |     |     | O   |      | O    | O    |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.5.0     |     |     | O   |      | O    | O    |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.4.0     |     |     | O   |      | O    |      |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.2.0     |     |     | O   | O    |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.1.0     |     | O   |     | O    |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.0.1     |     | O   |     | O    |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| v1.0.0     | O   | O   |     | O    |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
...

* Refs:
    * <https://pytorch.org/get-started/locally/>
    * <https://pytorch.org/get-started/previous-versions/>


AMD ROCM

| PyTorch    | 3.10 | 4.0.1 | 4.1 | 4.2 | 4.5.2 | 5.1.1 | 5.2 | 5.4.2 | 5.6 | 5.7 | 6.0 | 6.1 | 6.2 | 6.3 | 6.4 |
|------------|------|-------|-----|-----|-------|-------|-----|-------|-----|-----|-----|-----|-----|-----|-----|
| v2.8       |      |       |     |     |       |       |     |       |     |     |     |     |     |     | O   |
| v2.7.1     |      |       |     |     |       |       |     |       |     |     |     |     |     | O   |     |
| v2.7.0     |      |       |     |     |       |       |     |       |     |     |     |     |     | O   |     |
| v2.6.0     |      |       |     |     |       |       |     |       |     |     |     | O   |     |     |     |
| v2.5.1     |      |       |     |     |       |       |     |       |     |     |     | O   | O   |     |     |
| v2.5.0     |      |       |     |     |       |       |     |       |     |     |     | O   | O   |     |     |
| v2.4.1     |      |       |     |     |       |       |     |       |     |     |     | O   |     |     |     |
| v2.4.0     |      |       |     |     |       |       |     |       |     |     |     | O   |     |     |     |
| v2.3.1     |      |       |     |     |       |       |     |       |     |     | O   |     |     |     |     |
| v2.3.0     |      |       |     |     |       |       |     |       |     |     | O   |     |     |     |     |
| v2.2.2     |      |       |     |     |       |       |     |       |     | O   |     |     |     |     |     |
| v2.2.1     |      |       |     |     |       |       |     |       |     | O   |     |     |     |     |     |
| v2.2.0     |      |       |     |     |       |       |     |       | O   |     |     |     |     |     |     |
| v2.1.2     |      |       |     |     |       |       |     |       | O   |     |     |     |     |     |     |
| v2.1.1     |      |       |     |     |       |       |     |       | O   |     |     |     |     |     |     |
| v2.1.0     |      |       |     |     |       |       |     |       | O   |     |     |     |     |     |     |
| v2.0.1     |      |       |     |     |       |       |     | O     |     |     |     |     |     |     |     |
| v2.0.0     |      |       |     |     |       |       |     | O     |     |     |     |     |     |     |     |
| v1.13.1    |      |       |     |     |       |       | O   |       |     |     |     |     |     |     |     |
| v1.13.0    |      |       |     |     |       |       | O   |       |     |     |     |     |     |     |     |
| v1.12.1    |      |       |     |     |       | O     |     |       |     |     |     |     |     |     |     |
| v1.11.0    |      |       |     |     | O     |       |     |       |     |     |     |     |     |     |     |
| v1.10.1    |      | O     | O   | O   |       |       |     |       |     |     |     |     |     |     |     |
| v1.10.0    |      | O     | O   | O   |       |       |     |       |     |     |     |     |     |     |     |
| v1.9.1     |      | O     | O   | O   |       |       |     |       |     |     |     |     |     |     |     |
| v1.9.0     |      | O     | O   | O   |       |       |     |       |     |     |     |     |     |     |     |
| v1.8.2 LTS |      |       |     |     |       |       |     |       |     |     |     |     |     |     |     |
| v1.8.1     | O    | O     |     |     |       |       |     |       |     |     |     |     |     |     |     |
| v1.8.0     |      | O     |     |     |       |       |     |       |     |     |     |     |     |     |     |
...


## 怎么选各个组件的版本
GPU 型号确定了可以安装 device driver 的版本范围，从而确定了 CUDA driver 的版本范围，从而确定 CUDA Toolkit 的版本范围，从而确定 PyTorch 的版本范围。

所以主要考虑 GPU 型号，但是由于 PyTorch 并不支持最新的 CUDA Toolkit，所以，我们还需要考虑 PyTorch 的对 CUDA Toolkit 的版本范围要求。对于目前最新的 GeForce RTX 5090，device driver、CUDA driver 虽然可以装最新的 `580`，但是由于对于该版驱动 CUDA Toolkit 只能安装 `13.x`，而 PyTorch 当下还不支持 CUDA Toolkit 13.x，所以我们只能安装 `576` 的 driver，`12.9` 的 Toolkit 和 `2.8` 的 PyTorch。等到 PyTorch 更新支持 `13.x` 的 Toolkit 时，就没有这些限制啦。
