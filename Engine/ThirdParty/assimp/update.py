import sys, pathlib
sys.path.append(str(pathlib.Path(__file__).absolute().parent.parent.parent / "Tools" / "Python"))
from ZeronSubmodule import ZeronSubmodule

ZeronSubmodule().extract([     
    {'from':'code/'},
    {'from':'include/'},
    {'from':'cmake-modules/'},
    {'from':'contrib/'},
    {'from':'assimp.pc.in'},
    {'from':'revision.h.in'},
    {'from':'CMakeLists.txt'}
]
).updateCMakeFile('''
set(ASSIMP_FOLDER "ZeronEngine/ThirdParty/assimp")
set_target_properties(assimp PROPERTIES FOLDER ${ASSIMP_FOLDER})
if(TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER ${ASSIMP_FOLDER})
endif()
''', [
    ('IF ( ASSIMP_BUILD_ASSIMP_TOOLS )', 'IF ( FALSE )'),
    ('IF ( ASSIMP_BUILD_TESTS )','IF ( FALSE )'),
    ('IF ( ASSIMP_INSTALL )', 'IF ( FALSE )'),
    ('NOT TARGET uninstall AND ASSIMP_INSTALL', 'FALSE'),
    ('MSVC12 OR MSVC14 OR MSVC15', 'FALSE')
])