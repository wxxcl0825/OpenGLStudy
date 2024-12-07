cmake_minimum_required(VERSION 3.12)

project(OpenGL_Template)

set(CMAKE_CXX_STANDARD 17)

# 向项目中加入全局预编译宏
add_definitions(-DDEBUG)

include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

add_subdirectory(wrapper)

add_executable(openglStudy "main.cpp" "glad.c")

find_package(glfw3 3.4 REQUIRED)

target_link_libraries(openglStudy glfw wrapper)