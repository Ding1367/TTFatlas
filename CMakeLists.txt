cmake_minimum_required(VERSION 3.29)
project(ttfatlas C)

set(CMAKE_C_STANDARD 11)

add_library(ttfatlas TTFat/font.c
        TTFat/font.h
        TTFat/atlas.c
        TTFat/atlas.h)

if(BUILD_TEST)
    add_executable(ttfatlas_test test/main.c test/glad.c)
    target_link_libraries(ttfatlas_test PRIVATE ttfatlas)
    target_include_directories(ttfatlas_test PRIVATE "test/include" "${PROJECT_SOURCE_DIR}")
    target_compile_definitions(ttfatlas_test PRIVATE "TEST_FONT_PATH=\"${PROJECT_SOURCE_DIR}/test/JetBrainsMono-Regular.ttf\"")
    target_link_directories(ttfatlas_test PRIVATE "test/lib")
    target_link_libraries(ttfatlas_test PRIVATE "-lglfw3")

    if(APPLE)
        target_link_libraries(ttfatlas_test PRIVATE "-framework Cocoa -framework IOKit -framework OpenGL")
    elseif(WIN32)
        if(MSVC)
            target_link_libraries(ttfatlas_test PRIVATE opengl32.lib gdi32.lib)
        else()
            target_link_libraries(ttfatlas_test PRIVATE opengl32 gdi32)
        endif()
    else()
        target_link_libraries(ttfatlas_test PRIVATE "-lGL $(pkg-config --static --libs glfw3)")
    endif()
    message("You need GLFW3 (static) to build the test.")
endif()