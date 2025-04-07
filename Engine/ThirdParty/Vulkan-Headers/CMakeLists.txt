# Copyright (C) Eser Kokturk. All Rights Reserved.

# Vulkan Headers

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

# Fetch Vulkan validation layer libs for Android flavors
if(ANDROID AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(VULKAN_SDK_VERSION "1.4.309.0")
    set(VULKAN_ANDROID_VALIDATION_VERSION "android-binaries-${VULKAN_SDK_VERSION}")
    set(VULKAN_ANDROID_VALIDATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VULKAN_ANDROID_VALIDATION_VERSION})

    if(NOT EXISTS ${VULKAN_ANDROID_VALIDATION_DIR})
        set(DOWNLOAD_VULKAN_ANDROID_VALIDATION true)
    else()
    file(GLOB VULKAN_ANDROID_VALIDATION_LIBS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${VULKAN_ANDROID_VALIDATION_DIR}/*/libVkLayer_khronos_validation.so)
    list(LENGTH VULKAN_ANDROID_VALIDATION_LIBS VULKAN_ANDROID_VALIDATION_LIB_COUNT)
        if(NOT ${VULKAN_ANDROID_VALIDATION_LIB_COUNT} EQUAL 4)
            file(REMOVE_RECURSE ${VULKAN_ANDROID_VALIDATION_DIR})
            set(DOWNLOAD_VULKAN_ANDROID_VALIDATION true) 
        endif()
    endif()

    if(DOWNLOAD_VULKAN_ANDROID_VALIDATION)
        set(VULKAN_ANDROID_VALIDATION_LINK https://github.com/KhronosGroup/Vulkan-ValidationLayers/releases/download/vulkan-sdk-${VULKAN_SDK_VERSION}/${VULKAN_ANDROID_VALIDATION_VERSION}.zip)
        file(DOWNLOAD 
            ${VULKAN_ANDROID_VALIDATION_LINK}
            ${VULKAN_ANDROID_VALIDATION_DIR}.zip 
            STATUS DOWNLOAD_RESULT
            INACTIVITY_TIMEOUT 10
        )
        if(NOT DOWNLOAD_RESULT EQUAL 0)
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Couldn't download Vulkan validation layer from ${VULKAN_ANDROID_VALIDATION_LINK}")
        endif()
        file(ARCHIVE_EXTRACT INPUT ${VULKAN_ANDROID_VALIDATION_DIR}.zip  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
        file(REMOVE ${VULKAN_ANDROID_VALIDATION_DIR}.zip)
    endif()

    set(VULKAN_VALIDATION_LIB ${VULKAN_ANDROID_VALIDATION_DIR}/${CMAKE_ANDROID_ARCH_ABI}/libVkLayer_khronos_validation.so)
    if(NOT EXISTS ${VULKAN_VALIDATION_LIB})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Vulkan validation layer library is not found at ${VULKAN_VALIDATION_LIB}")
    endif()

    get_property(EXTRA_ANDROID_LIBS GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS)
    set_property(GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS ${EXTRA_ANDROID_LIBS}
        ${VULKAN_ANDROID_VALIDATION_DIR}/${CMAKE_ANDROID_ARCH_ABI}/libVkLayer_khronos_validation.so
    )
endif()

add_subdirectory(.submodule)
# TODO: Make a dummy uninstall for the engine
set_target_properties(uninstall PROPERTIES FOLDER ${ZERON_THIRDPARTY_FOLDER_PREFIX}/Vulkan-Headers)