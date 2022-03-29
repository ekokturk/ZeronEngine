import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'deps/'},
    {'from':'include/'},
    {'from':'src/'},
    {'from':'CMake/'},
    {'from':'CMakeLists.txt'}
]
).updateCMakeFile('''
set(GLFW_FOLDER "ZeronEngine/ThirdParty/GLFW")
set_target_properties(glfw PROPERTIES FOLDER ${GLFW_FOLDER})
''', [
    ('"Build shared libraries" OFF', '"Build shared libraries" ON'),
    ('"Build the GLFW example programs" ON', '"Build the GLFW example programs" OFF'),
    ('"Build the GLFW test programs" ON', '"Build the GLFW test programs" OFF'),
    ('option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)', 'option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)'),
    ('NOT TARGET uninstall', 'TARGET uninstall')
])
