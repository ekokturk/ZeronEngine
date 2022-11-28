import tarfile, requests, zipfile, re, io, os, sys

CURRENT_DIR = os.path.abspath(os.path.dirname(sys.argv[0]))

SHADERC_URL = "https://storage.googleapis.com/shaderc/badges"

# Get Windows executable
respWindows = requests.get(f"{SHADERC_URL}/build_link_windows_vs2017_release.html", stream=True)
urlWindows = f"{re.search('url=(.*).zip', respWindows.content.decode()).group(1)}.zip"
respZipWindows = requests.get(urlWindows, stream=True)
zipWindows = zipfile.ZipFile(io.BytesIO(respZipWindows.content))
for info in zipWindows.infolist():
    if 'bin/glslc' in info.filename:
        info.filename = os.path.basename(info.filename)
        zipWindows.extract(info, path=f"{CURRENT_DIR}/bin/windows/")

# Get Linux executable
respLinux = requests.get(f"{SHADERC_URL}/build_link_linux_clang_release.html", stream=True)
urlLinux = f"{re.search('url=(.*).tgz', respLinux.content.decode()).group(1)}.tgz"
respZipLinux = requests.get(urlLinux, stream=True)
zipLinux = tarfile.open(fileobj=io.BytesIO(respZipLinux.content))
for member in zipLinux.getmembers():
    if 'bin/glslc' in member.name:
        member.name = os.path.basename(member.name)
        zipLinux.extract(member, path=f"{CURRENT_DIR}/bin/linux/")