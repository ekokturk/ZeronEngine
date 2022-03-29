import sys, subprocess
from pathlib import Path
from ZeronSubmodule import ZeronSubmodule

dependencyPath = Path(sys.argv[0]).absolute().parent.parent.parent / 'ThirdParty'
print("\n================== UPDATING ENGINE DEPENDENCIES ==================\n")
successCount = 0
dependencyCount = 0
for p in dependencyPath.iterdir():
    updateScriptPath = p / ZeronSubmodule.UPDATE_SCRIPT
    if not updateScriptPath.exists():
        continue
    dependencyCount += 1
    print("---------------------------------------------------------------------")
    error = subprocess.call(f'py -3 {str(updateScriptPath)}')
    if error == 0:
        successCount +=1
print("========================================================================")
print(f"Engine dependencies updated. Success: {successCount}/{dependencyCount}")
print("========================================================================")
