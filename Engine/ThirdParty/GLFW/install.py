import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "GLFW"
VERSION_MAJOR = 3
VERSION_MINOR = 3
URL = f"https://github.com/glfw/glfw/archive/{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"glfw-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/include/", 
        "TARGET_PATH":"include/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/deps/", 
        "TARGET_PATH":"deps/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/src/", 
        "TARGET_PATH":"src/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMake/", 
        "TARGET_PATH":"CMake/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/LICENSE.md", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMakeLists.txt", 
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
set(GLFW_FOLDER "ZeronEngine/ThirdParty/GLFW")
set_target_properties(glfw PROPERTIES FOLDER ${GLFW_FOLDER})
'''
    )

# Disable GLFW examples, tests and documentation
with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = edited_content.replace('"Build shared libraries" OFF', 
                                            '"Build shared libraries" ON')
    edited_content = edited_content.replace('"Build the GLFW example programs" ON', 
                                            '"Build the GLFW example programs" OFF')
    edited_content = edited_content.replace('"Build the GLFW test programs" ON', 
                                            '"Build the GLFW test programs" OFF')
    edited_content = edited_content.replace('option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)', 
                                            'option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)')
    edited_content = edited_content.replace('NOT TARGET uninstall', 
                                            'TARGET uninstall')

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)