//
// Created by shriller44 on 12/26/22.
//

#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../ResourceManagers/Shader.h"
#include "Camera.h"
#include "Block.h"
#include "../stb_image.h"
#include <fmt/format.h>
#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include "buffer_data.h"

class Renderer {
 public:

  Renderer(uint32_t screen_width, uint32_t screen_height): screen_width_{screen_width}, screen_height_{screen_height}{
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
    last_x_ = static_cast<float>(screen_height_) / 2.0f;
    last_y_ = static_cast<float>(screen_width_) / 2.0f;
    first_mouse_ = true;
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
    /*******************/
  }

  void AddVoxel(Voxel* voxel){
    glGenVertexArrays(1, &voxel->vao);
    glBindVertexArray(voxel->vao);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, shriller::cube_data.size()  * sizeof(float) , shriller::cube_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &voxel->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxel->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxel->indices.size()  * sizeof(int)), voxel->indices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < 108; i+=3){
      voxel->colour_data[i] = voxel->colour.r;
      voxel->colour_data[i+1] = voxel->colour.g;
      voxel->colour_data[i+2] = voxel->colour.b;
    }

    glGenBuffers(1, &voxel->colour_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, voxel->colour_buffer);
    glBufferData(GL_ARRAY_BUFFER, voxel->colour_data.size() * sizeof(float), voxel->colour_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,0, nullptr);
    glEnableVertexAttribArray(1);

    voxels_.emplace_back(voxel);
  }

  void Render(){
    glEnable(GL_DEPTH_TEST);
    auto currentFrame = static_cast<float>(SDL_GetTicks());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_->use();

    glm::mat4 projection = glm::perspective(glm::radians(camera_->Zoom), (float)(screen_width_) / (float)(screen_height_), 0.1f, 100.0f);
    glm::mat4 view = camera_->GetViewMatrix();
    shader_->setMat4("projection", projection);
    shader_->setMat4("view", view);

    for (const auto* v: voxels_){
      glBindVertexArray(v->vao);
      glm::mat4 model = glm::mat4(1.0f);
      glm::vec3 scale = glm::vec3(0.01, 0.01, 0.01);
      model = glm::translate(model, {v->position.x,v->position.y,v->position.z});
      model = glm::scale(model, scale);
      shader_->setMat4("model", model);
      glDrawElements(GL_TRIANGLES, static_cast<int>(v->indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    SDL_GL_SwapWindow(window_);
  }

  void ProcessMouse(float x_pos, float y_pos){

    if (first_mouse_)
    {
      last_x_ = x_pos;
      last_y_ = y_pos;
      first_mouse_ = false;
    }

    float x_offset = x_pos - last_x_;
    float y_offset = last_y_ - y_pos; // reversed since y-coordinates go from bottom to top

    last_x_ = x_pos;
    last_y_ = y_pos;

    camera_->ProcessMouseMovement(x_offset, y_offset);
  }

  void processKeyboard(){
    auto state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_GetScancodeFromKey(SDLK_w)]){
      camera_->ProcessKeyboard(FORWARD, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_s)]){
      camera_->ProcessKeyboard(BACKWARD, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_a)]){
      camera_->ProcessKeyboard(LEFT, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_d)]){
      camera_->ProcessKeyboard(RIGHT, deltaTime);
    }
  }

 private:

  // SDL window context
  SDL_Window *window_;
  uint32_t screen_width_;
  uint32_t screen_height_;

  // SDL GL context
  SDL_GLContext context_;
  int width_;
  int height_;
  int channels_;

  // Buffer identifiers;
  uint32_t vbo_, texture1_;

  // Shader we are using
  Shader *shader_;

  // Camera
  Camera *camera_;
  float last_x_;
  float last_y_;
  bool first_mouse_;

  // timings
  float deltaTime = 0.0f;	// time between current frame and last frame
  float lastFrame = 0.0f;

  // Voxel Data
  std::vector<Voxel*> voxels_;
};

#endif  // VOXEL_RENDERER_H
