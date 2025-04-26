cmake_minimum_required(VERSION 3.17)
project(VulkanProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# check env variable
if (NOT DEFINED ENV{GLFW_HOME})
    message(FATAL_ERROR "found no env named GLFW_HOME")
endif ()
if (NOT DEFINED ENV{GLM_HOME})
    message(FATAL_ERROR "found no env named GLM_HOME")
endif ()
if (NOT DEFINED ENV{VMA_HOME})
    message(FATAL_ERROR "found no env named VMA_HOME")
endif ()

# store env variable
set(GLFW_HOME $ENV{GLFW_HOME})
set(GLM_HOME $ENV{GLM_HOME})
set(VMA_HOME $ENV{VMA_HOME})

add_executable(VulkanProject main.cpp
        Header/common.h)

# add GLFW3 lib
add_library(glfw SHARED IMPORTED
        Header/common.h)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.dylib")

# GLM
include_directories(${GLM_INCLUDE_DIRS})

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)
#include_directories(${Vulkan_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})

# VulkanMemoryAllocator
include_directories(${VMA_HOME}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${VMA_HOME}/include)

#set(VulkanMemoryAllocator_DIR )
#find_package(VulkanMemoryAllocator CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)

# 1. locate glslangValidator
find_program(GLSLANG_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/Bin)
if (NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found!")
endif ()

# 2. define output dir
set(SPIRV_BIN ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SPIRV_BIN})

# 3. compile shader
function(compile_glsl SHADER)
    get_filename_component(SUFFIX ${SHADER} EXT)
    string(SUBSTRING "${SUFFIX}" 1 -1 SUFFIX_NO_DOT)
    set(OUT ${SPIRV_BIN}/${SUFFIX_NO_DOT}.spv)
    add_custom_command(
            OUTPUT ${OUT}
            COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${OUT}
            MAIN_DEPENDENCY ${SHADER}
            COMMENT "Compiling ${SHADER} → ${OUT}"
            VERBATIM
    )
    list(APPEND SPV_SHADERS ${OUT})
    set(SPV_SHADERS ${SPV_SHADERS} PARENT_SCOPE)
endfunction()

# copy the shader files to the cmake-build-debug folder
#file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 4. scan -> generate spv shaders
file(GLOB_RECURSE GLSL_SHADERS "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")
foreach (S ${GLSL_SHADERS})
    compile_glsl(${S})
endforeach ()

# 5. create virtual target -> let the app depend
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})
add_dependencies(${PROJECT_NAME} Shaders)
