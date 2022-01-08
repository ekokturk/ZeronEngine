import zipfile, os, sys, requests, io, shutil, subprocess

NAME = "GLAD - OpenGL Loader"
VERSION_MAJOR = 0
VERSION_MINOR = 1.34
URL = f"https://github.com/Dav1dde/glad/archive/v{VERSION_MAJOR}.{VERSION_MINOR}.zip"
ZIP_FOLDER = f"glad-{VERSION_MAJOR}.{VERSION_MINOR}"
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
os.chdir(f'{INSTALL_DIRECTORY}/{ZIP_FOLDER}')

proc = subprocess.run(f'py -3 -m glad --profile {GLAD_SPECS["PROFILE"]} \
--api gl={GLAD_SPECS["OPENGL_VERSION"]} --generator {GLAD_SPECS["GENERATOR"]} \
--spec gl --out-path {INSTALL_DIRECTORY}'.split(), stderr=subprocess.DEVNULL)

# Remove extracted zip files
os.chdir(INSTALL_DIRECTORY)
shutil.rmtree(f"{INSTALL_DIRECTORY}/{ZIP_FOLDER}")
print(f"Generation complete...\n")

# Content of the cmake file to be generated
with open(f"{INSTALL_DIRECTORY}/CMakeLists.txt", "w") as f:
    f.write( 
'''# Copyright (C) Eser Kokturk. All Rights Reserved.

set(GLAD_FOLDER "ZeronEngine/ThirdParty/GLAD")

add_library( glad SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")

set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)

set_target_properties(glad PROPERTIES FOLDER ${GLAD_FOLDER})'''
)

