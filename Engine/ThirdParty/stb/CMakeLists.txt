# Copyright (C) Eser Kokturk. All Rights Reserved.

# STB - Image Loading Library
set(SUBMODULE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/.submodule)
set(STB_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(REMOVE_RECURSE ${STB_SOURCE_DIR})

configure_file(${SUBMODULE_DIR}/stb_image.h         ${STB_SOURCE_DIR}/stb/stb_image.h          COPYONLY)
configure_file(${SUBMODULE_DIR}/stb_image_write.h   ${STB_SOURCE_DIR}/stb/stb_image_write.h    COPYONLY)
configure_file(${SUBMODULE_DIR}/stb_easy_font.h     ${STB_SOURCE_DIR}/stb/stb_easy_font.h      COPYONLY)
configure_file(${SUBMODULE_DIR}/stb_image_resize.h  ${STB_SOURCE_DIR}/stb/stb_image_resize.h   COPYONLY)
configure_file(${SUBMODULE_DIR}/stb_truetype.h      ${STB_SOURCE_DIR}/stb/stb_truetype.h       COPYONLY)
configure_file(${SUBMODULE_DIR}/stb_rect_pack.h      ${STB_SOURCE_DIR}/stb/stb_rect_pack.h     COPYONLY)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${STB_SOURCE_DIR})