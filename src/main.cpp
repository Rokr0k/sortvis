#include "sort_ctx.hpp"
#include <GLES3/gl32.h>
#include <SDL.h>

const char *const vertexShaderSource = R"(
    #version 320 es

    layout (location = 0) in int aValue;

    uniform int _Size;

    out int index;

    void main() {
        float x = float(gl_VertexID) / float(_Size) * 2.0 - 1.0;
        float y = float(aValue + 1) / float(_Size) * 2.0 - 1.0;
        float size = 1.0 / float(_Size);
        gl_Position = vec4(x, y, 0, 1);
        index = gl_VertexID;
    }
)";
const char *const geometryShaderSource = R"(
    #version 320 es

    layout(points) in;

    in int index[];

    uniform int _Size;

    flat out int Index;

    layout(triangle_strip, max_vertices = 4) out;

    void main() {
        vec4 pos = gl_in[gl_PrimitiveIDIn].gl_Position;
        vec2 size = vec2(2.0 / float(_Size), -1.0 - pos.y);

        gl_Position = pos;
        Index = index[gl_PrimitiveIDIn];
        EmitVertex();

        gl_Position = pos + vec4(size * vec2(1, 0), 0, 0);
        Index = index[gl_PrimitiveIDIn];
        EmitVertex();

        gl_Position = pos + vec4(size * vec2(0, 1), 0, 0);
        Index = index[gl_PrimitiveIDIn];
        EmitVertex();

        gl_Position = pos + vec4(size, 0, 0);
        Index = index[gl_PrimitiveIDIn];
        EmitVertex();

        EndPrimitive();
    }
)";
const char *const fragmentShaderSource = R"(
    #version 320 es

    #ifdef GL_ES
    precision highp float;
    #endif

    flat in int Index;

    uniform int _Behavior;
    uniform int _Arg0;
    uniform int _Arg1;

    out vec4 outColor;

    void main() {
        if(_Behavior == 1 && (Index == _Arg0 || Index == _Arg1)) {
            outColor = vec4(0, 0, 1, 1);
        } else if(_Behavior == 2 && (Index == _Arg0 || Index == _Arg1)) {
            outColor = vec4(1, 0, 0, 1);
        } else if(_Behavior == 3 && Index == _Arg0) {
            outColor = vec4(0, 1, 0, 1);
        } else {
            outColor = vec4(1);
        }
    }
)";

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

  SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

  SDL_Window *window =
      SDL_CreateWindow("SortVis", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

  SDL_GLContext gl = SDL_GL_CreateContext(window);

  SDL_GL_SetSwapInterval(-1);

  sv::SortContext sortCtx;

  GLuint abo = 0;
  glGenBuffers(1, &abo);
  glBindBuffer(GL_ARRAY_BUFFER, abo);
  glBufferData(GL_ARRAY_BUFFER, sortCtx.GetArray().size_bytes(), nullptr,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  int success;
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
  glCompileShader(geometryShader);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, geometryShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glDetachShader(program, vertexShader);
  glDetachShader(program, geometryShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  sv::SortEvent sEvent{sv::SortBehavior::None, 0, 0};

  bool autorun = false;
  int speed = 1;

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        goto quit;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_RETURN:
          if (!autorun) {
            sEvent = sortCtx.Next();
          }
          break;
        case SDLK_SPACE:
          autorun = !autorun;
          break;
        case SDLK_1:
          speed = 1;
          break;
        case SDLK_2:
          speed = 4;
          break;
        case SDLK_3:
          speed = 8;
          break;
        case SDLK_4:
          speed = 16;
          break;
        case SDLK_5:
          sortCtx.SetSize(64);
          goto resizebuffer;
        case SDLK_6:
          sortCtx.SetSize(128);
          goto resizebuffer;
        case SDLK_7:
          sortCtx.SetSize(256);
          goto resizebuffer;
        case SDLK_8:
          sortCtx.SetSize(512);
          goto resizebuffer;
        case SDLK_9:
          sortCtx.SetSize(1024);
          goto resizebuffer;
        case SDLK_0:
          sortCtx.SetSize(2048);
          goto resizebuffer;
        resizebuffer:
          glBindBuffer(GL_ARRAY_BUFFER, abo);
          glBufferData(GL_ARRAY_BUFFER, sortCtx.GetArray().size_bytes(),
                       nullptr, GL_DYNAMIC_DRAW);
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          break;
        case SDLK_r:
          sortCtx.Shuffle();
          break;
        case SDLK_b:
          sortCtx.BubbleSort();
          break;
        case SDLK_s:
          sortCtx.SelectionSort();
          break;
        case SDLK_i:
          sortCtx.InsertionSort();
          break;
        case SDLK_m:
          sortCtx.MergeSort();
          break;
        case SDLK_q:
          sortCtx.QuickSort();
          break;
        }
      }
    }

    if (autorun) {
      for (int i = 0; i < speed; i++) {
        sEvent = sortCtx.Next();
      }
    }

    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);

    glViewport(0, 0, w, h);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, abo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sortCtx.GetArray().size_bytes(),
                    sortCtx.GetArray().data());
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), 0);

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "_Size"),
                sortCtx.GetArray().size());
    glUniform1i(glGetUniformLocation(program, "_Behavior"),
                (int)sEvent.behavior);
    glUniform1i(glGetUniformLocation(program, "_Arg0"), sEvent.arg0);
    glUniform1i(glGetUniformLocation(program, "_Arg1"), sEvent.arg1);
    glDrawArrays(GL_POINTS, 0, sortCtx.GetArray().size());
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    SDL_GL_SwapWindow(window);
  }

quit:
  glDeleteBuffers(1, &abo);
  glDeleteProgram(program);

  SDL_GL_DeleteContext(gl);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
