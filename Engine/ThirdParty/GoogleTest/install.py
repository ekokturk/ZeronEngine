import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.abspath(sys.argv[0]))+"/../../Tools/Utils"))
from zip_helpers import *

NAME = "GoogleTest - Testing and Mocking Framework - GitHub"
VERSION_MAJOR = 1
VERSION_MINOR = 11.0
URL = f"https://github.com/google/googletest/archive/refs/tags/release-{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"googletest-release-{VERSION_MAJOR}.{VERSION_MINOR}"
DIRECTORIES = [
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/googlemock/", 
        "TARGET_PATH":"googlemock/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/googletest", 
        "TARGET_PATH":"googletest/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CMakeLists.txt", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CONTRIBUTING.md", 
        "TARGET_PATH":"/",
    },
    {
        "ZIP_PATH":f"{ZIP_FOLDER}/CONTRIBUTORS", 
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

append_content = '''
# START - Generated for Zeron Engine Development
set_target_properties( gmock PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gmock_main PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gtest PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gtest_main PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
# END - Generated for Zeron Engine Development
'''

with open(CMAKE_FILE_PATH) as f:
    edited_content = f.read()
    edited_content = edited_content + append_content

with open(CMAKE_FILE_PATH, "w") as f:
    f.write(edited_content)