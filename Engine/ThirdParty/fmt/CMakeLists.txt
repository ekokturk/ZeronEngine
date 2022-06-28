# Copyright (C) Eser Kokturk. All Rights Reserved.

# fmt - Formatting Library

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
add_subdirectory(.submodule)
set_target_properties(fmt PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/fmt")