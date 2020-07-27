import sys, zipfile, requests, os, io, shutil

# NAME: name of the library
# URL: link of the library 
# DIRECTORIES: Directories/files to be searched for extraction
    # ZIP_PATH: Path of the file/directory in the zip
    # TARGET_PATH: Relative directory (caller file) to extract files to
    # EXCLUDES: excluded directories or files 
    
# ex.
'''
    NAME = "Project/Library Name"
    URL = "url.zip"
    DIRECTORIES = [
        {
            "ZIP_PATH":"zip-file/files/file.h", 
            "TARGET_PATH":"includes/",
            "EXCLUDES":{},
        },
        {
            "ZIP_PATH":"zip-file/files/", 
            "TARGET_PATH":"includes/",
            "EXCLUDES":{"other-file.h"},
        }
    ]
'''

def download_zip(name, url):
    print(f"\nDownloading '{name}'...")
    zip_url = url
    zip_download = requests.get(zip_url, stream=True)
    zip_archive = zipfile.ZipFile(io.BytesIO(zip_download.content))
    return zip_archive


def extract_from_zip(zipfile, filter):
    print(f"Extracting files...")
    # use relative path for extraction
    path = os.path.abspath(os.path.dirname(sys.argv[0]))
    # check files in the zip
    for file in zipfile.namelist():
        # check specified filter directories
        for dir in filter:
            zip_path    = dir["ZIP_PATH"]
            target_path = dir["TARGET_PATH"]
            excludes = {}
            if "EXCLUDES" in dir:
                excludes = dir["EXCLUDES"]
            if file.startswith(zip_path):
                filename = os.path.basename(file)   # file name in zip 
                dirname = os.path.dirname(file)+"/"     # path name of the file
                # check file name is valid (not exluded or directory)
                if not filename or filename in excludes:
                    continue
                source = zipfile.open(file)
                # create target path with relative zip path of the file (exlude searched directory)
                target_dir = f"{path}/{target_path}" 
                if filename in zip_path:
                    target_dir += zip_path.replace(dirname + filename,"")
                else:
                    target_dir += dirname.replace(zip_path,"")
                os.makedirs(os.path.dirname(target_dir+"/"), exist_ok=True)
                target = open(os.path.join(target_dir, filename), "wb")
                with source, target:
                    shutil.copyfileobj(source, target)
    print(f"Extraction complete...\n")