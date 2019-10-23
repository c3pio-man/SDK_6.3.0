host_build {
    QT_CPU_FEATURES.x86_64 = mmx sse sse2
} else {
    QT_CPU_FEATURES.arm = neon
}
QT.global_private.enabled_features = alloca_h alloca dbus dbus-linked gui network posix_fallocate reduce_exports release_tools sql system-zlib widgets xml
QT.global_private.disabled_features = sse2 alloca_malloc_h android-style-assets avx2 private_tests gc_binaries libudev reduce_relocations stack-protector-strong testlib
PKG_CONFIG_EXECUTABLE = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/bin/pkg-config
QMAKE_LIBS_DBUS = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib/libdbus-1.so
QMAKE_INCDIR_DBUS = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/include/dbus-1.0 /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot//usr/lib/dbus-1.0/include
QT_COORD_TYPE = double
QMAKE_LIBS_ZLIB = /srv/jenkins/workspace/qt-5.12.4-builder/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot/usr/lib/libz.so
CONFIG += cross_compile compile_examples enable_new_dtags largefile neon precompile_header
QT_BUILD_PARTS += libs
QT_HOST_CFLAGS_DBUS += 
