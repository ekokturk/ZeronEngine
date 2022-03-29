import sys, zipfile, requests, os, io, shutil
from pathlib import Path

def ExtractDirectory(fromDirPath, toDirPath, filter):
    fromDir = Path(fromDirPath)
    toDir = Path(toDirPath)
    if not fromDir.is_dir():
        print(f"Invalid directory '{fromDirPath}'!")
        return
    for item in filter:
        if not 'from' in item:
            continue
        itemFromPathStr = item['from']
        itemFromPath = fromDir / itemFromPathStr
        itemToPathStr = item['to'] if 'to' in item else ''
        itemExcludeList = item['exclude'] if 'exclude' in item else []
        if itemFromPath.is_file():
            itemToPath = Path(toDir / itemToPathStr)
            itemToPath.mkdir(parents=True, exist_ok=True)
            shutil.copy2(itemFromPath, itemToPath)
        elif itemFromPath.is_dir():
            if not itemToPathStr:
                itemToPathStr = itemFromPathStr
            shutil.copytree(itemFromPath, toDir / itemToPathStr, ignore=shutil.ignore_patterns(*itemExcludeList))
        else:
            print(f"Filter item is not a directory or a file: '{str(itemFromPath)}'!")
            continue

def ClearDirectory(dirPath, excludeList = {}):
    return

def UpdateFileContent(fileName, appendStr, modifyStrList = []):
    file = Path(fileName)
    if not file.exists():
        file.parent.mkdir(parents=True, exist_ok=True)
        file.write_text('')
    content = file.read_text()
    for item in modifyStrList:
        content = content.replace(item[0], item[1])
    file.write_text(f"{content}{appendStr}")

