# Copyright (C) Eser Kokturk. All Rights Reserved.

file(GLOB_RECURSE SANDBOX_LINUX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(SANDBOX_SOURCES ${SANDBOX_LINUX_SOURCES} ${SANDBOX_SHARED_SOURCES})
source_group(TREE "${SANDBOX_DIR}" PREFIX "Source Files/" FILES ${SANDBOX_SOURCES})

zeron_set_output_directory("Sandbox")
add_executable(Sandbox ${SANDBOX_SOURCES}  )
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SANDBOX_SHARED_DIR})
target_link_libraries(Sandbox PUBLIC ZeronRenderer)