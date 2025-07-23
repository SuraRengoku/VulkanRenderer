cmake_minimum_required(VERSION 3.17)
project(VulkanRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# check system variables
foreach(VAR GLFW_HOME GLM_HOME VMA_HOME)
    if (NOT DEFINED ENV{${VAR}})
        message(FATAL_ERROR "found no env named ${VAR}")
    endif ()
endforeach()

# set system variables
set(GLFW_HOME $ENV{GLFW_HOME})
set(GLM_HOME $ENV{GLM_HOME})
set(VMA_HOME $ENV{VMA_HOME})

# main executable files
add_executable(VulkanRenderer main.cpp
    Header/common.h
    Header/stb_image.h
    Header/tiny_obj_loader.h
)

include_directories(Header)

# GLFW
if(WIN32)
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw3.a")
    include_directories(${GLFW_HOME}/include)
elseif(APPLE)
    add_library(glfw SHARED IMPORTED)
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.dylib")
endif()

# GLM
include_directories(${GLM_HOME})

# Vulkan SDK
if(WIN32)
    include_directories($ENV{VULKAN_SDK}/Include)
endif()
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)

# Vulkan Memory Allocator
include_directories(${VMA_HOME}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${VMA_HOME}/include)

# define path
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    __SHADER_PATH__="${CMAKE_BINARY_DIR}/Shaders/"
    __TEXTURE_PATH__="${CMAKE_SOURCE_DIR}/Textures/"
    __MODEL_PATH__="${CMAKE_SOURCE_DIR}/Models/"
)

# load modules
include(cmake/SetupLua.cmake)
include(cmake/SetupImgui.cmake)
include(cmake/CompileShaders.cmake)
