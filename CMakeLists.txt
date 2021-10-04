cmake_minimum_required(VERSION 3.21)

project(KiDS)

add_subdirectory(./imgui)
set (CMAKE_CXX_FLAGS "-g -O0")
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(KiDS 
main.cpp
memory.cpp memory.hpp
arm9.cpp arm9.hpp
arm9_instructions.cpp arm9_instructions.hpp
math.hpp math.cpp
)

target_link_libraries(KiDS imgui glfw OpenGL::GL ${CMAKE_DL_LIBS})