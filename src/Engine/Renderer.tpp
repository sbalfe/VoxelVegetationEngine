//
// Created by shriller44 on 12/26/22.
//



Renderer::Renderer(uint32_t screen_width, uint32_t screen_height)
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
  gui_.Init();
  ImGui_ImplOpenGL3_Init("#version 330");
  /*******************/
}


void Renderer::FillBuffers(Voxel& voxel){
  glGenVertexArrays(1, &voxel.vao_);
  glBindVertexArray(voxel.vao_);

  glGenBuffers(1, &voxel.vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, voxel.vbo_);

  glBufferData(GL_ARRAY_BUFFER, shriller::cube_data.size()  * sizeof(double) , shriller::cube_data.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &voxel.ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxel.ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxel.indices_.size()  * sizeof(int)), voxel.indices_.data(), GL_STATIC_DRAW);

  for (int i = 0; i < 108; i+=3) {
    voxel.colour_data_[i] = voxel.colour_.r;
    voxel.colour_data_[i+1] = voxel.colour_.g;
    voxel.colour_data_[i+2] = voxel.colour_.b;
  }

  glGenBuffers(1, &voxel.colour_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, voxel.colour_buffer_);
  glBufferData(GL_ARRAY_BUFFER, voxel.colour_data_.size() * sizeof(double), voxel.colour_data_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE,0, nullptr);
  glEnableVertexAttribArray(1);
}

void Renderer::AddVoxel(Voxel voxel){
  voxel.world_position_ = voxel.chunk_position_ / 4;
  FillBuffers(voxel);
  voxels_.emplace_back(voxel);
}

void Renderer::ResetVoxels(){
  for (auto& voxel : voxels_) {
    glDeleteVertexArrays(1, &voxel.vao_);
    glDeleteBuffers(1, &voxel.vbo_);
    glDeleteBuffers(1, &voxel.ebo_);
    glDeleteBuffers(1, &voxel.colour_buffer_);
  }
  voxels_.clear();
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

void Renderer::ProcessChunk(Chunk* chunk){
  SetRandomColours(chunk);
  for (const Voxel& voxel: *chunk){
    if (voxel) {
      AddVoxel(voxel);
    }
  }
}

void Renderer::Render() {
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

    for (const auto& v : voxels_) {
      glBindVertexArray(v.vao_);
      glm::mat4 model = glm::mat4(1.0f);
      glm::vec3 scale = glm::vec3(0.25, 0.25, 0.25);
      model = glm::translate(model, {v.world_position_.x_, v.world_position_.y_, v.world_position_.z_});
      model = glm::scale(model, scale);
      shader_->setMat4("model", model);
      glDrawElements(GL_TRIANGLES, static_cast<int>(v.indices_.size()),GL_UNSIGNED_INT, nullptr);
    }
  }

  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  gui_.SwapWindow();
}


void Renderer::ShowGUI(){gui_.ShowGUI();}


void Renderer::Export(const std::string& filename){}


void Renderer::SetRandomColours(Chunk* chunk){

  auto random_colour = []() -> Voxel::Colour {
    std::mt19937_64 eng(std::random_device{}());
    std::uniform_real_distribution<double> distr(0, 1);
    return {distr(eng) ,distr(eng), distr(eng)};
  };

  for (auto& voxel: *chunk){
    voxel.colour_ = random_colour();
  }
}
