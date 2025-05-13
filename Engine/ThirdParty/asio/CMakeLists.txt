# Copyright (C) Eser Kokturk. All Rights Reserved.

# Asio - Networking library

set(ASIO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/asio/include)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${ASIO_SOURCE_DIR})
