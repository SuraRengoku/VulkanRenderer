# find glslangValidator
find_program(GLSLANG_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/Bin)
if (NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found!")
endif ()

# set output dir
set(SPIRV_BIN ${CMAKE_BINARY_DIR}/Shaders)
file(MAKE_DIRECTORY ${SPIRV_BIN})

# compile GLSL 
function(compile_glsl SHADER)
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(OUT ${SPIRV_BIN}/${FILE_NAME}.spv)
    add_custom_command(
        OUTPUT ${OUT}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${OUT}
        MAIN_DEPENDENCY ${SHADER}
        COMMENT "Compiling GLSL ${SHADER} → ${OUT}"
        VERBATIM
    )
    list(APPEND SPV_SHADERS ${OUT})
    set(SPV_SHADERS ${SPV_SHADERS} PARENT_SCOPE)
endfunction()

# compile HLSL
function(compile_hlsl SHADER)
    get_filename_component(FILE_NAME ${SHADER} NAME)

    if (FILE_NAME MATCHES "\\.vert\\.hlsl$")
        set(STAGE vert)
    elseif (FILE_NAME MATCHES "\\.frag\\.hlsl$")
        set(STAGE frag)
    else()
        message(FATAL_ERROR "Unknown shader stage in ${SHADER}")
    endif()

    set(OUT ${SPIRV_BIN}/${FILE_NAME}.spv)
    add_custom_command(
        OUTPUT ${OUT}
        COMMAND ${GLSLANG_VALIDATOR} -V -D -e main -S ${STAGE} ${SHADER} -o ${OUT}
        MAIN_DEPENDENCY ${SHADER}
        COMMENT "Compiling HLSL ${SHADER} → ${OUT}"
        VERBATIM
    )
    list(APPEND SPV_SHADERS ${OUT})
    set(SPV_SHADERS ${SPV_SHADERS} PARENT_SCOPE)
endfunction()

# assemble and complie
file(GLOB_RECURSE GLSL_SHADERS "${CMAKE_SOURCE_DIR}/Shaders/*.vert.glsl" "${CMAKE_SOURCE_DIR}/Shaders/*.frag.glsl")
file(GLOB_RECURSE HLSL_SHADERS "${CMAKE_SOURCE_DIR}/Shaders/*.vert.hlsl" "${CMAKE_SOURCE_DIR}/Shaders/*.frag.hlsl")

set(SPV_SHADERS "")
foreach(S ${GLSL_SHADERS})
    compile_glsl(${S})
endforeach()
foreach(S ${HLSL_SHADERS})
    compile_hlsl(${S})
endforeach()

# add compilation targets
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})
add_dependencies(${PROJECT_NAME} Shaders)
