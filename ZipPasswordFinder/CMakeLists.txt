cmake_minimum_required(VERSION 3.13)
project(ZipPasswordFinder)

set(CMAKE_CXX_STANDARD 17)

add_executable(ZipPasswordFinder main.cpp)

INCLUDE_DIRECTORIES(/usr/include)

add_library(libzip STATIC IMPORTED)

set_property(TARGET libzip PROPERTY IMPORTED_LOCATION /usr/lib64/libzip.so)

target_link_libraries(ZipPasswordFinder libzip)





