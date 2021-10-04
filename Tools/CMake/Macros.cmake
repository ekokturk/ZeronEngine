# Set output directory of the binary files to designated folders
macro(init_output_directory path)
    if(NOT DEFINED ${OUTPUT_DIRECTORY})
        set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries")
    endif()
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/Debug/${path})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/Debug/${path})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/Debug/${path})
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release/${path})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release/${path})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release/${path})

endmacro()

macro(add_subdirectories path)
    file(GLOB dirs ${path}/*)
    foreach(dir ${dirs})
        if(EXISTS "${dir}/CMakeLists.txt")
            add_subdirectory(${dir})
        endif()
    endforeach()
endmacro()

macro(add_subdirectory_no_symbols)
    if(WIN32) # Disable symbol exports for these libraries
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
    endif()
    add_subdirectory(${ARGV})
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
endmacro()

macro(add_target )
    file(GLOB_RECURSE module_files	"${module_dir}/*.cpp"	"${module_dir}/*.h")
    source_group(TREE "${module_dir}"	PREFIX "Source Files/" FILES ${module_files})
    add_library(${module_name} SHARED ${module_files})
endmacro()

macro(add_zeron_module module_name module_dir)
    file(GLOB_RECURSE module_files	"${module_dir}/*.cpp"	"${module_dir}/*.h")
    source_group(TREE "${module_dir}"	PREFIX "Source Files/" FILES ${module_files})
    add_library(${module_name} SHARED ${module_files})
    set_target_properties(${module_name} PROPERTIES FOLDER "ZeronEngine")
    set_property(TARGET ${module_name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${module_name}>)
endmacro()