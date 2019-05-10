EXTRA_INCLUDEPATH += /srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/arm-obreey-linux-gnueabi/sysroot/usr/include
host_build {
    QT_CPU_FEATURES.x86_64 = mmx sse sse2
} else {
    QT_CPU_FEATURES.arm = neon
}
QT.global_private.enabled_features = alloca_h alloca dbus dbus-linked gui network posix_fallocate reduce_exports release_tools sql system-zlib testlib widgets xml
QT.global_private.disabled_features = sse2 alloca_malloc_h android-style-assets avx2 private_tests libudev reduce_relocations stack-protector-strong
PKG_CONFIG_EXECUTABLE = /srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/bin/pkg-config
QMAKE_LIBS_DBUS = -L/srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib -ldbus-1
QMAKE_INCDIR_DBUS = /srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/arm-obreey-linux-gnueabi/sysroot//usr/include/dbus-1.0 /srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib/dbus-1.0/include
QT_COORD_TYPE = double
QMAKE_LIBS_ZLIB = -lz
CONFIG += cross_compile compile_examples enable_new_dtags largefile neon precompile_header
QT_BUILD_PARTS += libs
QT_HOST_CFLAGS_DBUS += -I/srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/bin/../arm-obreey-linux-gnueabi/sysroot//usr/include/dbus-1.0 -I/srv/jenkins/workspace/qt-5.11.2-builder/SDK-A13/usr/bin/../arm-obreey-linux-gnueabi/sysroot//usr/lib/dbus-1.0/include
