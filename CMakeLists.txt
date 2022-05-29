cmake_minimum_required(VERSION 3.10)
project(asx_top)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

enable_testing()
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(thirdparty/bln EXCLUDE_FROM_ALL)

add_subdirectory(thirdparty/portaudio EXCLUDE_FROM_ALL)
target_compile_options(portaudio PRIVATE -Wno-deprecated-declarations)

set(BUILD_TESTING OFF)
set(BUILD_REGTEST OFF)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
set(ENABLE_CPACK OFF)
set(ENABLE_EXTERNAL_LIBS OFF)
set(ENABLE_PACKAGE_CONFIG OFF)
set(INSTALL_PKGCONFIG_MODULE OFF)
add_subdirectory(thirdparty/libsndfile)

add_subdirectory(asx)
add_subdirectory(app)
