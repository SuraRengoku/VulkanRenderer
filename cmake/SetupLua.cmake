set(LUA_SRC_PATH ${CMAKE_SOURCE_DIR}/Src/luasrc)
include_directories(${LUA_SRC_PATH})
aux_source_directory(${LUA_SRC_PATH} LUA_CORE)

set(LIB_SRC ${LUA_CORE})
list(REMOVE_ITEM LIB_SRC 
    ${LUA_SRC_PATH}/lua.c 
    ${LUA_SRC_PATH}/luac.c
)

add_library(lualib ${LIB_SRC})

add_executable(lua ${LUA_SRC_PATH}/lua.c)
target_link_libraries(lua lualib)

add_executable(luac ${LUA_SRC_PATH}/luac.c)
target_link_libraries(luac lualib)
