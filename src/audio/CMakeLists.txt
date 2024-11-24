set(AUDIO_SOURCES
    AudioTypes.hpp
    AudioConfig.hpp
    AudioConfig.cpp
    SoundManager.hpp
    SoundManager.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${AUDIO_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
