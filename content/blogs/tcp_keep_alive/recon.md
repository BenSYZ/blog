### 后续的 bug, `keepAlive`

* Problem: 网络状态不佳的情况下，client 端没收到 Server 发送的四次挥手，导致 TCP 层并不知道下层断了 (`netstat -pant` 查出来是`ESTABILISHED`)，也就导致上层收不到 `disconnect` 信号。
* Solution: 添加TCP `keepAlive` 心跳机制。keepAlive 简单来说就是当 socket 中无数据交流一段时间之后发个探测报文(seq 比正常下次要发的-1，这样回传的时候ack又和上次一样了，也就保证昨晚 keepAlive 之后的状态和未作之前的状态一样)，探测网络是否正常。

(关于 `keepAlive` 原理的介绍后面有空会照着抓包信息来分析一下。)

而 Qt 只提供了 `keepAlive` 的使能位的接口，没有提供详细参数的设定

* `QTCPSocket::setSocketOption(QAbstractSocket::KeepAliveOption,1)`

查阅资料后发现 `socket->socketDescriptor()` 可以获取到封装在 `QTCPSocket` 内部的底层的 socket，所以可以用 C 的方式设置 socket 的参数

```cpp
void ComTcpSoket::setSocketKeepAlive(int keepAlive, int keepIdle, int keepInterval, int keepCount){
    // 开启keepalive属性. 缺省值: 0(关闭)
    // 如果在60秒内没有任何数据交互,则进行探测. 缺省值:7200(s)
    // 探测时发探测包的时间间隔为5秒. 缺省值:75(s)
    // 探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)

    setsockopt(this->socketDescriptor(), SOL_SOCKET, SO_KEEPALIVE,(void*)&keepAlive , sizeof(keepAlive));
    setsockopt(this->socketDescriptor(), SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
    setsockopt(this->socketDescriptor(), SOL_TCP, TCP_KEEPINTVL, (void*)&keepInterval, sizeof(keepInterval));
    setsockopt(this->socketDescriptor(), SOL_TCP, TCP_KEEPCNT, (void*)&keepCount, sizeof(keepCount));
}
```
