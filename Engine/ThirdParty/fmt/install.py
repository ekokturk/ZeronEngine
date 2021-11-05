import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "FMT - Formatting Library"
VERSION_MAJOR = 8
VERSION_MINOR = 0.1
URL = f"https://github.com/fmtlib/fmt/archive/{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"fmt-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/include/", 
        "TARGET_PATH":"include/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/LICENSE.rst", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMakeLists.txt", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/support/", 
        "TARGET_PATH":"support/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/src/", 
        "TARGET_PATH":"src/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)

# ================== GENERATE CMAKE ================================

CMAKE_FILE_PATH = f"{os.path.abspath(os.path.dirname(sys.argv[0]))}/CMakeLists.txt"

append_content = '''
# START - Generated for Zeron Engine Development
set_target_properties(fmt PROPERTIES FOLDER "ZeronEngine/ThirdParty/fmt")
# END - Generated for Zeron Engine Development
'''

with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = edited_content + append_content
    edited_content = edited_content.replace('README.rst ChangeLog.rst', '')

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)