cmake_minimum_required(VERSION 3.15)

project(SSAO LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY}/bin)

#set(ENGIN3D_BUILD_SETTINGS ON) #might break on windows, might not...

add_subdirectory(lib/Engin3D)
add_subdirectory(src)

