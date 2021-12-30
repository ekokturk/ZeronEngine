import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "ImGui - Graphical User Interface Library"
VERSION_MAJOR = 1
VERSION_MINOR = 86
URL = f"https://github.com/ocornut/imgui/archive/refs/tags/v{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"imgui-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/backends/", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imconfig.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui_internal.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imstb_rectpack.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imstb_textedit.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imstb_truetype.h", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui.cpp", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui_draw.cpp", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui_tables.cpp", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/imgui_widgets.cpp", 
        "TARGET_PATH":"imgui/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/LICENSE.txt", 
        "TARGET_PATH":"/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)

# ================== GENERATE CMAKE ================================

CMAKE_FILE_PATH = f"{os.path.abspath(os.path.dirname(sys.argv[0]))}/CMakeLists.txt"

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(
'''
# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

set(IMGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_FOLDER "ZeronEngine/ThirdParty/ImGui")
set(IMGUI_SOURCE_FILES 
    "${IMGUI_SOURCE_DIR}/imgui.h"
    "${IMGUI_SOURCE_DIR}/imgui.cpp"
    "${IMGUI_SOURCE_DIR}/imconfig.h"
    "${IMGUI_SOURCE_DIR}/imgui_internal.h"
    "${IMGUI_SOURCE_DIR}/imstb_rectpack.h"
    "${IMGUI_SOURCE_DIR}/imstb_textedit.h"
    "${IMGUI_SOURCE_DIR}/imstb_truetype.h"
    "${IMGUI_SOURCE_DIR}/imgui_draw.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_tables.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_widgets.cpp"
)

if(TARGET glad)
    list(APPEND IMGUI_SOURCE_FILES 
        "${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.h"
        "${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.cpp"
        "${IMGUI_SOURCE_DIR}/imgui_impl_opengl3_loader.h"
    )
	list(APPEND IMGUI_LIBS glad opengl32)
endif()

if(WIN32)
    list(APPEND IMGUI_SOURCE_FILES 
        "${IMGUI_SOURCE_DIR}/imgui_impl_dx11.h"
        "${IMGUI_SOURCE_DIR}/imgui_impl_dx11.cpp"
    )
	list(APPEND IMGUI_LIBS dxgi d3d11)
endif()

add_library(imgui SHARED ${IMGUI_SOURCE_FILES})
set_target_properties(imgui PROPERTIES FOLDER ${IMGUI_FOLDER})

target_link_libraries(imgui 
    PRIVATE
        ${IMGUI_LIBS}
)
target_include_directories(imgui 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"	
    PRIVATE 
        "${IMGUI_SOURCE_DIR}"		
)
'''
    )