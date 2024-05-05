#pragma once

#include <GL/glew.h>

namespace sv {
extern const char *const hist_vert;
extern const char *const hist_geom;
extern const char *const hist_frag;

extern const char *const pie_vert;
extern const char *const pie_geom;
extern const char *const pie_frag;

extern const char *const dot_vert;
extern const char *const dot_geom;
extern const char *const dot_frag;

extern const char *const line_vert;
extern const char *const line_frag;

GLuint MakeShader(const char *vert, const char *geom, const char *frag);
} // namespace sv
