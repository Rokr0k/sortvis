cmake_minimum_required(VERSION 3.20)

project(sortvis)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED GLES3)

add_executable(sortvis "src/main.cpp" "src/sort_ctx.cpp")

target_link_libraries(sortvis PRIVATE SDL2::SDL2 SDL2::SDL2main OpenGL::GLES3)
