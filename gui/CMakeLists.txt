# Add ImGui, GLFW, and OpenGL to the build
# This assumes the ImGui, ImGui backends, and GLFW are in external/

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/external/glfw)

# ImGui sources
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)



add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ${GLFW_DIR}/include)

# GUI sources
set(GUI_SOURCES
    ${CMAKE_SOURCE_DIR}/gui/GUIManager.cpp
)

add_library(gui STATIC ${GUI_SOURCES})
target_include_directories(gui PUBLIC ${CMAKE_SOURCE_DIR}/gui ${IMGUI_DIR} ${IMGUI_DIR}/backends ${GLFW_DIR}/include ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gui imgui glfw OpenGL32)
