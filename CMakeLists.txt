cmake_minimum_required(VERSION 3.29)
project(ttfatlas C)

set(CMAKE_C_STANDARD 11)
set(LIB_SOURCES TTFat/font.c
        TTFat/font.h
        TTFat/atlas.c
        TTFat/atlas.h)

add_library(ttfatlas ${LIB_SOURCES})
add_executable(ttfatlas_test main.c
        ${LIB_SOURCES})

target_include_directories(ttfatlas_test PUBLIC "/Users/alifezithu/Downloads/glfw-3.4 2/include")
target_link_directories(ttfatlas_test PUBLIC "/Users/alifezithu/Downloads/glfw-3.4 2/src")
target_link_libraries(ttfatlas_test PUBLIC "-framework Cocoa -framework IOKit -lglfw3 -framework OpenGL")
