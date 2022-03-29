import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'support/'},
    {'from':'include/'},
    {'from':'src/'},
    {'from':'CMakeLists.txt'}
]
).updateCMakeFile('''
set_target_properties(fmt PROPERTIES FOLDER "ZeronEngine/ThirdParty/fmt")
''', [
    ('README.rst ChangeLog.rst', '')
])