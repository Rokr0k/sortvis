#include "shader.hpp"
#include <SDL.h>

#define SHADER_EMBED(name, file)                                               \
  asm(".section \".rodata\", \"a\", @progbits\n"                               \
      "sv_" #name ":\n"                                                        \
      ".incbin \"" file "\"\n"                                                 \
      ".byte 0\n"                                                              \
      ".previous");                                                            \
  extern const char sv_##name[];                                               \
  const char *const sv::name = sv_##name;

SHADER_EMBED(hist_vert, "hist_vert.glsl")
SHADER_EMBED(hist_geom, "hist_geom.glsl")
SHADER_EMBED(hist_frag, "hist_frag.glsl")

SHADER_EMBED(pie_vert, "pie_vert.glsl")
SHADER_EMBED(pie_geom, "pie_geom.glsl")
SHADER_EMBED(pie_frag, "pie_frag.glsl")

SHADER_EMBED(dot_vert, "dot_vert.glsl")
SHADER_EMBED(dot_geom, "dot_geom.glsl")
SHADER_EMBED(dot_frag, "dot_frag.glsl")

GLuint sv::MakeShader(const char *vert, const char *geom, const char *frag) {
  GLuint vertex = 0;
  GLuint geometry = 0;
  GLuint fragment = 0;

  if (vert) {
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert, nullptr);
    glCompileShader(vertex);
    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint length;
      glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
      GLchar *log = new GLchar[length];
      glGetShaderInfoLog(vertex, length, &length, log);
      SDL_Log("%s", log);
      delete[] log;
    }
  }
  if (geom) {
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geom, nullptr);
    glCompileShader(geometry);
    GLint success;
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint length;
      glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &length);
      GLchar *log = new GLchar[length];
      glGetShaderInfoLog(geometry, length, &length, log);
      SDL_Log("%s", log);
      delete[] log;
    }
  }
  if (frag) {
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag, nullptr);
    glCompileShader(fragment);
    GLint success;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint length;
      glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
      GLchar *log = new GLchar[length];
      glGetShaderInfoLog(fragment, length, &length, log);
      SDL_Log("%s", log);
      delete[] log;
    }
  }

  GLuint program = glCreateProgram();

  if (vertex)
    glAttachShader(program, vertex);
  if (geometry)
    glAttachShader(program, geometry);
  if (fragment)
    glAttachShader(program, fragment);

  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar *log = new GLchar[length];
    glGetProgramInfoLog(program, length, &length, log);
    SDL_Log("%s", log);
    delete[] log;
  }

  if (vertex) {
    glDetachShader(program, vertex);
    glDeleteShader(vertex);
  }
  if (geometry) {
    glDetachShader(program, geometry);
    glDeleteShader(geometry);
  }
  if (fragment) {
    glDetachShader(program, fragment);
    glDeleteShader(fragment);
  }

  return program;
}
