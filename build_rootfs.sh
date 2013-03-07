#!/bin/sh

BUSYBOX_INSTALL_DIR=${1}
ARCH_LIB_PATH=${2}
echo "BUSYBOX_INSTALL_DIR=${BUSYBOX_INSTALL_DIR}\n"
echo "ARCH_LIB_PATH=${ARCH_LIB_PATH}\n"

#create base dir
cp -r ${BUSYBOX_INSTALL_DIR} ./rootfs
cp -r ./base_rootfs/etc ./rootfs/

mkdir ./rootfs/proc
mkdir ./rootfs/sys
mkdir ./rootfs/dev
mkdir ./rootfs/home
mkdir ./rootfs/home/root
mkdir ./rootfs/lib
mkdir ./rootfs/mnt
mkdir ./rootfs/tmp
mkdir ./rootfs/var

#enable excute for all script
chmod +x ./rootfs/etc/

cp ${ARCH_LIB_PATH}/* ./rootfs/lib

#create device
cd rootfs
cd dev
mknod console c 5 1
mknod null c 1 3
 