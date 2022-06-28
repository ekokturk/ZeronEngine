# Copyright (C) Eser Kokturk. All Rights Reserved.

# glad - OpenGL Loader

file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/include)

execute_process(COMMAND python update.py WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_library( glad ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)

set_target_properties(glad PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/GLAD")