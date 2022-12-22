cmake_minimum_required(VERSION 3.10)
project(asx_top)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Copy tracks to build dir
file(COPY tracks DESTINATION ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(third_party/bln_log)
add_subdirectory(third_party/bln_net)

# Configure 3rd party deps
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_compile_options(-Wno-deprecated-declarations)

add_subdirectory(third_party/json)

set(PA_USE_JACK OFF)
add_subdirectory(third_party/portaudio)

set(BUILD_TESTING OFF)
set(BUILD_REGTEST OFF)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
set(ENABLE_CPACK OFF)
set(ENABLE_MPEG OFF)
set(ENABLE_EXTERNAL_LIBS OFF)
set(ENABLE_PACKAGE_CONFIG OFF)
set(INSTALL_PKGCONFIG_MODULE OFF)
add_subdirectory(third_party/libsndfile)

# ASX lib
set(ASX_LIB asx)
add_library(${ASX_LIB} "")

set_target_properties(${ASX_LIB} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
)

target_sources(${ASX_LIB} PRIVATE
    include/asx/json.hpp
    include/asx/message.hpp
    include/asx/log.hpp
    include/asx/server.hpp
    include/asx/sink.hpp
    include/asx/track.hpp
    include/asx/types.hpp

    src/json.cpp
    src/message.cpp
    src/server.cpp
    src/sink.cpp
    src/track.cpp
)

target_include_directories(${ASX_LIB} PUBLIC include)
target_compile_features(${ASX_LIB} PUBLIC cxx_std_20)

target_link_libraries(${ASX_LIB} PUBLIC
    bln_log
    bln_net
    nlohmann_json
    portaudio
    sndfile
)

# ASX daemon
set(ASXD asxd)
add_executable(${ASXD} src/daemon.cpp)

set_target_properties(${ASXD} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)

target_compile_features(${ASXD} PUBLIC cxx_std_20)
target_link_libraries(${ASXD} PUBLIC
    asx
)
