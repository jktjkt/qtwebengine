GYP_ARGS += "-D qt_os=\"desktop_linux\""

include(linux.pri)

GYP_CONFIG += \
    desktop_linux=1 \
    enable_widevine=1 \
    enable_pdf=1

# Currently a linux-clang-libc++ works only if sanitizer is used as well, because it uses a
# Chromium provided libc++ library, and not the system one.
linux-clang|if(linux-clang-libc++:sanitizer) {
    GYP_CONFIG += clang=1 host_clang=1 clang_use_chrome_plugins=0 make_clang_dir=/usr
    linux-clang-libc++: GYP_CONFIG += use_system_libcxx=1
} else {
    GYP_CONFIG += clang=0 host_clang=0
}
