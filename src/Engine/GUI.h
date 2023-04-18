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

  GUI(int screen_width, int screen_height);

  void Destroy();

  [[nodiscard]] bool FlagSet(Interface::Flags flags) const;

  void ShowGUI();

  void Init() const;
  void NewFrame() const;
  void SwapWindow() const;

 private:
  SDL_Window *window_;
  SDL_GLContext context_;
  std::unique_ptr<Interface> imgui_interface_;
};

#endif  // VOXEL_GUI_H
