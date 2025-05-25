# Add a new Android Project library that will be copied over to the package
# `lib`: Full path to library file
function(zeron_android_add_external_lib lib)
    if(NOT EXISTS ${lib})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Library file is not found '${lib}'")
    endif()
    get_property(EXTRA_ANDROID_LIBS GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS)
    set_property(GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS ${EXTRA_ANDROID_LIBS} ${lib})
endfunction()

# Create an Android Project in build directory using the Zeron Engine template
# `target`: Target to create the project for
# `packageName`: Name of the Android package (e.g. "com.zeron.sandbox")
# `appName`: Name of the Android application (e.g. "Sandbox")
# `appSourceDir`: C++ source files to include for the project
macro(zeron_android_configure_package target packageName appName appSourceDir)
    if(ANDROID)
        # Target
        if(NOT TARGET ${target})
            message(FATAL_ERROR "${ZERON_ERROR_MSG}  Invalid target for Android!")
        endif()
        set(ZERON_ANDROID_TARGET ${target})
        
        # App Name
        if(${packageName} STREQUAL "")
            set(ZERON_ANDROID_APP_NAME "Zeron Engine App")
        else()
            set(ZERON_ANDROID_APP_NAME ${appName})
        endif()

        # Package name
        if(${packageName} STREQUAL "")
            set(ZERON_ANDROID_PACKAGE_NAME "com.zeronengine.application")
        else()
            set(ZERON_ANDROID_PACKAGE_NAME ${packageName})
        endif()

        # Get template Android project directory
        get_property(ZERON_ANDROID_PROJECT_DIR GLOBAL PROPERTY ZERON_ANDROID_PROJECT_DIR)
        if(NOT DEFINED ZERON_ANDROID_PROJECT_DIR)
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Unable to locate template Zeron Android project directory!")
        endif()

        set(ZERON_ANDROID_PACKAGE_DIR               ${CMAKE_BINARY_DIR}/AndroidProject/${target})
        set(ZERON_ANDROID_PACKAGE_MAIN_DIR          ${ZERON_ANDROID_PACKAGE_DIR}/app/src/main)

        set(ZERON_ANDROID_PROJECT_MAIN_DIR          ${ZERON_ANDROID_PROJECT_DIR}/app/src/main/)
        set(ZERON_ANDROID_PROJECT_RESOURCE_DIR      ${ZERON_ANDROID_PROJECT_MAIN_DIR}/res)

        message("ZERON - Generating: Android project at ${ZERON_ANDROID_PACKAGE_DIR}")

        # Cleanup when configuration changes
        file(REMOVE_RECURSE ${ZERON_ANDROID_PACKAGE_DIR})

        # Copy gradle dependencies
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/gradle               DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/build.gradle         DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/gradle.properties    DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/gradlew              DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/gradlew.bat          DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})
        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/settings.gradle      DESTINATION ${ZERON_ANDROID_PACKAGE_DIR})

        if(EXISTS ${ZERON_ANDROID_CUSTOM_RESOURCE_DIR})
            message("ZERON --- Using custom Android package resources from '${ZERON_ANDROID_CUSTOM_RESOURCE_DIR}'")
            set(ZERON_ANDROID_PROJECT_RESOURCE_DIR ${ZERON_ANDROID_CUSTOM_RESOURCE_DIR})
        endif()

        file(COPY ${ZERON_ANDROID_PROJECT_RESOURCE_DIR}/mipmap-hdpi     DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res)
        file(COPY ${ZERON_ANDROID_PROJECT_RESOURCE_DIR}/mipmap-mdpi     DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res)
        file(COPY ${ZERON_ANDROID_PROJECT_RESOURCE_DIR}/mipmap-xhdpi    DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res)
        file(COPY ${ZERON_ANDROID_PROJECT_RESOURCE_DIR}/mipmap-xxhdpi   DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res)

        # Asset directory for external files
        file(MAKE_DIRECTORY ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/assets)
        set_property(GLOBAL PROPERTY ZERON_BUILD_ASSETS_DIR ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/assets)

        file(COPY ${ZERON_ANDROID_PROJECT_DIR}/app/proguard-rules.pro   DESTINATION ${ZERON_ANDROID_PACKAGE_DIR}/app)

        if(EXISTS ${appSourceDir})
            set(ZERON_APP_DIR "\'${appSourceDir}\'")
        endif()

        # Configuration settings for the application
        if(NOT DEFINED ZERON_ANDROID_CONFIG_SCREEN_ORIENTATION)
            set(ZERON_ANDROID_CONFIG_SCREEN_ORIENTATION "unspecified") # "landscape" | "portrait"
        endif()

        # Copy configured package files
        configure_file(${ZERON_ANDROID_PROJECT_DIR}/app/build.gradle 
                    ${ZERON_ANDROID_PACKAGE_DIR}/app/build.gradle @ONLY)

        configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/AndroidManifest.xml 
                    ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/AndroidManifest.xml @ONLY)

        configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/res/values/strings.xml 
                    ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res/values/strings.xml @ONLY)

        configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/res/values/themes.xml 
                    ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res/values/themes.xml @ONLY)

        configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/java/com.application/AppActivity.java 
                    ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/java/${ZERON_ANDROID_PACKAGE_NAME}/${ZERON_ANDROID_TARGET}Activity.java @ONLY)

        file(COPY ${ZERON_ANDROID_PROJECT_MAIN_DIR}/java/com.zeron.engine/ DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/java/com.zeron.engine/)

        # Find dependency directories
        set(ANDROID_CPP_DEP_DIR ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++)
        if(EXISTS ${ANDROID_CPP_DEP_DIR})
            set(ANDROID_CPP_LIB_DIR ${ANDROID_CPP_DEP_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI})
            set(ANDROID_CPP_INCLUDE_DIR ${ANDROID_NDK}/include)
        else()
            set(ANDROID_CPP_DEP_DIR ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr)
            if(NOT EXISTS ${ANDROID_CPP_DEP_DIR})
                message(FATAL_ERROR "${ZERON_ERROR_MSG} Missing NDK C++ dependency directory: ${ANDROID_CPP_DEP_DIR}")
            endif()

            if(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
                set(_androidCppDepArch "x86_64-linux-android")
            elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
                set(_androidCppDepArch "aarch64-linux-android")
            else()
                message(FATAL_ERROR "${ZERON_ERROR_MSG} Unsupported Android arch: ${CMAKE_ANDROID_ARCH_ABI}")
            endif()

            set(ANDROID_CPP_LIB_DIR ${ANDROID_CPP_DEP_DIR}/lib/${_androidCppDepArch})
            set(ANDROID_CPP_INCLUDE_DIR ${ANDROID_CPP_DEP_DIR}/include/c++/v1)
        endif()
        if(NOT EXISTS ${ANDROID_CPP_LIB_DIR})
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Missing C++ library directory: ${ANDROID_CPP_LIB_DIR}")
        endif()
        if(NOT EXISTS ${ANDROID_CPP_INCLUDE_DIR})
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Missing C++ include directory: ${ANDROID_CPP_INCLUDE_DIR}")
        endif()

        # Copy libraries
        get_property(EXTRA_ANDROID_LIBS GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS)
        file(GLOB_RECURSE ANDROID_STL_LIB ${ANDROID_CPP_LIB_DIR}/lib${CMAKE_ANDROID_CPP_LIB_TYPE}.*)
        if(NOT EXISTS ${ANDROID_STL_LIB})
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Unable to locate C++ library for Android!")
        endif()
        file(COPY ${ANDROID_STL_LIB} DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/jniLibs/${CMAKE_ANDROID_ARCH_ABI})
        foreach(extraLib ${EXTRA_ANDROID_LIBS})
            file(COPY ${extraLib} DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/jniLibs/${CMAKE_ANDROID_ARCH_ABI})
        endforeach()

        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/libs
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/Binaries/$<CONFIG>/ ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}
        )
    endif()
endmacro()