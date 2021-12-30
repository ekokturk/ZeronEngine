import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "SDL2"
VERSION_MAJOR = 2
VERSION_MINOR = 0.18
URL = f"https://github.com/libsdl-org/SDL/archive/refs/tags/release-{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"SDL-release-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/", 
        "TARGET_PATH":"/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)

# ================== GENERATE CMAKE ================================

CMAKE_FILE_PATH = f"{os.path.abspath(os.path.dirname(sys.argv[0]))}/CMakeLists.txt"

# Add project folders for organization purposes
with open(CMAKE_FILE_PATH, "a") as f:
    f.write(
'''
# START - Generated for Zeron Engine Development
set(SDL2_FOLDER "ZeronEngine/ThirdParty/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER ${SDL2_FOLDER})
set_target_properties(SDL2 PROPERTIES FOLDER ${SDL2_FOLDER})
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
# END - Generated for Zeron Engine Development
'''
)

prepend_content = '''# START - Generated for Zeron Engine Development
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
endif()
# END - Generated for Zeron Engine Development

'''

with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = prepend_content + edited_content
    edited_content = edited_content.replace('''add_custom_target(uninstall
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)''', '')
    edited_content = edited_content.replace('APPEND EXTRA_LIBS user32', 'APPEND EXTRA_LIBS vcruntime user32')

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)
