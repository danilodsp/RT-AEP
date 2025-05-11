cmake_minimum_required(VERSION 3.10)
project(rt-aep VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use environment variable to locate PortAudio
if(DEFINED ENV{PORTAUDIO_PATH})
    set(PORTAUDIO_PATH $ENV{PORTAUDIO_PATH})
    message(STATUS "Using PORTAUDIO_PATH from environment: ${PORTAUDIO_PATH}")
    
    set(PORTAUDIO_INCLUDE_DIR ${PORTAUDIO_PATH}/include)
    set(PORTAUDIO_LIB_DIR ${PORTAUDIO_PATH}/build/Release)
    set(PORTAUDIO_LIB ${PORTAUDIO_LIB_DIR}/portaudio_static_x64.lib)  # (portaudio_x64.lib if using DLL)
else()
    message(FATAL_ERROR "PORTAUDIO_PATH environment variable not set.")
endif()


# Add GLFW (external)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw ${CMAKE_BINARY_DIR}/external/glfw-build EXCLUDE_FROM_ALL)

# Add internal project include dir
add_subdirectory(src)
add_subdirectory(gui)

# Add the main executable
add_executable(rt-aep main.cpp)

# Include directories
target_include_directories(rt-aep PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${PORTAUDIO_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/gui
)

# Link directories (needed for .lib)
link_directories(${PORTAUDIO_LIB_DIR})

# Link libraries
target_link_libraries(rt-aep PRIVATE
    AudioEngine
    gui
    ${PORTAUDIO_LIB}
)
