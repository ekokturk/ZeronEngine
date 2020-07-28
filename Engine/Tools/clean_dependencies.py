import os, sys, shutil, glob

script_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
dependency_dir = os.path.join(script_dir, '../ThirdParty/*/*')

fileList = glob.glob(dependency_dir, recursive=True)


print("\n======================================================\n")
print("Cleaning up ZERON ENGINE third party dependencies...")
for f in fileList:
    #
    if f.endswith("install.py"):
        continue
    if os.path.isdir(f):
        shutil.rmtree(f)
    elif os.path.isfile(f):
        os.remove(f)

print("Cleaning up is complete...")
print("\n======================================================\n")
    
