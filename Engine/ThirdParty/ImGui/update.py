import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'backends/', 'to':'imgui/'},
    {'from':'imconfig.h', 'to':'imgui/'},
    {'from':'imgui.h', 'to':'imgui/'},
    {'from':'imgui_internal.h', 'to':'imgui/'},
    {'from':'imstb_rectpack.h', 'to':'imgui/'},
    {'from':'imstb_textedit.h', 'to':'imgui/'},
    {'from':'imstb_truetype.h', 'to':'imgui/'},
    {'from':'imgui.cpp', 'to':'imgui/'},
    {'from':'imgui_draw.cpp', 'to':'imgui/'},
    {'from':'imgui_tables.cpp', 'to':'imgui/'},
    {'from':'imgui_widgets.cpp', 'to':'imgui/'},
]
).updateCMakeFile('''
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
    find_package(OpenGL REQUIRED)
    list(APPEND MODULE_LIBS_PRIVATE glad ${OPENGL_gl_LIBRARY})
endif()

if(WIN32)
    list(APPEND IMGUI_SOURCE_FILES 
        "${IMGUI_SOURCE_DIR}/imgui_impl_dx11.h"
        "${IMGUI_SOURCE_DIR}/imgui_impl_dx11.cpp"
    )
	list(APPEND IMGUI_LIBS dxgi d3d11)
endif()

add_library(imgui ${IMGUI_SOURCE_FILES})
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
''')
