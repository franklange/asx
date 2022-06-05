project(asx)

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
    include/asx/audio_settings.hpp
    include/asx/queue.hpp
    include/asx/sink_portaudio.hpp
    include/asx/wav.hpp

    src/sink_portaudio.cpp
    src/wav.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")

target_link_libraries(${PROJECT_NAME} PUBLIC
    bln_queue
    portaudio
    sndfile
)