# Copyright (C) Eser Kokturk. All Rights Reserved.

include("${ZERON_CMAKE_UTILS}/ZeronProjectUtils.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronAndroidUtils.cmake")

file(GLOB_RECURSE test_files LIST_DIRECTORIES false *.h *.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}	PREFIX "Source Files/" FILES ${test_files})
zeron_set_output_directory("Tests/ZeronTestApp")

zeron_create_project(ZeronTestApp "${test_files}")

zeron_target_add_module_test(ZeronTestApp ZeronCore)
zeron_target_add_module_test(ZeronTestApp ZeronPlatform)
zeron_target_add_module_test(ZeronTestApp ZeronScript)
zeron_target_add_module_test(ZeronTestApp ZeronPhysics)
zeron_target_add_module_test(ZeronTestApp ZeronDB)
zeron_target_add_module_test(ZeronTestApp ZeronNet)

set_target_properties(ZeronTestApp PROPERTIES FOLDER "ZeronEngine/Tests")
target_link_libraries(ZeronTestApp PUBLIC ZeronTestFramework ZeronCore ZeronPlatform)
zeron_copy_runtime_dependencies(ZeronTestApp)

zeron_android_configure_package(ZeronTestApp "com.zeron.testApp" ZeronTestApp "")

# Install
zeron_install_executable(
    ZeronTestApp 
    "${CMAKE_INSTALL_PREFIX}/ZeronTestApp" 
    ""
)
