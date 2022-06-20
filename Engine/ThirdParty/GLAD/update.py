import os, sys, requests, io, shutil, subprocess
import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

GLAD_SPECS = {
    "PROFILE" : "core",
    "OPENGL_VERSION" : 4.6,
    "GENERATOR" : "c",
}
glad = ZeronSubmodule()
glad.clear()

os.chdir(glad.getSubmoduleDir())
proc = subprocess.run(f'py -3 -m glad --profile {GLAD_SPECS["PROFILE"]} \
--api gl={GLAD_SPECS["OPENGL_VERSION"]} --generator {GLAD_SPECS["GENERATOR"]} \
--spec gl --out-path {glad.path}'.split(), stderr=subprocess.DEVNULL)
os.chdir(glad.path)

glad.updateCMakeFile('''
set(GLAD_FOLDER "ZeronEngine/ThirdParty/GLAD")
add_library( glad "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
set_target_properties(glad PROPERTIES FOLDER ${GLAD_FOLDER})
'''
)

