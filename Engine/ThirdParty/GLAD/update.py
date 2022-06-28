import os, subprocess

GLAD_SPECS = {
    "PROFILE" : "core",
    "OPENGL_VERSION" : 4.6,
    "GENERATOR" : "c",
}

output_path = os.path.abspath("./")
os.chdir("./.submodule")
proc = subprocess.run(f'py -3 -m glad --profile {GLAD_SPECS["PROFILE"]} \
--api gl={GLAD_SPECS["OPENGL_VERSION"]} --generator {GLAD_SPECS["GENERATOR"]} \
--spec gl --out-path {output_path}'.split(), stderr=subprocess.DEVNULL)

