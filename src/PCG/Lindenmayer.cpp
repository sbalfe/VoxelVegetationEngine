//
// Created by shriller44 on 1/4/23.
//

#include "Lindenmayer.h"


void Lindenmayer::SetFunctions(){

  /* place assets */
  symbol_functions_['#'] = [&](){};

  /* move turtle and place voxels */
  symbol_functions_['>'] = [&](){
    Position turtle_voxel_position = turtle_state_.chunk_voxel_position_;
    for (uint32_t i {0}; i < branch_length_; ++i){

    }
  };

  /* branching */
  symbol_functions_[']'] = [&](){
    turtle_state_ = turtle_states.top();
    turtle_states.pop();
  };
  symbol_functions_['['] = [&](){ turtle_states.push(turtle_state_); };

  /* rotations */
  symbol_functions_['^'] = [&](){ Rotate(Axis::kX,-1); };
  symbol_functions_['\\'] = [&](){ Rotate(Axis::kY, 1); };
  symbol_functions_['/'] = [&](){ Rotate(Axis::kY,-1); };
  symbol_functions_['&'] = [&](){ Rotate(Axis::kX,1); };
  symbol_functions_['+'] = [&](){ Rotate(Axis::kZ, 1); };
  symbol_functions_['-'] = [&](){ Rotate(Axis::kZ, -1); };
}

void Lindenmayer::ProcessString(int length) {

  branch_length_ = length;

  /* set initial turtle state*/
  turtle_state_.chunk_voxel_position_ = {0,0,0};
  turtle_state_.direction_ = {0,1,0};

  renderer_->ResetVoxels();

  std::for_each(std::begin(result_), std::end(result_), [&](char c){

    /* parse digit characters differently */
    if (isdigit(c)) {

      /* indexes determine the size of the branches generated */
      uint32_t dimension_index = c - '0';

      /* if a number is read that is not in the map set to the smallest possible value */
      if (dimension_index > dimension_limit_) dimension_index = dimension_limit_;
      turtle_state_.branch_dimension_ = dimensions_map_[dimension_index];
    }

    /* run the logic associated with that symbol*/
    symbol_functions_[c]();

  });

  /* wipe the states for the next render*/
  turtle_states.empty();
}

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
  turtle_state_.direction_ = glm::dvec3(rotation_matrix * glm::vec4(turtle_state_.direction_ ,1.0f));
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

void Lindenmayer::ShowTurtleVoxel() {}

void Lindenmayer::TestFill() {

  auto& blocks_ = plant_chunk_.GetVoxels();
  for (const auto& block_ptr: blocks_){
    auto [x,y,z] = block_ptr->Position();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, 1);
    float r = distrib(gen); // generate a random float between 0 and 1
    float g = distrib(gen); // generate a random float between 0 and 1
    float b = distrib(gen); // generate a random float between 0 and 1
    renderer_->AddVoxel();
  }
}


