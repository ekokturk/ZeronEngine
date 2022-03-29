import sys
from pathlib import Path
from ZeronSubmodule import ZeronSubmodule

dependencyPath = Path(sys.argv[0]).absolute().parent.parent.parent / 'ThirdParty'
print("\n======================================================\n")
print("Cleaning up ZERON ENGINE third party dependencies...")
for p in dependencyPath.iterdir():
    ZeronSubmodule(p, False).clear()
print("Cleaning up is complete...")
print("\n======================================================\n")
    
