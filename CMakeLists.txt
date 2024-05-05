cmake_minimum_required(VERSION 3.20)

project(sortvis)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(sortvis "src/main.cpp" "src/shader.cpp" "src/sort_ctx.cpp")

target_include_directories(sortvis PRIVATE "shaders")
target_link_libraries(sortvis PRIVATE SDL2::SDL2 SDL2::SDL2main OpenGL::GL GLEW::GLEW)
