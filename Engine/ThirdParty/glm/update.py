import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'glm/', 'to':'include/glm/'},
]
).updateCMakeFile('''
add_library(glm-header-only INTERFACE)
target_include_directories(glm-header-only INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
''')
