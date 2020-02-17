# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# =========== VARIABLES ===========
set(SANDBOX_FOLDER "ZeronEngine")
set_output_directory("")

# =========== TARGET ===========

add_executable(
    Sandbox 
    "Source/main.cpp"
)


set_target_properties(
    Sandbox 
    PROPERTIES 
        FOLDER ${SANDBOX_FOLDER}
)

target_link_libraries(Sandbox ZeronEngine)
