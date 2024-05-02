cmake_minimum_required(VERSION 3.20)

project(sortvis)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED GLES3)

add_executable(sortvis "src/main.cpp" "src/shader.cpp" "src/sort_ctx.cpp" "shaders/hist_vert.glsl" "shaders/hist_geom.glsl" "shaders/hist_frag.glsl" "shaders/pie_vert.glsl" "shaders/pie_geom.glsl" "shaders/pie_frag.glsl")

target_include_directories(sortvis PRIVATE "shaders")
target_link_libraries(sortvis PRIVATE SDL2::SDL2 SDL2::SDL2main OpenGL::GLES3)
