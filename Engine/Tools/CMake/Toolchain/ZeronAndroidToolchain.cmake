if($ENV{ANDROID_SDK_ROOT} STREQUAL "")
    message(FATAL_ERROR "ZERON - ANDROID_SDK_ROOT environment variable is not set!")
endif()

set(ANDROID_NDK_VERSION 24.0.8215888 CACHE INTERNAL "Android NDK version")
file(TO_CMAKE_PATH $ENV{ANDROID_SDK_ROOT}/ndk/${ANDROID_NDK_VERSION} CMAKE_ANDROID_NDK)
set(ANDROID_NDK ${CMAKE_ANDROID_NDK} CACHE INTERNAL "Android NDK directory")


set(ANDROID_STL_LIB_DIR ${CMAKE_ANDROID_NDK}/sources/cxx-stl/llvm-libc++/libs/${CMAKE_ANDROID_ARCH_ABI})
set(ANDROID_STL_INCLUDE_DIR ${CMAKE_ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include)
if(NOT EXISTS ${ANDROID_STL_LIB_DIR})
    message(FATAL_ERROR "Zeron - C++ STL libraries directory cannot be found!")
endif()
set(ANDROID_STL_LIB_DIR ${ANDROID_STL_LIB_DIR} CACHE INTERNAL "Android C++ STL Library Dir")


