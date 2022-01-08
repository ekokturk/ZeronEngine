import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "glm"
VERSION_MAJOR = 0
VERSION_MINOR = 9.8
URL = f"https://github.com/g-truc/glm/archive/{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"glm-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/glm/", 
        "TARGET_PATH":"include/glm/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/copying.txt", 
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
# Copyright (C) Eser Kokturk. All Rights Reserved.

add_library(glm-header-only INTERFACE)
target_include_directories(glm-header-only INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
'''
)
