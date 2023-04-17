//
// Created by shriller44 on 4/16/23.
//

#ifndef VOXEL_GUI_H
#define VOXEL_GUI_H

#include <SDL2/SDL.h>
#include <iostream>
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include <memory>


class Interface {
 public:

  explicit Interface(SDL_Window* window): window_{window}, min{}, max{} {}

  static bool SliderInt(const char* label, int* value, uint32_t min, uint32_t max)
  {
    ImGui::PushID(label);
    ImGui::Text("%s", label);
    ImGui::SameLine();
    bool result = ImGui::SliderInt("", value, static_cast<int>(min), static_cast<int>(max));
    ImGui::PopID();
    return result;
  }

  struct RenderState {
    int branching_angle_;
    int production_count_;
    int branch_length_;
  };

  enum Flags {
    kExport = 1 << 0,
    kClearVoxels = 1 << 1,
    kProcessAgain  = 1 << 2,
    kShow = 1 << 3
  };

  [[nodiscard]] bool FlagSet(Flags flags) const {return (flags_ & flags) == flags;}
  void ToggleFlag(Flags flags) { flags_ = static_cast<Flags>(flags_ ^ static_cast<Flags>(flags));}

  void CoreGui(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_);
    ImGui::NewFrame();
    ImGui::Begin("test window");
    if (ImGui::Button("show model")){
      ToggleFlag(Flags::kShow);
    }
    if (ImGui::Button("re-render")){
      ToggleFlag(Flags::kProcessAgain);
    }
    if (ImGui::Button("export to OBJ")){
      ToggleFlag(Flags::kExport);
    }
    SliderInt("branch length", &state_.branch_length_, min, max);
    SliderInt("production count", &state_.production_count_, min, max);
  }

 private:
  bool show;
  int branch_length_;
  int production_count_;
  uint32_t min;
  uint32_t max;
  RenderState state_;
  Flags flags_;
  SDL_Window* window_;
};

class GUI {

 public:

  GUI(int screen_width, int screen_height){

    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cout << "test" << std::endl;
      fprintf(stderr, "Failed to initialize SDL video\n");
    }

    // Create main window
    window_ = SDL_CreateWindow(
        "SDL App",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(screen_width), static_cast<int>(screen_height),
        SDL_WINDOW_OPENGL);

    if (window_ == nullptr) {
      fprintf(stderr, "Failed to create main window\n");
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

    if (context_ == nullptr) {
      fprintf(stderr, "Failed to create GL context\n");
      SDL_DestroyWindow(window_);
      SDL_Quit();
    }

    imgui_interface_ =  std::make_unique<Interface>(window_);

    SDL_GL_SetSwapInterval(1); // Use VSYNC

  }

  void Destroy(){
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  [[nodiscard]] bool FlagSet(Interface::Flags flags) const { return imgui_interface_->FlagSet(flags);}

  void ShowGUI(){
    imgui_interface_->CoreGui();
  }

  void Init() const {
    ImGui_ImplSDL2_InitForOpenGL(window_, SDL_GL_GetCurrentContext());
  }

  void NewFrame() const {
    ImGui_ImplSDL2_NewFrame(window_);
  }

  void SwapWindow() const {
    SDL_GL_SwapWindow(window_);
  }
  SDL_Window *window_;
 private:
  SDL_GLContext context_;
  std::unique_ptr<Interface> imgui_interface_;

};

#endif  // VOXEL_GUI_H
