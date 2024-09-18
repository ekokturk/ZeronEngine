# Copyright (C) Eser Kokturk. All Rights Reserved.

# glm - Math Library

set(GLM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT EXISTS ${GLM_SOURCE_DIR}/glm)
     file(COPY           ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/glm/ 
          DESTINATION    ${GLM_SOURCE_DIR}/glm
     )
endif()

add_library(glm-header-only INTERFACE)
target_include_directories(glm-header-only INTERFACE ${GLM_SOURCE_DIR})
target_compile_definitions(glm-header-only 
     INTERFACE 
          GLM_FORCE_DEPTH_ZERO_TO_ONE
          GLM_FORCE_LEFT_HANDED
)
