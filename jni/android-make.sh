#!/bin/sh
DEV_PREFIX=/opt
ANDROID_NDK=${DEV_PREFIX}/ndk
CROSS_COMPILE=arm-linux-androideabi
ANDROID_PREFIX=/tmp/my-android-toolchain
SYSROOT=${ANDROID_NDK}/platforms/android-17/arch-arm
CROSS_PATH=${ANDROID_PREFIX}/bin/${CROSS_COMPILE}
CPP=${CROSS_PATH}-cpp
AR=${CROSS_PATH}-ar
AS=${CROSS_PATH}-as
NM=${CROSS_PATH}-nm
CC=${CROSS_PATH}-gcc
CXX=${CROSS_PATH}-g++
LD=${CROSS_PATH}-ld
RANLIB=${CROSS_PATH}-ranlib
PREFIX=/data/local/tmp
PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
CFLAGS="-UHAVE_LOCALE_H --sysroot=${SYSROOT} -I${SYSROOT}/usr/include -I${ANDROID_PREFIX}/include -I${DEV_PREFIX}/android/bionic "
CPPFLAGS="${CFLAGS}"
LDFLAGS="${LDFLAGS} -L${SYSROOT}/usr/lib -L${ANDROID_PREFIX}/lib"
EFTE_INSTALL_DIR=${PREFIX}
$NDK/ndk-build 
