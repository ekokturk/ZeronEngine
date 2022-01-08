import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "STB - Image Loading Library"
URL = f"https://github.com/nothings/stb/archive/refs/heads/master.zip"
ZIP_FOLDER = f"stb-master"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/LICENSE", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/stb_image.h", 
        "TARGET_PATH":"include/stb/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/stb_image_write.h", 
        "TARGET_PATH":"include/stb/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/stb_easy_font.h", 
        "TARGET_PATH":"include/stb/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/stb_image_resize.h", 
        "TARGET_PATH":"include/stb/",
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
add_library(stb-header-only INTERFACE)
target_include_directories(stb-header-only INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
'''
)
