if($ENV{ANDROID_SDK_ROOT} STREQUAL "")
    message(FATAL_ERROR "${ZERON_ERROR_MSG} ANDROID_SDK_ROOT environment variable is not set!")
endif()

if(ANDROID_NDK_VERSION STREQUAL "")
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Expected ANDROID_NDK_VERSION to be defined for toolchain!")
endif()

file(TO_CMAKE_PATH $ENV{ANDROID_SDK_ROOT}/ndk/${ANDROID_NDK_VERSION} CMAKE_ANDROID_NDK)
set(ANDROID_NDK ${CMAKE_ANDROID_NDK} CACHE INTERNAL "Android NDK directory")


