//
// Created by shriller44 on 12/26/22.
//

#include "Renderer.h"


Renderer::Renderer(uint32_t screen_width, uint32_t screen_height, int chunk_size)
    :screen_width_{screen_width},
      screen_height_{screen_height},
      delta_time_ {0.0f},
      last_frame_ {0.0f},
      width_ {},height_ {},channels_ {},texture1_{},
      gui_ {GUI(static_cast<int>(screen_width), static_cast<int>(screen_height))}
{

  GLenum err;
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to init GLEW\n");
    gui_.Destroy();
  }

  glm::vec3 default_ = glm::vec3(static_cast<double>(chunk_size / 2) / 20,
                                 0.5f,
                                 (static_cast<double>(chunk_size / 2) / 20));

  //glm::vec3 hardcode = glm::vec3(819.14, 0.492, 818.3163);
  /****** Camera ******/
  camera_ = new Camera(default_);
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
  gui_.Init();
  ImGui_ImplOpenGL3_Init("#version 330");
  /*******************/
}

void Renderer::FillBuffers(Voxel* voxel){
  glGenVertexArrays(1, &voxel->vao_);
  glBindVertexArray(voxel->vao_);

  glGenBuffers(1, &voxel->vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, voxel->vbo_);

  glBufferData(GL_ARRAY_BUFFER, shriller::cube_data.size()  * sizeof(double) , shriller::cube_data.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &voxel->ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxel->ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxel->indices_.size()  * sizeof(int)), voxel->indices_.data(), GL_STATIC_DRAW);


  auto random_colour = []() -> Voxel::Colour {
    std::mt19937_64 eng(std::random_device{}());
    std::uniform_real_distribution<double> distr(0, 1);
    return {distr(eng) ,distr(eng), distr(eng)};
  };

  for (int i = 0; i < 108; i+=3) {
    voxel->colour_data_[i] = voxel->colour_.r;
    voxel->colour_data_[i+1] = voxel->colour_.g;
    voxel->colour_data_[i+2] = voxel->colour_.b;
  }

  glGenBuffers(1, &voxel->colour_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, voxel->colour_buffer_);
  glBufferData(GL_ARRAY_BUFFER, voxel->colour_data_.size() * sizeof(double), voxel->colour_data_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE,0, nullptr);
  glEnableVertexAttribArray(1);
}

void Renderer::AddChunk(Chunk* chunk, uint32_t chunk_index){
  chunks_[chunk_index] = chunk;
}

void Renderer::HandleKeyboard(){
  auto state = SDL_GetKeyboardState(nullptr);
  camera_->ProcessKeyboard(state, delta_time_);
}

void Renderer::ProcessMouse(double x_pos, double y_pos){
  camera_->ProcessMouse(x_pos, y_pos);
}

GUI& Renderer::GetGui(){
  return gui_;
}

void Renderer::ProcessChunkData(uint32_t chunk_index){
  auto chunk_data = chunks_[chunk_index]->GetRenderData();
  SetRandomColours(chunk_data);
  for (auto* v : chunk_data){
    auto voxel_chunk_position = v->chunk_position_;
    auto [x,y,z] = voxel_chunk_position();
    auto CheckQuad = [&](Vector3 pos, int index){
      if (chunks_[chunk_index]->GetVoxel(pos) == nullptr){
        v->AddQuad(index);
      }
    };
    CheckQuad(Vector3{x, y+1, z}, 0);
    CheckQuad(Vector3{x,y-1,z}, 1);
    CheckQuad(Vector3{x-1,y,z}, 2);
    CheckQuad(Vector3{x,y-1,z}, 3);
    CheckQuad(Vector3{x, y+1, z}, 4);
    CheckQuad(Vector3{x,y,z-1}, 5);
    v->world_position_ = v->chunk_position_ / 20;
    FillBuffers(v);
  }
}

void Renderer::Render(uint32_t chunk_index) {
  auto render_data = chunks_[chunk_index]->GetRenderData();
  HandleKeyboard();
  ShowGUI();
  glEnable(GL_DEPTH_TEST);
  auto currentFrame = static_cast<float>(SDL_GetTicks());
  delta_time_ = currentFrame - last_frame_;
  last_frame_ = currentFrame;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader_->use();

  if (gui_.FlagSet(Interface::Flags::kShow)) {
    glm::mat4 projection = glm::perspective(
        glm::radians(camera_->Zoom),
        (double)(screen_width_) / (double)(screen_height_), 0.1, 100.0);
    glm::mat4 view = camera_->GetViewMatrix();

    shader_->setMat4("projection", projection);
    shader_->setMat4("view", view);

    for (const auto& v : render_data) {
      glBindVertexArray(v->vao_);
      glm::mat4 model = glm::mat4(1.0f);
      glm::vec3 scale = glm::vec3(0.05, 0.05, 0.05);
      model = glm::translate(model, {v->world_position_.x_, v->world_position_.y_, v->world_position_.z_});
      model = glm::scale(model, scale);
      shader_->setMat4("model", model);
      glDrawElements(GL_TRIANGLES, static_cast<int>(v->indices_.size()),GL_UNSIGNED_INT, nullptr);
    }
  }

  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  gui_.SwapWindow();
}

void Renderer::ShowGUI(){ gui_.ShowGUI(); }


void Renderer::SetRandomColours(std::vector<Voxel*>& chunk_data){

  auto random_colour = []() -> Voxel::Colour {
    std::mt19937_64 eng(std::random_device{}());
    std::uniform_real_distribution<double> distr(0, 1);
    return {distr(eng) ,distr(eng), distr(eng)};
  };

  auto random_brown = []() -> Voxel::Colour {
    std::mt19937_64 eng(std::random_device{}());
    std::uniform_real_distribution<double> distr(0.3, 0.7); // adjust range to control darkness of brown
    double r = distr(eng);
    double g = r * 0.5;
    double b = b * 0.10;
    return {r, g, b};
  };

  for (const auto voxel: chunk_data ){
    if (!voxel->leaf_block_) {
      voxel->colour_ = random_brown();
    }
    else {
      std::cout << "leaf block\n";
      voxel->colour_ = Voxel::Colour {0.0, 1.0, 0.0};
    }
  }
}


