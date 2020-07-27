import sys, os
sys.path.insert(0, os.path.abspath(os.path.dirname(sys.argv[0])+"/../../Tools/Utils"))
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
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)
