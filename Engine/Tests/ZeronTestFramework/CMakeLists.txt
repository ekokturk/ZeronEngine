# Copyright (C) Eser Kokturk. All Rights Reserved.
    
file(GLOB_RECURSE files	"*.cpp"	"*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files/" FILES ${files})
add_library(ZeronTestFramework SHARED ${files})
set_target_properties(ZeronTestFramework PROPERTIES FOLDER "ZeronEngine/Tests")

target_include_directories(
    ZeronTestFramework 
        PUBLIC 
            "${CMAKE_CURRENT_SOURCE_DIR}"
)


target_link_libraries(
    ZeronTestFramework 
        PUBLIC 
            gtest 
)

set_property(
    TARGET ZeronTestFramework 
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:ZeronTestFramework>
)
