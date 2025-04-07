# Copyright (C) Eser Kokturk. All Rights Reserved.

# lmdb - Lightning Memory-Mapped Database Library

set(LMDB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/libraries/liblmdb)

add_library(lmdb STATIC
    ${LMDB_SOURCE_DIR}/lmdb.h
    ${LMDB_SOURCE_DIR}/mdb.c
    ${LMDB_SOURCE_DIR}/midl.c
    ${LMDB_SOURCE_DIR}/midl.h
)
target_include_directories(lmdb INTERFACE ${LMDB_SOURCE_DIR})
set_target_properties(lmdb PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/lmdb")

