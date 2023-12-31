# DASICS CVE-2021-3156 

## 版本要求

`sudo`：1.8.31

`glibc`：2.31

`Linux`：4.18 或 5.10.167-gee203559bc0b-dirty

## 使用方法

为了方便，在编译 `Linux` 时，不能使用 `initramfs` 方式，而使用本仓库中提供的 `rootfs.img` 作为根目录启动，内部包含了支持 `sudo` 以及 `nss` 服务正常工作的必要配置文件，是一个极其简单的发行版。

利用 `DASICS_QEMU` 提供的 `qemu` 以 `rootfs.img` 为根目录启动 `bbl`。此外，也可以利用 `OpenSBI` 启动 `linux`。示例命令如下：

```shell
#!/bin/sh
qemu-system-riscv64 -M virt -m 1G \
        -cpu rv64 \
        -nographic \
        -kernel bbl \
        -drive file=rootfs.img,format=raw,id=hd0 \
        -device virtio-blk-device,drive=hd0 \
        -append "console=ttyS0 rw root=/dev/vda" \
        -bios none  -s
```

进入 `busybox` 命令行界面后执行如下命令：

```shell
$ su wanghan
$ cd
$ whoami
$ ./expolit
$ whoami
```

由于系统启动进入命令行界面后便是 `root` 权限，此时可以使用 `passwd` 轻松修改密码。 

##  效果演示

<img src="image/image-20231230223439990.png" alt="image-20231230223439990" style="zoom: 50%;" />

可见，`expolit` 实现了提权，并可以稳定复现。
