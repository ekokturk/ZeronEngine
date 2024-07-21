import tarfile, requests, zipfile, re, io, os, sys

CURRENT_DIR = os.path.abspath(os.path.dirname(sys.argv[0]))
PLATFORM = sys.argv[1]
SHADERC_URL = "https://storage.googleapis.com/shaderc"

def getLatestArchive(build: str, extension: str):
    buildResp = requests.get(f"{SHADERC_URL}/badges/{build}", stream=True)
    url = f"{re.search(f'url=(.*).{extension}', buildResp.content.decode()).group(1)}.{extension}"
    return requests.get(url, stream=True)

if PLATFORM == "Windows":
# Get Windows executable
    respZipWindows = getLatestArchive('build_link_windows_vs2019_release.html', 'zip')
    zipWindows = zipfile.ZipFile(io.BytesIO(respZipWindows.content))
    for info in zipWindows.infolist():
        if 'bin/glslc' in info.filename:
            info.filename = os.path.basename(info.filename)
            zipWindows.extract(info, path=f"{CURRENT_DIR}/bin/windows/")
elif PLATFORM == "Linux":
    # Get Linux executable

    # -- Latest --
    # respLinux = getLatestArchive('build_link_linux_clang_release.html', 'tgz')

    release = "artifacts/prod/graphics_shader_compiler/shaderc/linux/continuous_clang_release/420/20230710-074253/install.tgz"
    respLinux = requests.get(f"{SHADERC_URL}/{release}", stream=True)
    
    zipLinux = tarfile.open(fileobj=io.BytesIO(respLinux.content))
    for member in zipLinux.getmembers():
        if 'bin/glslc' in member.name:
            member.name = os.path.basename(member.name)
            zipLinux.extract(member, path=f"{CURRENT_DIR}/bin/linux/")
else:
    raise Exception("GLSLC dependencies require platform definition")