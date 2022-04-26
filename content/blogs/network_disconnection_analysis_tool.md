---
title: "网络断连分析工具介绍"
date: 2022-04-26T02:32:01+08:00
lastMod: 2022-04-26T02:32:01+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "mainly about ethtool and wireshark"
tag: ["network", "tool", "ethtool"]
---

# 网络断连分析工具介绍

[toc]

## 抓包工具（数据链路层及之上）

### `tcpdump`
1. 网络层：

```console
# tcpdump -i eth0
```

2. 数据链路层

```console
# tcpdump -e -i eth0
```

### `wireshark`

`wireshark` 还是更香一点，可以直接抓，抓完还可以看流量图 (statistic-Flow graph)(一应一答的那种)

需要注意的是通讯双方的 `wireshark` 版本要保持一致，老版本的 sequence number 和新版本的对不上

我们这次使用的是 Version 3.6.3，如果没有可以自行 [下载](https://1.as.dl.wireshark.org/src/wireshark-3.6.3.tar.xz) 编译

## PHY 芯片的查看（物理层）

rtl8169 的手册：<https://www.semiee.com/file/EOL2/Realtek-RTL8168-LF.pdf>

PHY 分 MAC 侧和 PHY 侧，我们这次主要看 PHY 是否有变化，以及 中断 mask 是否有变化导致网络中断上不来。

### 网上找的一个小工具 mdio

我先是找到了这个[工具](https://blog.csdn.net/qq_40083589/article/details/102495601)，但它只支持 PHY 侧的状态查询

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/mii.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <netinet/in.h>


#define reteck(ret)    \
        if(ret < 0){    \
            printf("%m! \"%s\" : line: %d\n", __func__, __LINE__);  \
            goto lab;  \
        }

#define help() \
    printf("mdio:\n");                  \
    printf("read operation: mdio reg_addr\n");          \
    printf("write operation: mdio reg_addr value\n");    \
    printf("For example:\n");            \
    printf("mdio eth0 1\n");            \
    printf("mdio eth0 0 0x12\n\n");      \
    exit(0);

int sockfd;

int main(int argc, char *argv[]){
        
    if(argc == 1 || !strcmp(argv[1], "-h")){
        help();
    }
    
    struct mii_ioctl_data *mii = NULL;
    struct ifreq ifr;
    int ret;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ - 1);

    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    reteck(sockfd);

    //get phy address in smi bus
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);
    reteck(ret);

    mii = (struct mii_ioctl_data*)&ifr.ifr_data;
	

    if(argc == 3){

        mii->reg_num    = (uint16_t)strtoul(argv[2], NULL, 0);
        
        ret = ioctl(sockfd, SIOCGMIIREG, &ifr);
        reteck(ret);
		
    
        printf("read phy addr: 0x%x  reg: 0x%x  value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_out);
 }else if(argc == 4){

        mii->reg_num    = (uint16_t)strtoul(argv[2], NULL, 0);
        mii->val_in    = (uint16_t)strtoul(argv[3], NULL, 0);

        ret = ioctl(sockfd, SIOCSMIIREG, &ifr);
        reteck(ret);

        printf("write phy addr: 0x%x  reg: 0x%x  value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_in);
    }

lab:
    close(sockfd);
    return 0;
}
```

编译后使用方法：

* 读取：

```console
# ./mdio eth0 0x00
```

* 修改：

```console
# ./mdio eth0 0x00 0x00
```

### `ethtool`

ethtool 可以同时直接看 PHY侧 和 MAC 侧的 PHY 芯片的状态：

```console
ethtool -d eth0
```

```output
RealTek RTL8168g/8111g registers:
--------------------------------------------------------
0x00: MAC Address                      ma:c0:ad:dr:es:s0
0x08: Multicast Address Filter     0x02020040 0x80400083
0x10: Dump Tally Counter Command   0x1303b000 0x00000001
0x20: Tx Normal Priority Ring Addr 0x02a25000 0x00000001
0x28: Tx High Priority Ring Addr   0x00000000 0x00000000
0x30: Flash memory read/write                 0x00000000
0x34: Early Rx Byte Count                              0
0x36: Early Rx Status                               0x00
0x37: Command                                       0x0c
      Rx on, Tx on
0x3C: Interrupt Mask                              0x003f
      LinkChg RxNoBuf TxErr TxOK RxErr RxOK
0x3E: Interrupt Status                            0x0000

0x40: Tx Configuration                        0x53900f80
0x44: Rx Configuration                        0x0002cf0e
0x48: Timer count                             0x00000000
0x4C: Missed packet counter                     0x000000
0x50: EEPROM Command                                0x10
0x51: Config 0                                      0x00
0x52: Config 1                                      0xcf
0x53: Config 2                                      0x1c
0x54: Config 3                                      0x60
0x55: Config 4                                      0x11
0x56: Config 5                                      0x00
0x58: Timer interrupt                         0x00000000
0x5C: Multiple Interrupt Select                   0x0000
0x60: PHY access                              0x00000000
0x64: TBI control and status                  0x00000000
0x68: TBI Autonegotiation advertisement (ANAR)    0x0000
0x6A: TBI Link partner ability (LPAR)             0x0000
0x6C: PHY status                                    0x84
0x84: PM wakeup frame 0            0x00000000 0x00000000
0x8C: PM wakeup frame 1            0x00000000 0x00000000
0x94: PM wakeup frame 2 (low)      0x00000000 0x00000000
0x9C: PM wakeup frame 2 (high)     0x00000000 0x00000000
0xA4: PM wakeup frame 3 (low)      0x00000000 0x00000000
0xAC: PM wakeup frame 3 (high)     0x00000000 0x00000000
0xB4: PM wakeup frame 4 (low)      0x00000000 0xd20a0000
0xBC: PM wakeup frame 4 (high)     0x00000000 0x00000000
0xC4: Wakeup frame 0 CRC                          0x0000
0xC6: Wakeup frame 1 CRC                          0x0000
0xC8: Wakeup frame 2 CRC                          0x0000
0xCA: Wakeup frame 3 CRC                          0x0000
0xCC: Wakeup frame 4 CRC                          0x0000
0xDA: RX packet maximum size                      0x4000
0xE0: C+ Command                                  0x2060
      VLAN de-tagging
      RX checksumming
0xE2: Interrupt Mitigation                        0x0000
      TxTimer:       0
      TxPackets:     0
      RxTimer:       0
      RxPackets:     0
0xE4: Rx Ring Addr                 0x03fef000 0x00000001
0xEC: Early Tx threshold                            0x27
0xF0: Func Event                              0x0000003f
0xF4: Func Event Mask                         0x00000000
0xF8: Func Preset State                       0x00000003
0xFC: Func Force Event                        0x00000000
```

但ethtool 并不支持解析 r8168 驱动，只会返回给我们裸数据，所以我们需要添加r8168，用r8169来解析这个裸数据，并自己编译。添加的位置：是 `driver_list[]={}`。虽说这可能会有些问题，有些状态不对，但由于我们只需看中断mask，所以可以直接拿来用。根据从手册上说的，r8168 和 r8169 中断mask的位置是同一个所以我们可以直接看了。


