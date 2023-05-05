//
// Created by shriller44 on 5/1/23.
//

#ifndef VOXEL_INTERFACE_H
#define VOXEL_INTERFACE_H

#include "../../../external/ImGui/imgui_impl_opengl3.h"
#include "../../../external/ImGui/imgui_impl_sdl.h"
#include "../../../external/ImGui/imgui_internal.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>

class Interface {
 public:

  struct RenderState {
    int branch_length_;
    int production_count_;
    int colour_;
    int branching_angle_;
    int chunk_index_;
    int rule_map_;
    int root_size_;
    char voxel_file_[256];
    char axiom_[256];
  };

  explicit Interface(SDL_Window* window);
  static bool SliderInt(const char* label, int* value, uint32_t min, uint32_t max);

  enum Flags {
    kExport = 1 << 0,
    kProcessAgain  = 1 << 2,
    kShow = 1 << 3,
    kSwapChunks = 1 << 4,
    kLoadCustomFile = 1 << 5,
    kSwapMaps = 1 << 6
  };

  [[nodiscard]] bool FlagSet(Flags flags) const;
  void ToggleFlag(Flags flags);

  void CoreGui();
  RenderState* GetState();

 private:

  bool show;
  uint32_t branch_length_min_;
  uint32_t branch_length_max_;
  uint32_t production_count_min_;
  uint32_t production_count_max_;
  uint32_t branching_angle_min_;
  uint32_t branching_angle_max_;
  uint32_t chunk_index_min_;
  uint32_t chunk_index_max_;
  uint32_t rule_min_;
  uint32_t rule_max_;
  uint32_t root_size_min_;
  uint32_t root_size_max_;
  RenderState state_;
  Flags flags_;
  SDL_Window* window_;
};

#endif  // VOXEL_INTERFACE_H
