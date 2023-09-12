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

        set(ZERON_ANDROID_PACKAGE_DIR               ${CMAKE_BINARY_DIR}/AndroidProject)
        set(ZERON_ANDROID_PACKAGE_MAIN_DIR          ${ZERON_ANDROID_PACKAGE_DIR}/app/src/main)

        set(ZERON_ANDROID_PROJECT_MAIN_DIR          ${ZERON_ANDROID_PROJECT_DIR}/app/src/main/)
        set(ZERON_ANDROID_PROJECT_RESOURCE_DIR      ${ZERON_ANDROID_PROJECT_MAIN_DIR}/res)

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

        # Copy libraries
        get_property(EXTRA_ANDROID_LIBS GLOBAL PROPERTY ZERON_ANDROID_EXTRA_LIBS)
        file(GLOB_RECURSE ANDROID_STL_LIB ${ANDROID_STL_LIB_DIR}/lib${CMAKE_ANDROID_STL_TYPE}.*)
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