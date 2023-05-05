//
// Created by shriller44 on 4/16/23.
//

#ifndef VOXEL_GUI_H
#define VOXEL_GUI_H

#include "../../../external/ImGui/imgui_impl_opengl3.h"
#include "../../../external/ImGui/imgui_impl_sdl.h"
#include "../../../external/ImGui/imgui_internal.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "interface.h"

//class Interface {
// public:
//
//  explicit Interface(SDL_Window* window)
//      : window_{window},
//        state_{0},
//        branch_length_min_{1},
//        branch_length_max_{20},
//        production_count_min_{1},
//        production_count_max_{10},
//        branching_angle_min_{5},
//        branching_angle_max_{45},
//        chunk_index_min_ {0},
//        chunk_index_max_ {1},
//  show{true},flags_{}{}
//
//  static bool SliderInt(const char* label, int* value, uint32_t min, uint32_t max)
//  {
//    ImGui::PushID(label);
//    ImGui::Text("%s", label);
//    ImGui::SameLine();
//    bool result = ImGui::SliderInt("", value, static_cast<int>(min), static_cast<int>(max));
//    ImGui::PopID();
//    return result;
//  }
//
//  struct RenderState {
//    int branch_length_;
//    int production_count_;
//    int colour_;
//    int branching_angle_;
//    int chunk_index_;
//  };
//
//  enum Flags {
//    kExport = 1 << 0,
//    kProcessAgain  = 1 << 2,
//    kShow = 1 << 3,
//    kSwapChunks = 1 << 4
//  };
//
//  [[nodiscard]] bool FlagSet(Flags flags) const {return (flags_ & flags) == flags;}
//  void ToggleFlag(Flags flags) { flags_ = static_cast<Flags>(flags_ ^ static_cast<Flags>(flags));}
//
//  void CoreGui(){
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplSDL2_NewFrame(window_);
//    ImGui::NewFrame();
//    ImGui::Begin("test window");
//    if (ImGui::Button("show model")){
//      ToggleFlag(Flags::kShow);
//    }
//    if (ImGui::Button("Re-render")){
//      ToggleFlag(Flags::kProcessAgain);
//    }
//    if (ImGui::Button("export to OBJ")){
//      ToggleFlag(Flags::kExport);
//    }
//    if (ImGui::Button("Swap Chunk")){
//      ToggleFlag(Flags::kSwapChunks);
//    }
//    SliderInt("branch length", &state_.branch_length_, branch_length_min_, branch_length_max_);
//    SliderInt("production count", &state_.production_count_, production_count_min_, production_count_max_);
//    SliderInt("branching angle", &state_.branching_angle_, branching_angle_min_, branching_angle_max_);
//    SliderInt("model type", &state_.chunk_index_, chunk_index_min_, chunk_index_max_);
//  }
//
//  RenderState* GetState(){ return &state_; }
//
// private:
//
//  bool show;
//
//  uint32_t branch_length_min_;
//  uint32_t branch_length_max_;
//  uint32_t production_count_min_;
//  uint32_t production_count_max_;
//  uint32_t branching_angle_min_;
//  uint32_t branching_angle_max_;
//  uint32_t chunk_index_min_;
//  uint32_t chunk_index_max_;
//
//  RenderState state_;
//  Flags flags_;
//  SDL_Window* window_;
//};

class GUI {
 public:

  GUI(int screen_width, int screen_height);
  void Destroy();
  [[nodiscard]] bool FlagSet(Interface::Flags flags) const;
  void ShowGUI();
  std::tuple<int,int,int,int, std::string, int> GetState();
  int GetChunkIndex ();
  int GetMapIndex();
  void SetFlag(Interface::Flags flag);
  void Init() const;
  void NewFrame() const;
  void SwapWindow() const;
  std::string GetFileName();

 private:
  SDL_Window *window_;
  SDL_GLContext context_;
  std::unique_ptr<Interface> imgui_interface_;
};
#endif  // VOXEL_GUI_H
