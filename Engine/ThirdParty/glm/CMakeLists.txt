# Copyright (C) Eser Kokturk. All Rights Reserved.

# glm - Math Library

set(GLM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(REMOVE_RECURSE ${GLM_SOURCE_DIR})

file(COPY           ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/glm/ 
     DESTINATION    ${GLM_SOURCE_DIR}/glm
)

add_library(glm-header-only INTERFACE)
target_include_directories(glm-header-only INTERFACE ${GLM_SOURCE_DIR})
