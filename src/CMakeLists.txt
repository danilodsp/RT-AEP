add_library(AudioEngine STATIC AudioEngine.cpp)

# Add headers and PortAudio headers
target_include_directories(AudioEngine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PORTAUDIO_INCLUDE_DIR}
)

# Link to PortAudio depending on how it was found
if(TARGET PortAudio::PortAudio)
    target_link_libraries(AudioEngine PUBLIC PortAudio::PortAudio)
elseif(DEFINED PortAudio_LIBRARY)
    target_link_libraries(AudioEngine PUBLIC ${PortAudio_LIBRARY})
endif()
