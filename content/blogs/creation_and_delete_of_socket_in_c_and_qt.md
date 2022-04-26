---
title: "C 和 Qt 中 socket 的创建和删除"
date: 2022-04-14T08:36:19+08:00
lastMod: 2022-04-14T08:36:19+08:00
code: true
mermaid: false
draft: false
author: "Ben"
tag: ["network", "socket", "qt", "c"]
---


# C 和 Qt 中 socket 的创建和删除

[toc]

## C

[example code](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)

### 客户端
客户端相对来说比较简单：创建，然后 connect

```c
connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
```

### 服务端
服务端相对来说会复杂一些

```c
bind(sockfd, (SA*)&servaddr, sizeof(servaddr));
listen(sockfd, 5);
connfd = accept(sockfd, (SA*)&cli, &len);
```

先新建一个 socket 然后这个socket 来做监听，用 accept 取到和客户端连接的服务端的socket

从 `man 2 accept` 可以知道 accept 做了这些事：

1. 从这个监听的 socket 中取出第一个连接请求
2. 创建一个新的连接着的socket，
3. 返回一个指向这个 socket 的文件描述符

这里返回的 socket 是一个变量，也就是说它是在栈里的，当退出函数的时候，这个 socket 会自动释放。

#### 文件描述符：
文件描述符号是 int。

在 socket 断连再重连的时候我们看到对于一个新的 socket，他的文件描述符没变，那 accept 建立的socket 还是原来的吗？答案是否定的。

文件描述符在系统中在这里 `/proc/<PID>/fd/<fdNum>` 虽然新的socket 的描述符还是 21，但它的软连接对象已经变了。

```
/proc/30475/fd/21 -> socket:[31166317]
/proc/30475/fd/21 -> socket:[31206486]

cat /proc/net/tcp |grep 31206486
```

## Qt
Qt 就结构上也差不多，只不过它把服务端的监听的 socket 抽象成了 `QTcpServer`，但有一个区别就是 accept 在 Qt 中的对应是 `QTcpSocket *QTcpServer::nextPendingConnection()`，注意这里返回的是一个指针也就是说它现在不再栈中了，而是在堆中，那它是在什么时候建的呢，我是否可以删除它？那就去翻源码咯(doge)

```cpp
QTcpSocket *QTcpServer::nextPendingConnection()
    return d->pendingConnections.takeFirst();

void QTcpServer::addPendingConnection(QTcpSocket* socket)
    d_func()->pendingConnections.append(socket);

void QTcpServer::incomingConnection(qintptr socketDescriptor)
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);

void QTcpServerPrivate::readNotification()
    int descriptor = socketEngine->accept();
    q->incomingConnection(descriptor);
```

我们可以看到 `pendingConnections` 里面的 `socket` 是在 `incommingConnection` 那里 `new` 的

那我们什么时候去 delete 呢，查阅 [隐式共享](https://doc.qt.io/qt-5/implicit-sharing.html)，QAbstractsocket 不在其中，所以需要对它进行手动释放。
在 [stack overflow 的这个回答](https://stackoverflow.com/questions/28820450/how-to-safely-delete-a-qtqtcpsocket) 中就说了，在 QTcpServer 删除的时候会把所有的socket 都释放掉，也就是 Qt 释放时的树状结构。但为了追求高效率，我们可以手动去释放掉。根据 Qt 的 [手册](https://doc.qt.io/archives/qt-4.8/objecttrees.html): When any QObject in the tree is deleted, if the object has a parent, the destructor automatically removes the object from its parent. 所以你可以放心地删除，而不必担心析构两次。

另外在 disconnect 的信号的帮助里也有说到：

```help
[signal] void QAbstractSocket::disconnected()
Warning: If you need to delete the sender() of this signal in a slot connected to it, use the deleteLater() function.
```
