import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "GLFW"
URL = "https://github.com/glfw/glfw/archive/master.zip"
DIRECTORIES = [
    {
        "ZIP_PATH":"glfw-master/include/", 
        "TARGET_PATH":"include/",
    },
    {
        "ZIP_PATH":"glfw-master/deps/", 
        "TARGET_PATH":"deps/",
    },
    {
        "ZIP_PATH":"glfw-master/src/", 
        "TARGET_PATH":"src/",
    },
    {
        "ZIP_PATH":"glfw-master/CMake/", 
        "TARGET_PATH":"CMake/",
    },
    {
        "ZIP_PATH":"glfw-master/LICENSE.md", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":"glfw-master/CMakeLists.txt", 
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
    edited_content = edited_content.replace('"Build the GLFW example programs" ${GLFW_STANDALONE}', 
                                            '"Build the GLFW example programs" OFF')
    edited_content = edited_content.replace('"Build the GLFW test programs" ${GLFW_STANDALONE}', 
                                            '"Build the GLFW test programs" OFF')
    edited_content = edited_content.replace('option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)', 
                                            'option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)')
    edited_content = edited_content.replace('NOT TARGET uninstall', 
                                            'TARGET uninstall')

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)