cmake_minimum_required(VERSION 3.21)

project(KiDS)

add_subdirectory(./imgui)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(KiDS 
main.cpp
)

target_link_libraries(KiDS imgui glfw OpenGL::GL ${CMAKE_DL_LIBS})