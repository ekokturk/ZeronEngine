# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# =========== VARIABLES ===========
init_output_directory("")

# =========== TARGET ===========

set(SANDBOX_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE SANDBOX_SOURCE_FILES	"${SANDBOX_SOURCE_DIR}/*.cpp"	"${SANDBOX_SOURCE_DIR}/*.h")
source_group(TREE "${SANDBOX_SOURCE_DIR}"	PREFIX "Source Files/" FILES ${SANDBOX_SOURCE_FILES})
add_executable(
    Sandbox 
    ${SANDBOX_SOURCE_FILES}
)

target_link_libraries(Sandbox ZeronClient)
#target_precompile_headers(Sandbox REUSE_FROM ZeronEngine)

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>)