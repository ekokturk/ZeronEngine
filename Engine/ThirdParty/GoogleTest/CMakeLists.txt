# Copyright (C) Eser Kokturk. All Rights Reserved.

# GoogleTest - Testing and Mocking Framework

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
add_subdirectory(.submodule)
set_target_properties( gmock        PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/GoogleTest")
set_target_properties( gmock_main   PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/GoogleTest")
set_target_properties( gtest        PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/GoogleTest")
set_target_properties( gtest_main   PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/GoogleTest")