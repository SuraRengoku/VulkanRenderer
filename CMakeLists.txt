cmake_minimum_required(VERSION 3.17)
project(VulkanRenderer)

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

set(LUA_SRC_PATH Src/luasrc)
set(IMGUI_DIR_PATH Src/imgui-1.91.9b)

add_executable(VulkanRenderer main.cpp
        Header/common.h
        Header/stb_image.h
        Header/tiny_obj_loader.h
        ${IMGUI_DIR_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR_PATH}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR_PATH}/imgui.cpp
        ${IMGUI_DIR_PATH}/imgui_draw.cpp
        ${IMGUI_DIR_PATH}/imgui_demo.cpp
        ${IMGUI_DIR_PATH}/imgui_tables.cpp
        ${IMGUI_DIR_PATH}/imgui_widgets.cpp
)

# add GLFW3 lib
add_library(glfw SHARED IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.dylib")

# GLM
include_directories(${GLM_INCLUDE_DIRS})

#Lua
include_directories(${LUA_SRC_PATH})
aux_source_directory(${LUA_SRC_PATH} LUA_CORE)
set(LIB_SRC ${LUA_CORE})
list(REMOVE_ITEM LIB_SRC Src/luasrc/lua.c Src/luasrc/luac.c)
add_library(lualib ${LIB_SRC})

add_executable(lua ${LUA_SRC_PATH}/lua.c)
target_link_libraries(lua lualib)

add_executable(luac ${LUA_SRC_PATH}/luac.c)
target_link_libraries(luac lualib)

#ImGui
include_directories(${IMGUI_DIR_PATH} ${IMGUI_DIR_PATH}/backends)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)
#include_directories(${Vulkan_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})

# VulkanMemoryAllocator
include_directories(${VMA_HOME}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${VMA_HOME}/include)

set(VulkanMemoryAllocator_DIR)
# find_package(VulkanMemoryAllocator CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)

# 1. locate glslangValidator
find_program(GLSLANG_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/Bin)
if (NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found!")
endif ()

# 2. define output dir
set(SPIRV_BIN ${CMAKE_BINARY_DIR}/Shaders)
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
file(GLOB_RECURSE GLSL_SHADERS "${CMAKE_SOURCE_DIR}/Shaders/*.vert" "${CMAKE_SOURCE_DIR}/Shaders/*.frag")
foreach (S ${GLSL_SHADERS})
    compile_glsl(${S})
endforeach ()

target_compile_definitions(${PROJECT_NAME} PRIVATE __SHADER_PATH__="${SPIRV_BIN}/")
target_compile_definitions(${PROJECT_NAME} PRIVATE __TEXTURE_PATH__="${CMAKE_SOURCE_DIR}/Textures/")
target_compile_definitions(${PROJECT_NAME} PRIVATE __MODEL_PATH__="${CMAKE_SOURCE_DIR}/Models/")

# 5. create virtual target -> let the app depend
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})
add_dependencies(${PROJECT_NAME} Shaders)
