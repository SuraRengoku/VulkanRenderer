set(IMGUI_DIR_PATH ${CMAKE_SOURCE_DIR}/Src/imgui-1.91.9b)

include_directories(${IMGUI_DIR_PATH} ${IMGUI_DIR_PATH}/backends)

target_sources(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR_PATH}/backends/imgui_impl_vulkan.cpp
    ${IMGUI_DIR_PATH}/imgui.cpp
    ${IMGUI_DIR_PATH}/imgui_draw.cpp
    ${IMGUI_DIR_PATH}/imgui_demo.cpp
    ${IMGUI_DIR_PATH}/imgui_tables.cpp
    ${IMGUI_DIR_PATH}/imgui_widgets.cpp
)
