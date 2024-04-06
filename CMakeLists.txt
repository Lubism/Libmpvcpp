cmake_minimum_required(VERSION 3.5)

project(
    Libmpvcpp
    VERSION 1.0.0
    DESCRIPTION "The C++ framework of the media library \"mpv\""
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories("./external")
include_directories("./src/")

add_library(libmpv SHARED IMPORTED)

set_target_properties(libmpv PROPERTIES IMPORTED_LOCATION
    ${PROJECT_SOURCE_DIR}/external/lib/libmpv.dll.a)

file(GLOB_RECURSE linkedFile CMAKE_CONFIGURE_DEPENDS
src/mpvcpp/code/*.hpp
src/mpvcpp/*.hpp
external/mpv/*.h
)

add_executable(Test "./test/main.cpp" ${linkedFile})

target_link_directories(Test PUBLIC "./external/mpv")

target_link_libraries(Test ${PROJECT_SOURCE_DIR}/external/lib/libmpv-2.dll)
