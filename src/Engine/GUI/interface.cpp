//
// Created by shriller44 on 5/1/23.
//

#include "interface.h"

Interface::Interface(SDL_Window* window)
    : window_{window},
      state_{0},
      branch_length_min_{1},
      branch_length_max_{20},
      production_count_min_{1},
      production_count_max_{10},
      branching_angle_min_{5},
      branching_angle_max_{45},
      chunk_index_min_ {0},
      chunk_index_max_ {1},
      rule_min_ {0},
      rule_max_ {1},
      root_size_min_{1},
      root_size_max_{6},
      show{true},flags_{}{}

void Interface::CoreGui(){
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window_);
  ImGui::NewFrame();
  ImGui::Begin("test window");
  if (ImGui::Button("Show")){
    ToggleFlag(Flags::kShow);
  }
  if (ImGui::Button("Update Render")){
    ToggleFlag(Flags::kProcessAgain);
  }
  if (ImGui::Button("Swap Chunk")){
    ToggleFlag(Flags::kSwapChunks);
  }
  if (ImGui::Button("Load Voxel File")){
    ToggleFlag(Flags::kLoadCustomFile);
  }
  if (ImGui::Button("Swap Map")){
    ToggleFlag(Flags::kSwapMaps);
  }

  ImGui::InputText("filename", state_.voxel_file_, 256);
  ImGui::InputText("axiom", state_.axiom_, 256);
  SliderInt("branch length", &state_.branch_length_, branch_length_min_, branch_length_max_);
  SliderInt("Root Branch Size", &state_.root_size_, root_size_min_, root_size_max_);
  SliderInt("production count", &state_.production_count_, production_count_min_, production_count_max_);
  SliderInt("branching angle", &state_.branching_angle_, branching_angle_min_, branching_angle_max_);
  SliderInt("model type", &state_.chunk_index_, chunk_index_min_, chunk_index_max_);
  SliderInt("rule map", &state_.rule_map_, rule_min_, rule_max_);
}


bool Interface::SliderInt(const char* label, int* value, uint32_t min, uint32_t max)
{
  ImGui::PushID(label);
  ImGui::Text("%s", label);
  ImGui::SameLine();
  bool result = ImGui::SliderInt("", value, static_cast<int>(min), static_cast<int>(max));
  ImGui::PopID();
  return result;
}

Interface::RenderState* Interface::GetState(){ return &state_; }

[[nodiscard]] bool Interface::FlagSet(Flags flags) const {return (flags_ & flags) == flags;}
void Interface::ToggleFlag(Flags flags) { flags_ = static_cast<Flags>(flags_ ^ static_cast<Flags>(flags));}