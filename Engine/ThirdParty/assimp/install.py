import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "assimp - Asset Import Library"
VERSION_MAJOR = 5
VERSION_MINOR = 1.4
URL = f"https://github.com/assimp/assimp/archive/refs/tags/v{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"assimp-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/code/", 
        "TARGET_PATH":"code/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/include/", 
        "TARGET_PATH":"include/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/cmake-modules/", 
        "TARGET_PATH":"cmake-modules/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/assimp.pc.in", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/revision.h.in", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMakeLists.txt", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMakeLists.txt", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/LICENSE", 
        "TARGET_PATH":"/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)

# ================== GENERATE CMAKE ================================

CMAKE_FILE_PATH = f"{os.path.abspath(os.path.dirname(sys.argv[0]))}/CMakeLists.txt"

with open(CMAKE_FILE_PATH, "a") as f:
    f.write(
'''
# START - Generated for Zeron Engine Development
set(ASSIMP_FOLDER "ZeronEngine/ThirdParty/assimp")
set_target_properties(assimp PROPERTIES FOLDER ${ASSIMP_FOLDER})
set_target_properties(zlibstatic PROPERTIES FOLDER ${ASSIMP_FOLDER})
# END - Generated for Zeron Engine Development
'''
)

with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = edited_content.replace('IF ( ASSIMP_BUILD_ASSIMP_TOOLS )', 
                                            'IF ( FALSE )')
    edited_content = edited_content.replace('IF ( ASSIMP_BUILD_TESTS )', 
                                            'IF ( FALSE )')
    edited_content = edited_content.replace('IF ( ASSIMP_INSTALL )', 
                                            'IF ( FALSE )')
    edited_content = edited_content.replace('NOT TARGET uninstall AND ASSIMP_INSTALL', 
                                            'FALSE')
    edited_content = edited_content.replace('MSVC12 OR MSVC14 OR MSVC15', 
                                            'FALSE')
with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)