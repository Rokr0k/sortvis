#include "shader.hpp"
#include "sort_ctx.hpp"
#include <GLES3/gl32.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

  SDL_Window *window = SDL_CreateWindow(
      "SortVis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  SDL_GLContext gl = SDL_GL_CreateContext(window);

  SDL_GL_SetSwapInterval(-1);

  sv::SortContext sortCtx;

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sortCtx.GetArray().size_bytes(), nullptr,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint shaders[] = {
      sv::MakeShader(sv::hist_vert, sv::hist_geom, sv::hist_frag),
      sv::MakeShader(sv::pie_vert, sv::pie_geom, sv::pie_frag),
      sv::MakeShader(sv::dot_vert, sv::dot_geom, sv::dot_frag),
  };

  sv::SortEvent sEvent{sv::SortBehavior::None, 0, 0};

  bool autorun = false;
  int speed = 1;
  int displayMode = 0;

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
        case SDLK_TAB:
          displayMode =
              (displayMode + 1) % (sizeof(shaders) / sizeof(*shaders));
          break;
        case SDLK_1:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(32);
            goto resizebuffer;
          } else {
            speed = 1;
          }
          break;
        case SDLK_2:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(64);
            goto resizebuffer;
          } else {
            speed = 2;
          }
          break;
        case SDLK_3:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(128);
            goto resizebuffer;
          } else {
            speed = 4;
          }
          break;
        case SDLK_4:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(256);
            goto resizebuffer;
          } else {
            speed = 8;
          }
          break;
        case SDLK_5:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(512);
            goto resizebuffer;
          } else {
            speed = 16;
          }
          break;
        case SDLK_6:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(1024);
            goto resizebuffer;
          } else {
            speed = 32;
          }
          break;
        case SDLK_7:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(2048);
            goto resizebuffer;
          } else {
            speed = 64;
          }
          break;
        case SDLK_8:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(4096);
            goto resizebuffer;
          } else {
            speed = 128;
          }
          break;
        case SDLK_9:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(8192);
            goto resizebuffer;
          } else {
            speed = 256;
          }
          break;
        case SDLK_0:
          if (event.key.keysym.mod & KMOD_SHIFT) {
            sortCtx.SetSize(16384);
            goto resizebuffer;
          } else {
            speed = 512;
          }
          break;
        resizebuffer:
          glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sortCtx.GetArray().size_bytes(),
                    sortCtx.GetArray().data());
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), 0);

    GLuint program = shaders[displayMode];
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
  glDeleteBuffers(1, &vbo);
  for (GLuint shader : shaders) {
    glDeleteProgram(shader);
  }

  SDL_GL_DeleteContext(gl);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
