---
title: "7zr CPU 性能测试结果解析"
date: 2024-10-30T10:07:54+08:00
lastMod: 2024-10-30T11:26:47+08:00
code: true
mermaid: false
katex: false
draft: false
author: "Ben"
geometry: margin=2cm
CJKmainfont: "Noto Sans CJK SC"
description: "对 7zr 输出结果 Speed，Usage，R/U 和 Rating 的结果解析"
tags: ["7zr", "benchmark", "Linux Command"]
---


## TL;DR

* `Speed(KiB/s) = 解压大小(B) * 重复次数(#)/time(s) /1024(B/KiB)`
* `usage(%)     ≈ 程序占用 CPU 时间/程序开始到现在为止总耗时*100%`
* `R/U(MIPS)    ≈ 产生的指令数(MI)/占用 CPU 的时间`
* `Rating(MIPS) ≈ 产生的指令数(MI)/程序耗时(s)`

`7zr b -mmt1`

```
                       Compressing  |                  Decompressing
Dict     Speed Usage    R/U Rating  |      Speed Usage    R/U Rating
         KiB/s     %   MIPS   MIPS  |      KiB/s     %   MIPS   MIPS

22:       4515   100   4403   4393  |      51373   100   4395   4386
23:       3883   100   3966   3956  |      50676   100   4395   4387
24:       3724   100   4014   4004  |      48750   100   4292   4280
25:       3520   100   4032   4020  |      48064   100   4293   4278
----------------------------------  | ------------------------------
Avr:             100   4104   4093  |              100   4344   4333
Tot:             100   4224   4213
```

## Speed

这段打印在

```cpp
//p7zip-17.05/CPP/7zip/UI/Common/Bench.cpp
static void PrintResults(IBenchPrintCallback *f,
    const CBenchInfo &info,
    unsigned weight,
    UInt64 rating,
    bool showFreq, UInt64 cpuFreq,
    CTotalBenchRes *res)
{
  UInt64 speed = info.GetSpeed(info.UnpackSize * info.NumIterations);
  if (f)
  {
    if (speed != 0)
      PrintNumber(*f, speed / 1024, kFieldSize_Speed);
    else
      PrintSpaces(*f, 1 + kFieldSize_Speed);
  }
  UInt64 usage = info.GetUsage();
  UInt64 rpu = info.GetRatingPerUsage(rating);
  if (f)
  {
    PrintResults(*f, usage, rpu, rating, showFreq, cpuFreq);
  }

  if (res)
  {
    // res->NumIterations1++;
    res->NumIterations2 += weight;
    res->RPU += (rpu * weight);
    res->Rating += (rating * weight);
    res->Usage += (usage * weight);
  }
}
```

其中 `info.GetSpeed()` 如下：

```cpp
//p7zip-17.05/CPP/7zip/UI/Common/Bench.cpp
static UInt64 MyMultDiv64(UInt64 value, UInt64 elapsedTime, UInt64 freq)
{
  UInt64 elTime = elapsedTime;
  NormalizeVals(freq, elTime);
  if (elTime == 0)
    elTime = 1;
  return value * freq / elTime;
  /*
   * 原始公式： value /(elTime/freq)
   * 各个变量对应的单位，其中：
   *    * value:    number
   *    * elTime:   ms
   *    * freq:     ms/s ( or to say: 一秒内有多少个 ms event )
  */
}

UInt64 CBenchInfo::GetSpeed(UInt64 numCommands) const
{
  return MyMultDiv64(numCommands, GlobalTime, GlobalFreq);
}
```


我们可以知道变量 speed 的定义如下：

```
speed(B/s) = 解压大小(B) * info.NumIterations(#)/time(s)
```

所以 Speed 列就是 `Speed(KiB/s)=speed(B/s)/1024(B/KiB)` 。即 `Speed(KiB/s)=解压大小(B) * 重复次数(#)/time(s) /1024(B/KiB)`

## Usage, R/U, Rating
继续看剩下三列的定义：Usage, R/U, Rating

```cpp
//p7zip-17.05/CPP/7zip/UI/Common/Bench.cpp
static void PrintResults(IBenchPrintCallback &f, UInt64 usage, UInt64 rpu, UInt64 rating, bool showFreq, UInt64 cpuFreq)
{
  PrintNumber(f, (usage + 5000) / 10000, kFieldSize_Usage);
  PrintRating(f, rpu, kFieldSize_RU);
  PrintRating(f, rating, kFieldSize_Rating);
  if (showFreq)
  {
    if (cpuFreq == 0)
      PrintSpaces(f, kFieldSize_EUAndEffec);
    else
    {
      UInt64 ddd = cpuFreq * usage / 100;
      if (ddd == 0)
        ddd = 1;
      PrintPercents(f, (rating * 10000), ddd, kFieldSize_EU);
      PrintPercents(f, rating, cpuFreq, kFieldSize_Effec);
    }
  }
}
```

### Usage

`usage` 是从上一级 `PrintResults` 中通过 `info.GetUsage()` 取到的。简单跟踪之后可知：

```
usage = 程序占用 CPU 时间/程序开始到现在为止总耗时 * 10^6
```

其中 `10^6` 会在后续被除以 `10000` 规整为百分比。

本级的 `PrintResults` 中打印了 Usage 列，打印的时间是：`(usage + 5000) / 10000`，除以 `10000` 就是上面说的“规整为百分比”，而加的 50% 大概是其他消耗占程序的 50%。总之，`Usage` 列大概指的是压缩测试占用 CPU 的百分比。即：`usage(%) ≈ 程序占用 CPU 时间/程序开始到现在为止总耗时*100%`

### Rating
`rating` 是从以下代码得来的：

```cpp
UInt64 CBenchProps::GetCompressRating(UInt32 dictSize, UInt64 elapsedTime, UInt64 freq, UInt64 size)
{
  if (dictSize < (1 << kBenchMinDicLogSize))
    dictSize = (1 << kBenchMinDicLogSize);
  UInt64 encComplex = EncComplex;
  if (LzmaRatingMode)
  {
    UInt64 t = GetLogSize(dictSize) - (kBenchMinDicLogSize << kSubBits);
    encComplex = 870 + ((t * t * 5) >> (2 * kSubBits));
  }
  UInt64 numCommands = (UInt64)size * encComplex;

  /*
   * 和计算 Usage 时差不多，大概意思是：
   *  numCommands/elapsedTime
   * 通过各种 size 估算出计算的 command 的数量是多少，单位是 Million Instruction （因为后面没见到除以 1,000,000）
   */
  return MyMultDiv64(numCommands, elapsedTime, freq);
}

HRESULT CBenchCallbackToPrint::SetEncodeResult(const CBenchInfo &info, bool final)
{
  RINOK(_file->CheckBreak());
  if (final)
  {
    UInt64 rating = BenchProps.GetCompressRating(DictSize, info.GlobalTime, info.GlobalFreq, info.UnpackSize * info.NumIterations);
    PrintResults(_file, info,
        EncodeWeight, rating,
        ShowFreq, CpuFreq, &EncodeRes);
    if (!Use2Columns)
      _file->NewLine();
  }
  return S_OK;
}
```

所以 Rating 就是在“程序耗时”（`Eltime`）期间，产生的指令数，单位：（MI/s) 即 `MIPS`。即 `R/U(MIPS) ≈ 产生的指令数(MI)/占用 CPU 的时间`

### R/U
（这也是我找这段代码的关键字）

可以看到 `R/U` 列来自变量 `rpu`，`rpu` 来自变量 `rating`，

简单跟踪之后发现，它和 `Rating` 的区别在于，将耗时改成了“占用 CPU 的时间”，所以也可以猜出 `rpu` 的缩写是 rating per usage，它的单位一样是（MI/s) 即 `MIPS`。即 `Rating(MIPS) ≈ 产生的指令数(MI)/程序耗时(s)`


