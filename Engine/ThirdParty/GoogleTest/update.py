import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'googlemock/'},
    {'from':'googletest/'},
    {'from':'CMakeLists.txt'}
]
).updateCMakeFile('''
set_target_properties( gmock PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gmock_main PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gtest PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
set_target_properties( gtest_main PROPERTIES FOLDER "ZeronEngine/ThirdParty/GoogleTest")
''')
