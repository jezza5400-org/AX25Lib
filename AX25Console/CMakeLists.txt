cmake_minimum_required(VERSION 3.10)

project(AX25Console)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

set(CMAKE_CXX_STANDARD 17)

add_executable(AX25Console ${SOURCES} ${HEADERS})
target_link_libraries(AX25Console PRIVATE AX25Lib)