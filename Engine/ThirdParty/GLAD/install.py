import zipfile, os, sys, requests, io, shutil, subprocess

NAME = "GLAD - OpenGL Loader"
URL = "https://github.com/Dav1dde/glad/archive/master.zip"

GLAD_SPECS = {
    "PROFILE" : "core",
    "OPENGL_VERSION" : 4.6,
    "GENERATOR" : "c",
}

# Download glad source
INSTALL_DIRECTORY = os.path.abspath(os.path.dirname(sys.argv[0]))
print(f"\nDownloading '{NAME}'...")
zip_download = requests.get(URL, stream=True)
zip_archive = zipfile.ZipFile(io.BytesIO(zip_download.content))
zip_archive.extractall(path=INSTALL_DIRECTORY)

# Generate glad files
print(f"Generating Files...")
os.chdir(f'{INSTALL_DIRECTORY}/glad-master')

proc = subprocess.run(f'py -3 -m glad --profile {GLAD_SPECS["PROFILE"]} \
--api gl={GLAD_SPECS["OPENGL_VERSION"]} --generator {GLAD_SPECS["GENERATOR"]} \
--spec gl --out-path {INSTALL_DIRECTORY}'.split(), stderr=subprocess.DEVNULL)

# Remove extracted zip files
os.chdir(INSTALL_DIRECTORY)
shutil.rmtree(f"{INSTALL_DIRECTORY}/glad-master")
print(f"Generation complete...\n")

# Content of the cmake file to be generated
with open(f"{INSTALL_DIRECTORY}/CMakeLists.txt", "w") as f:
    f.write( 
'''# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

set(GLAD_FOLDER "ZeronEngine/ThirdParty/GLAD")

add_library( glad "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")

target_include_directories(glad PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(glad PROPERTIES FOLDER ${GLAD_FOLDER})
'''
    )

