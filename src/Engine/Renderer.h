//
// Created by shriller44 on 12/26/22.
//

#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ResourceManagers/Shader.h"
#include "../stb_image.h"
#include "src/Engine/Camera/Camera.h"
#include "src/Engine/Chunk/Chunk.h"
#include "src/Engine/Data/buffer_data.h"
#include "src/Engine/Voxel/Voxel.h"
#include "src/Engine/old/Voxel_old.h"
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

class Renderer {
 public:

  struct RenderState {
    int branching_angle_;
    int production_count_;
    int branch_length_;
  };

  Renderer(uint32_t screen_width, uint32_t screen_height)
      :screen_width_{screen_width},
        screen_height_{screen_height},
        show {true},
        branch_length_ {4},
        production_count_ {4},
        min{},
        max{100},
        delta_time_ {0.0f},
        last_frame_ {0.0f},
        width_ {},height_ {},channels_ {},state_{},texture1_{}
        {
    state_ = RenderState{20, 3, 4};
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
        screen_width_, screen_height_,
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

    SDL_GL_SetSwapInterval(1); // Use VSYNC

    GLenum err;
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to init GLEW\n");
      SDL_GL_DeleteContext(context_);
      SDL_DestroyWindow(window_);
      SDL_Quit();
    }

    /****** Camera ******/
    camera_ = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    /*******************/

    /** Shaders **/
    shader_  = new Shader{"../data/Shaders/basic.vert", "../data/Shaders/basic.frag"};
    /************/

    /******** Textures *******/
    glGenTextures(1, &texture1_);
    glBindTexture(GL_TEXTURE_2D, texture1_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("../data/Textures/container.jpg", &width_, &height_, &channels_, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(window_, SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 330");
    /*******************/
  }

  [[nodiscard]] RenderState GetState() const { return {20, branch_length_, production_count_};}


  static void FillBuffers(std::unique_ptr<Voxel>& voxel){
    glGenVertexArrays(1, &voxel->vao_);
    glBindVertexArray(voxel->vao_);

    glGenBuffers(1, &voxel->vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, voxel->vbo_);

    glBufferData(GL_ARRAY_BUFFER, shriller::cube_data.size()  * sizeof(float) , shriller::cube_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &voxel->ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxel->ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxel->indices_.size()  * sizeof(int)), voxel->indices_.data(), GL_STATIC_DRAW);

    for (int i = 0; i < 108; i+=3) {
      voxel->colour_data_[i] = voxel->colour_.r;
      voxel->colour_data_[i+1] = voxel->colour_.g;
      voxel->colour_data_[i+2] = voxel->colour_.b;
    }

    glGenBuffers(1, &voxel->colour_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, voxel->colour_buffer_);
    glBufferData(GL_ARRAY_BUFFER, voxel->colour_data_.size() * sizeof(float), voxel->colour_data_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,0, nullptr);
    glEnableVertexAttribArray(1);
  }

  void AddVoxel(std::unique_ptr<Voxel>& voxel){
    FillBuffers(voxel);
    voxel->world_position_ = voxel->chunk_position_ / 4;
    voxels_.emplace_back(std::move(voxel));
  }

  void ResetVoxels(){
    for (auto& voxel : voxels_) {
      glDeleteVertexArrays(1, &voxel->vao_);
      glDeleteBuffers(1, &voxel->vbo_);
      glDeleteBuffers(1, &voxel->ebo_);
      glDeleteBuffers(1, &voxel->colour_buffer_);
    }
    voxels_.clear();
  }

  static bool SliderInt(const char* label, int* value, uint32_t min, uint32_t max)
  {
    ImGui::PushID(label);
    ImGui::Text("%s", label);
    ImGui::SameLine();
    bool result = ImGui::SliderInt("", value, static_cast<int>(min), static_cast<int>(max));
    ImGui::PopID();
    return result;
  }

  void ShowGUI(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_);
    ImGui::NewFrame();
    ImGui::Begin("test window");
    if (ImGui::Button("show model")){
      show = !show;
    }
    if (ImGui::Button("re-render")){
      process_again_ = true;
      ResetVoxels();
    }
    if (ImGui::Button("export to OBJ")){
      Export("");
    }
    SliderInt("branch length", &state_.branch_length_, min, max);
    SliderInt("production count", &state_.production_count_, min, max);
  }

  void Export(const std::string& filename){}

  void Render() {
    glEnable(GL_DEPTH_TEST);
    auto currentFrame = static_cast<float>(SDL_GetTicks());
    delta_time_ = currentFrame - last_frame_;
    last_frame_ = currentFrame;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_->use();

    if (show) {
      glm::mat4 projection = glm::perspective(
          glm::radians(camera_->Zoom),
          (double)(screen_width_) / (double)(screen_height_), 0.1, 100.0);
      glm::mat4 view = camera_->GetViewMatrix();

      shader_->setMat4("projection", projection);
      shader_->setMat4("view", view);

      for (const auto& v : voxels_) {
        glBindVertexArray(v->vao_);
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 scale = glm::vec3(0.25, 0.25, 0.25);
        model = glm::translate(model, {v->world_position_.x, v->world_position_.y, v->world_position_.z});
        model = glm::scale(model, scale);
        shader_->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, static_cast<int>(v->indices_.size()),GL_UNSIGNED_INT, nullptr);
      }
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_);
  }

  void ProcessChunk(Chunk& chunk){
    for (auto& voxel: chunk){
      AddVoxel(voxel);
    }
  }

  void HandleKeyboard(){
    auto state = SDL_GetKeyboardState(nullptr);
    camera_->ProcessKeyboard(state, delta_time_);
  }

  void ProcessMouse(double x_pos, double y_pos){
    camera_->ProcessMouse(x_pos, y_pos);
  }

  [[nodiscard]] auto ObtainLength() const{ return branch_length_; }
  [[nodiscard]] auto ObtainCount() const{ return production_count_; }

  bool process_again_ {false};
 private:

  /* Camera */
  Camera *camera_;

  /* SDL */
  SDL_GLContext context_;
  SDL_Window *window_;
  uint32_t screen_width_;
  uint32_t screen_height_;

  /* ImGui */
  bool show;
  int branch_length_;
  int production_count_;
  const uint32_t min;
  const uint32_t max;

  /* Render */
  RenderState state_;
  std::vector<std::unique_ptr<Voxel>> voxels_;
  float delta_time_;
  float last_frame_;
  uint32_t texture1_;
  Shader *shader_;
  int width_;
  int height_;
  int channels_;
};

#endif  // VOXEL_RENDERER_H
