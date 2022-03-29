import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'cmake/'},
    {'from':'include/'},
    {'from':'src/'},
    {'from':'sdl2-config.in'},
    {'from':'sdl2-config.cmake.in'},
    {'from':'sdl2-config-version.cmake.in'},
    {'from':'cmake_uninstall.cmake.in'},
    {'from':'SDL2Config.cmake'},
    {'from':'CMakeLists.txt'}
]
).updateCMakeFile('''
set(SDL2_FOLDER "ZeronEngine/ThirdParty/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER ${SDL2_FOLDER})
set_target_properties(SDL2 PROPERTIES FOLDER ${SDL2_FOLDER})
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
''', [
    ('project(SDL2 C CXX)', '''project(SDL2 C CXX)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
endif()
'''),
    ('APPEND EXTRA_LIBS user32', 'APPEND EXTRA_LIBS vcruntime user32'),
    ('''add_custom_target(uninstall
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)''', '')
])
