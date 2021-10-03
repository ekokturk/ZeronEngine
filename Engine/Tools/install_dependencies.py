import os, sys, glob, subprocess

script_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
dependency_dir = os.path.join(script_dir, '../ThirdParty/')

os.chdir(dependency_dir)

print("\n================== UPDATING ENGINE DEPENDENCIES ==================\n")

thirdparty_dirs = glob.glob(os.path.realpath(f"{dependency_dir}/*/"))

dependency_count = 0
success_count = 0

for d in thirdparty_dirs:
    script_path = os.path.join(d,"install.py")
    if os.path.exists(script_path):
       dependency_count += 1
       print("---------------------------------------------------------------------")
       error = subprocess.call(f'py -3 {script_path}')
       if error == 0:
           success_count +=1

print("========================================================================")
print(f"Engine dependencies updated. Success: {success_count}/{dependency_count}")
print("========================================================================")
