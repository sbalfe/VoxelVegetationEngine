//
// Created by shriller44 on 12/26/22.
//

#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../stb_image.h"
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
#include <vector>
#include <random>

/* self imports */
#include "src/Engine/Camera/Camera.h"
#include "src/Engine/Chunk/Chunk.h"
#include "src/Engine/Data/buffer_data.h"
#include "src/Engine/Voxel/Voxel.h"
#include "src/Engine/GUI.h"
#include "../ResourceManagers/Shader.h"


class Renderer {

 public:

  Renderer(uint32_t screen_width, uint32_t screen_height);
  static void FillBuffers(Voxel& voxel);
  void AddVoxel(Voxel voxel);
  void ResetVoxels();
  void Render();
  void ShowGUI();
  void ProcessChunk(Chunk& chunk);
  void HandleKeyboard();
  void ProcessMouse(double x_pos, double y_pos);
  GUI& GetGui();
  void Export(const std::string& filename);
  static void SetRandomColours(Chunk& chunk);

 private:
  /* Camera */
  Camera *camera_;

  /* GUI */
  GUI gui_;
  uint32_t screen_width_;
  uint32_t screen_height_;

  /* Render */
  std::vector<Voxel> voxels_;
  float delta_time_;
  float last_frame_;
  uint32_t texture1_;
  Shader *shader_;
  int width_;
  int height_;
  int channels_;
};

#endif  // VOXEL_RENDERER_H

#include "Renderer.tpp"
