# Copyright (C) Eser Kokturk. All Rights Reserved.

# Vulkan Headers

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

# Fetch Vulkan validation layer libs for Android flavors
if(ANDROID)
    set(VULKAN_SDK_VERSION "1.3.243.0")
    set(VULKAN_ANDROID_VALIDATION_FOLDER "android-sdk-${VULKAN_SDK_VERSION}")
    set(VULKAN_ANDROID_VALIDATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VULKAN_ANDROID_VALIDATION_FOLDER})

    if (NOT EXISTS ${VULKAN_ANDROID_VALIDATION_DIR})
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
        set(VULKAN_ANDROID_VALIDATION_LINK https://github.com/KhronosGroup/Vulkan-ValidationLayers/releases/download/sdk-${VULKAN_SDK_VERSION}/android-binaries-${VULKAN_SDK_VERSION}.zip)
        file(DOWNLOAD 
            ${VULKAN_ANDROID_VALIDATION_LINK}
            ${VULKAN_ANDROID_VALIDATION_DIR}.zip 
            INACTIVITY_TIMEOUT 10
        )
        file(ARCHIVE_EXTRACT INPUT ${VULKAN_ANDROID_VALIDATION_DIR}.zip  DESTINATION ${VULKAN_ANDROID_VALIDATION_DIR})
        file(REMOVE ${VULKAN_ANDROID_VALIDATION_DIR}.zip)
    endif()

    # TODO: Only add this for debug builds
    get_property(EXTRA_ANDROID_LIBS GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS)
    set_property(GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS ${EXTRA_ANDROID_LIBS}
        ${VULKAN_ANDROID_VALIDATION_DIR}/${CMAKE_ANDROID_ARCH_ABI}/libVkLayer_khronos_validation.so
    )
endif()

add_subdirectory(.submodule)
# TODO: Make a dummy uninstall for the engine
set_target_properties(uninstall PROPERTIES FOLDER ${ZERON_THIRDPARTY_FOLDER_PREFIX}/Vulkan-Headers)