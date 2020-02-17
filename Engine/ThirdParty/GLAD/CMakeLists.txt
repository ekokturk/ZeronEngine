# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

set(GLAD_FOLDER "ZeronEngine/ThirdParty/GLAD")

add_library(
    glad
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"
)

target_include_directories(
    glad
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(glad PROPERTIES FOLDER ${GLAD_FOLDER})
