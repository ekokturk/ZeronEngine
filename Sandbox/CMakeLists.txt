# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# =========== VARIABLES ===========
set(SANDBOX_FOLDER "ZeronEngine")
set_output_directory("")

# =========== TARGET ===========

set(
    SANDBOX_SOURCE_FILES
    "Source/Sandbox.h"
    "Source/Sandbox.cpp"
)

add_executable(
    Sandbox 
    ${SANDBOX_SOURCE_FILES}
)

set_target_properties(
    Sandbox 
    PROPERTIES 
        FOLDER ${SANDBOX_FOLDER}
)

target_link_libraries(Sandbox ZeronEngine)

source_group( TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SANDBOX_SOURCE_FILES})
