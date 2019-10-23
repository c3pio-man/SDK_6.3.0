!host_build {
    QMAKE_CFLAGS    += --sysroot=$$[QT_SYSROOT]
    QMAKE_CXXFLAGS  += --sysroot=$$[QT_SYSROOT]
    QMAKE_LFLAGS    += --sysroot=$$[QT_SYSROOT]
}
host_build {
    QT_ARCH = x86_64
    QT_BUILDABI = x86_64-little_endian-lp64
    QT_TARGET_ARCH = arm
    QT_TARGET_BUILDABI = arm-little_endian-ilp32-eabi
} else {
    QT_ARCH = arm
    QT_BUILDABI = arm-little_endian-ilp32-eabi
}
QT.global.enabled_features = cross_compile shared rpath c++11 c++14 c99 c11 thread future concurrent pkg-config
QT.global.disabled_features = framework appstore-compliant debug_and_release simulator_and_device build_all c++1z force_asserts separate_debug_info static
PKG_CONFIG_SYSROOT_DIR = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot/
PKG_CONFIG_LIBDIR = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib/pkgconfig:/srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/share/pkgconfig:/srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib/arm-obreey-linux-gnueabi/pkgconfig
CONFIG += cross_compile shared release
QT_CONFIG += shared rpath release c++11 c++14 concurrent dbus reduce_exports release_tools stl
QT_VERSION = 5.12.4
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 12
QT_PATCH_VERSION = 4
QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 2
QT_GCC_PATCH_VERSION = 1
QT_CLANG_MAJOR_VERSION = 7
QT_CLANG_MINOR_VERSION = 0
QT_CLANG_PATCH_VERSION = 0
QT_EDITION = OpenSource
