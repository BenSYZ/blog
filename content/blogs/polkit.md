---
title: "polkit"
date: 2022-06-06T08:55:33+08:00
lastMod: 2022-06-06T08:55:33+08:00
code: true
mermaid: false
draft: false
author: "Ben"
description: "非特权进程向特权进程发消息的控制"
tags: ["polkit", "Linux", "permission", "learns"]
---

# polkit

refs:

* <https://wiki.archlinux.org/title/Polkit>
* <https://www.freedesktop.org/software/polkit/docs/latest/polkit.8.html>

----

polkit 是非特权进程向特权进程发消息的控制，从而实现特权功能，比如，普通用户的挂载和卸载硬盘

polkit 配置有两种方式，一种是应用自带的，叫Action，(`.policy`)；另一种是类似补丁的形式，叫Authorization rules,(`.rules` or `.pkla`)

## Action:
policy file 文件位置：

`/usr/share/polkit-1/actions/*.policy`

在 default section 中，有这几个选项:

* keys: 
    + allow_any: 所有
    + allow_inactive: 远程的 session(ssh vnc, etc.)
    + allow_active: 本地  tty, X display
* values:
    + no                -
    + yes               -
    + auth_self[_keep]  non-sudoer [keep some minutes]
    + auth_admin[_keep] sudoer


## Authorization rules
文件位置：

`/etc/polkit-1/rules.d/*.rules`

Tips:

* 顺序是 `00-test.rules` 比 `99-test.rules` 更早验证，即，后面的不会覆写前面的。（区别于 `/etc/udev/hwdb.d/`）因为这里直接 return 了
* `subject.local: true/false` responding to `allow_active`, etc in policy
* `systemctl status polkit` 可以查看 Action name

### examples:

* `addAdminRule()`
    需要特权时，使用何种身份验证是否有特权
    ```rules
    # wheel 组的用户就可以使用这个特权
    polkit.addAdminRule(function(action, subject) {
        return ["unix-group:wheel"];
    });
    ```

    ```console
    [test@Misaka ben]$ systemctl restart polkit.service
    ==== AUTHENTICATING FOR org.freedesktop.systemd1.manage-units ====
    Authentication is required to restart 'polkit.service'.
    Authenticating as: ben (ben is in wheel)
    ```

    ```rules
    # root 用户才可以使用这个特权
    polkit.addAdminRule(function(action, subject) {
        return ["unix-user:root"];
    });
    ```

    ```console
    [test@Misaka ben]$ systemctl restart polkit.service
    ==== AUTHENTICATING FOR org.freedesktop.systemd1.manage-units ====
    Authentication is required to restart 'polkit.service'.
    Authenticating as: root
    ```
* `addRule()`
    + change rule
        ```rules
        polkit.addRule(function(action, subject) {
            if (action.id == "org.gnome.gparted" &&
                subject.isInGroup("admin")) {
                return polkit.Result.YES;
            }
        });
        ```
    + log
        ```rules
        polkit.addRule(function(action, subject) {
            if (action.id == "org.freedesktop.policykit.exec") {
                polkit.log("action=" + action);
                polkit.log("subject=" + subject);
            }
        });
        ```

### old Authorization rules

`pkaction --version`, rules not work for pkaction less then 106

<https://askubuntu.com/questions/536591/policykit-rules-never-come-into-effect>

```conf
#/etc/polkit-1/localauthority/50-local.d/00-udisk-umount.pkla
# section: random name
# man pklocalauthority(8)
[Allow]
Identity=unix-user:*
Action=org.freedesktop.udisks2.filesystem-unmount-others
ResultAny=yes
ResultInactive=no
ResultActive=yes
```

