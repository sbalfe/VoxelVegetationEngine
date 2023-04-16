//
// Created by shriller44 on 4/16/23.
//

#ifndef VOXEL_GUI_H
#define VOXEL_GUI_H

#include <SDL2/SDL.h>
#include "../ImGui/imgui_impl_sdl.h"
class GUI {

 public:

  GUI(int screen_width, int screen_height){
    // Create main window
    window_ = SDL_CreateWindow(
        "SDL App",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(screen_width), static_cast<int>(screen_height),
        SDL_WINDOW_OPENGL);

    if (window_ == nullptr) {
      fprintf(stderr, "failed to create main window\n");
      SDL_Quit();
    }

    // Initialize rendering context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* create opengl context in SDL */
    context_ = SDL_GL_CreateContext(window_);

    SDL_GL_SetSwapInterval(1); // Use VSYNC

  }

  void Destroy(){
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  void Init(){
    ImGui_ImplSDL2_InitForOpenGL(window_, SDL_GL_GetCurrentContext());
  }

  void NewFrame(){
    ImGui_ImplSDL2_NewFrame(window_);
  }

  void SwapWindow(){
    SDL_GL_SwapWindow(window_);
  }

 private:
  SDL_GLContext context_;
  SDL_Window *window_;

};

#endif  // VOXEL_GUI_H
