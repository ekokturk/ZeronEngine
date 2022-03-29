import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'stb_image.h', 'to':'include/stb/'},
    {'from':'stb_image_write.h', 'to':'include/stb/'},
    {'from':'stb_easy_font.h', 'to':'include/stb/'},
    {'from':'stb_image_resize.h', 'to':'include/stb/'},
]).updateCMakeFile('''
add_library(stb-header-only INTERFACE)
target_include_directories(stb-header-only INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
''')
