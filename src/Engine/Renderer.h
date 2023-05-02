//
// Created by shriller44 on 12/26/22.
//

#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "external/stb_image.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

/* self imports */
#include "src/Engine/Camera/Camera.h"
#include "src/Engine/Chunk/Chunk.h"
#include "src/Engine/Data/buffer_data.h"
#include "src/Engine/GUI/GUI.h"
#include "src/Engine/Shaders/Shader.h"
#include "src/Engine/Voxel/Voxel.h"

class Renderer {

 public:

  Renderer(uint32_t screen_width, uint32_t screen_height, int chunk_size);
  static void FillBuffers(Voxel* voxel);
  void Render(uint32_t chunk_index);
  void ShowGUI();
  void ProcessChunkData(uint32_t chunk_index);
  void HandleKeyboard();
  void ProcessMouse(double x_pos, double y_pos);
  GUI& GetGui();
  void AddChunk(Chunk* chunk, uint32_t chunk_index);
  static void SetRandomColours(std::vector<Voxel*>& chunk_data);

 private:

  /* Camera */
  Camera *camera_;

  /* GUI */
  GUI gui_;
  uint32_t screen_width_;
  uint32_t screen_height_;

  /* Render */
  std::unordered_map<uint32_t,std::vector<Voxel*>> voxels_;
  std::unordered_map<uint32_t, Chunk*> chunks_;
  static constexpr int voxel_scale_ {20};
  float delta_time_;
  float last_frame_;
  uint32_t texture1_;
  Shader *shader_;
  int width_;
  int height_;
  int channels_;

};

#endif  // VOXEL_RENDERER_H

