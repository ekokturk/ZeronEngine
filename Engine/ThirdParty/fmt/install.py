import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "FMT - Formatting Library"
URL = "https://github.com/fmtlib/fmt/archive/master.zip"
DIRECTORIES = [
    {
        "ZIP_PATH":"fmt-master/include/", 
        "TARGET_PATH":"include/",
    },
    {
        "ZIP_PATH":"fmt-master/LICENSE.rst", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":"fmt-master/CMakeLists.txt", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":"fmt-master/support/", 
        "TARGET_PATH":"support/",
    },
    {
        "ZIP_PATH":"fmt-master/src/", 
        "TARGET_PATH":"src/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)

# ================== GENERATE CMAKE ================================

CMAKE_FILE_PATH = f"{os.path.abspath(os.path.dirname(sys.argv[0]))}/CMakeLists.txt"

# Disable GLFW examples, tests and documentation
with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = edited_content.replace('README.rst ChangeLog.rst', '')

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)