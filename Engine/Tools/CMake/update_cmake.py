import os, re

def get_files_as_string(path, fileExtensions, pathPrefix =""):
    file_paths = ""
    for root, dirs, files in os.walk(path):
        for name in files:
            if name.endswith( fileExtensions ):
                rel_dir = os.path.relpath(root, path)
                rel_file = os.path.join(rel_dir, name).replace("\\","/").replace("./","")
                file_paths += f'\n\t\"{pathPrefix}/{rel_file}\"'
    return file_paths


def get_directories_as_string(path, dirName, pathPrefix =""):
    dir_paths = ""
    for root, dirs, files in os.walk(path):
        for name in dirs:
            if name == dirName:
                rel_dir = os.path.relpath(root, path)
                return_dir = os.path.join(rel_dir, name).replace("\\","/").replace("./","")
                dir_paths += f'\n\t\"{pathPrefix}/{return_dir}\"'
    return dir_paths

def update_cmake(filePath, updateString, beginPattern, endPattern):

    cmake_file = open(filePath).read()
    edited_file = re.sub(f'{beginPattern}.*{endPattern}',f"{beginPattern}{updateString}\n\t{endPattern}",cmake_file, flags=re.DOTALL)

    with open(filePath, "w") as file:
        file.write(edited_file)
        

engine_rel_path = './../../'
source_path = os.path.dirname(os.path.realpath(__file__)) + engine_rel_path + 'Source'
source_files = get_files_as_string(source_path, (".cpp", ".h"), "${ENGINE_SOURCE_DIR}")
update_cmake( engine_rel_path + 'CMakeLists.txt',source_files, "#ENGINE_SOURCE_BEGIN", "#ENGINE_SOURCE_END")


