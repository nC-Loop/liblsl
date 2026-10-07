# Colibri iMX6 (LooPIT device): Toradex SDK 5.7.2 (tdx-xwayland-rt), GCC 9.5.
# The SDK's own CMake is 3.16; liblsl 1.17 needs >= 3.23, so the host's CMake
# drives the SDK compilers through this file.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(TDX_SDK "/opt/tdx-xwayland-rt/5.7.2" CACHE PATH "Toradex SDK root")
set(CMAKE_SYSROOT "${TDX_SDK}/sysroots/armv7at2hf-neon-tdx-linux-gnueabi")
set(TDX_BIN "${TDX_SDK}/sysroots/x86_64-tdxsdk-linux/usr/bin/arm-tdx-linux-gnueabi")
set(CMAKE_C_COMPILER "${TDX_BIN}/arm-tdx-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "${TDX_BIN}/arm-tdx-linux-gnueabi-g++")
set(CMAKE_STRIP "${TDX_BIN}/arm-tdx-linux-gnueabi-strip")
set(CMAKE_OBJCOPY "${TDX_BIN}/arm-tdx-linux-gnueabi-objcopy")
# the framework's flags (recipes/build.recipe, colibri)
set(CMAKE_C_FLAGS_INIT "-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
