# Copyright (C) Eser Kokturk. All Rights Reserved.

# fmt - Formatting Library

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
set(JUST_INSTALL_CEREAL ON)
add_subdirectory(.submodule)
set_target_properties(cereal PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/cereal")