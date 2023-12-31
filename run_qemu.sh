#!/bin/sh
qemu-system-riscv64 -M virt -m 1G \
	-cpu rv64 \
    -nographic \
	-kernel bbl \
	-drive file=rootfs.img,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
	-append "console=ttyS0 rw root=/dev/vda" \
	-bios none  -s
