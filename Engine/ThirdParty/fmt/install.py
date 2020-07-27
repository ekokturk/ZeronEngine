import sys, os
print(os.path.abspath(sys.argv[0]))
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
        "ZIP_PATH":"fmt-master/support/", 
        "TARGET_PATH":"support/",
    },
    {
        "ZIP_PATH":"fmt-master/src/", 
        "TARGET_PATH":"src/",
    }
]

extract_from_zip(download_zip(NAME, URL), DIRECTORIES)
