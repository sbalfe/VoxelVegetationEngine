//
// Created by shriller44 on 1/4/23.
//


Lindenmayer::Lindenmayer(std::string axiom,
                               Chunk* plant_chunk_dimensions,
                               Position initial_position)
    : turtle_state_ {{0,1,0}, initial_position, {2,2} },
      branch_length_ {10},
      symbol_functions_{},
      dimensions_map_{{0, BranchDimension {7,7}}, {1, BranchDimension{4,4}}, {2, BranchDimension{3,3}}} ,axiom_ {std::move(axiom)},
      plant_chunk_ {plant_chunk_dimensions},
      ignore_symbols_ {'I', 'p', 'S'}
{
  SetFunctions();
}


void Lindenmayer::SetFunctions(){
  /* place assets */
  symbol_functions_['#'] = [&](){};
  /* branching */
  symbol_functions_[']'] = [&](){turtle_state_ = turtle_states.top();turtle_states.pop();};
  symbol_functions_['['] = [&](){ turtle_states.push(turtle_state_); };
  /* rotations */
  symbol_functions_['^'] = [&](){
    std::cout << "test" << std::endl;
    Rotate(Axis::kX,-1);
  };
  symbol_functions_['\\'] = [&](){ Rotate(Axis::kY, 1); };
  symbol_functions_['/'] = [&](){ Rotate(Axis::kY,-1); };
  symbol_functions_['&'] = [&](){ Rotate(Axis::kX,1); };
  symbol_functions_['+'] = [&](){ Rotate(Axis::kZ, 1); };
  symbol_functions_['-'] = [&](){ Rotate(Axis::kZ, -1); };
}


void Lindenmayer::ProcessString(int length, Renderer& renderer) {

    branch_length_ = length;
    turtle_state_.chunk_voxel_position_ = Position {0,0,0};

  /* set initial turtle state*/
//  turtle_state_.chunk_voxel_position_ = Position{plant_chunk_->chunk_dimensions_.l_ / 2.0,
//                                                   0,
//                                                   plant_chunk_->chunk_dimensions_.w_ / 2.0};

  turtle_state_.direction_ = Position{0.0,1.0,0.0};

  renderer.ResetVoxels();

  std::for_each(std::begin(result_), std::end(result_), [&](char c){

    /* parse digit characters differently */
    if (isdigit(c)) {

      /* indexes determine the size of the branches generated */
      uint32_t dimension_index = c - '0';

      /* if a number is read that is not in the map set to the smallest possible value */
      if (dimension_index > dimension_limit_) dimension_index = dimension_limit_;
      turtle_state_.branch_dimension_ = dimensions_map_[dimension_index];
    }
    else {

      if (c == '>'){


        Position initial_position = turtle_state_.chunk_voxel_position_;
        Position current_position = turtle_state_.chunk_voxel_position_;

        for (int i = 0; i < 3; i++){
          plant_chunk_->AddVoxel(turtle_state_.chunk_voxel_position_);
          //plant_chunk_->GetVoxel(turtle_state_.chunk_voxel_position_)->SetActive(true);

          Position current_voxel = turtle_state_.chunk_voxel_position_.Floor(1);
          Position check_voxel_boundary = current_voxel;

          for (;;) {


            if (check_voxel_boundary != current_voxel) {
              break;
            }
            current_position.Update(0.1, turtle_state_.direction_, initial_position);
            initial_position = current_position;
            check_voxel_boundary = current_position;
            check_voxel_boundary.Floor(1);
          }

          turtle_state_.chunk_voxel_position_ = check_voxel_boundary;
        }
      }
      else {
        // std::cout << fmt::format("character: {}\n", c);
        /* run the logic associated with that symbol*/
        if (symbol_functions_[c]) symbol_functions_[c]();
      }
    }
  });

  /* wipe the states for the next render*/
  //turtle_states.empty();
}

Chunk* Lindenmayer::GetPlantChunk() { return plant_chunk_; }

/* Figure out the voxels to draw*/
void Lindenmayer::Place(uint32_t t, Position initial_position) {
  auto old_position = turtle_state_.chunk_voxel_position_;
  auto [branch_width, branch_length] = turtle_state_.branch_dimension_.Get();
}

void Lindenmayer::Rotate(Lindenmayer::Axis axis, float sign) {

  glm::dmat4 rotation_matrix;

  switch(axis){
    case Axis::kX: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_, glm::vec3{1,0,0});
      break;
    }
    case Axis::kY: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_ , glm::vec3{0,1,0});
      break;
    }
    case Axis::kZ : {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_, glm::vec3{0,0,1});
      break;
    }
  }
  turtle_state_.direction_ = glm::dvec3(rotation_matrix * glm::vec4(turtle_state_.direction_.glm_vector_ ,1.0f));
  turtle_state_.direction_.UpdateGlm();
}


std::string Lindenmayer::ExecuteProductions(uint32_t production_count) {
  result_ = axiom_;

  while (production_count-- != 0) {
    branch_length_ *= 0.80;
    size_t index {0};
    std::string intermediate_string;
    std::for_each(std::begin(result_), std::end(result_), [&](char c) {

      auto production = rules_.find(c);
      if (production != rules_.end()) {intermediate_string += production->second;}
      else {intermediate_string += c;}

      index++;
    });
    result_ = intermediate_string;
  }

  if (branch_length_ < 1) branch_length_ = 1;

  return result_;
}

bool Lindenmayer::AddRule(char key, std::string&& rule){
  if (!rules_.contains(key)) {
    rules_.emplace(key, rule);
    return true;
  }
  return false;
}

//template <int size>
//void Lindenmayer<size>::ShowTurtleVoxel() {}

