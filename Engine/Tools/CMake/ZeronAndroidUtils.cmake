macro(zeron_set_android_libs libList)
    # find_library(ANDROID_LOG_LIB log) # We add this for ZeronCore
    find_library(ANDROID_DL_LIB NAMES libdl dl)
    find_library(ANDROID_LIBRARY_LIB android)
    set(${libList} ${ANDROID_DL_LIB} ${ANDROID_LOG_LIB} ${ANDROID_LIBRARY_LIB})
endmacro()

macro(zeron_target_android_package target packageName appName)
    # Target
    if(NOT TARGET ${target})
        message(FATAL_ERROR "ZERON - ERROR: Invalid target for Android!")
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

    # SDL is required for Android
    get_property(ZERON_JAVA_DEPENDENCY_DIR GLOBAL PROPERTY SDL_JAVA_DIR)

    # Get template Android project directory
    get_property(ZERON_ANDROID_PROJECT_DIR GLOBAL PROPERTY ZERON_ANDROID_PROJECT_DIR)
    if(NOT DEFINED ZERON_ANDROID_PROJECT_DIR)
        message(FATAL_ERROR "ZERON - ERROR: Unable to locate template Zeron Android project directory!")
    endif()

    set(ZERON_ANDROID_PACKAGE_DIR               ${CMAKE_BINARY_DIR}/AndroidProject/)
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

    file(COPY ${ZERON_ANDROID_PROJECT_DIR}/app/proguard-rules.pro   DESTINATION ${ZERON_ANDROID_PACKAGE_DIR}/app)

    # Copy configured package files
    configure_file(${ZERON_ANDROID_PROJECT_DIR}/app/build.gradle 
                ${ZERON_ANDROID_PACKAGE_DIR}/app/build.gradle @ONLY)

    configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/AndroidManifest.xml 
                ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/AndroidManifest.xml @ONLY)

    configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/res/values/strings.xml 
                ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/res/values/strings.xml @ONLY)

    configure_file(${ZERON_ANDROID_PROJECT_MAIN_DIR}/java/com.zeronengine.application/ZeronActivity.java 
                ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/java/${ZERON_ANDROID_PACKAGE_NAME}/${ZERON_ANDROID_TARGET}Activity.java @ONLY)

    # Copy libraries
    file(GLOB_RECURSE ANDROID_STL_LIB ${ANDROID_STL_LIB_DIR}/lib${CMAKE_ANDROID_STL_TYPE}.*)
    file(COPY ${ANDROID_STL_LIB} DESTINATION ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/jniLibs/${CMAKE_ANDROID_ARCH_ABI})

    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/libs
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/Binaries/$<CONFIG>/ ${ZERON_ANDROID_PACKAGE_MAIN_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}
    )
endmacro()