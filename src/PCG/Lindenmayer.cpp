//
// Created by shriller44 on 1/4/23.
//

#include "Lindenmayer.h"

void Lindenmayer::ProcessString(int length) {

  branch_length_ = length;
  turtle_position_ = {0,0,0};
  turtle_direction_ = {0,1,0};
  renderer_->ResetVoxels();

  std::stack<TurtleState> turtle_state;

  std::for_each(std::begin(result_), std::end(result_), [&](char c){
    if (isdigit(c)){
      ProcessTier(c - '0');
    }
    else if (c == '#'){
      CreateSphere(turtle_position_);
    }
    else if (c == '['){
      turtle_state.push(TurtleState{turtle_direction_, turtle_position_, length_, width_});
    }
    else if (c == ']'){
      turtle_position_ = turtle_state.top().position;
      turtle_direction_ = turtle_state.top().direction;
      length_ = turtle_state.top().length;
      width_ = turtle_state.top().width;
      turtle_state.pop();
    }
    else if (c == '+'){
      Rotate(Axis::kZ, 1);
    }
    else if (c == '-'){
      Rotate(Axis::kZ, -1);
    }
    else if (c == '&'){
      Rotate(Axis::kX,1);
    }
    else if (c == '^'){
      Rotate(Axis::kX,-1);
    }
    else if (c == '\\') {
      Rotate(Axis::kY, 1);
    }
    else if (c == '/'){
      Rotate(Axis::kY,-1);
    }

    // figure out where we are in our grid for every increment forward
    else if (c == '>'){
      Position initial_position = turtle_position_;
      for (uint32_t i {0}; i < branch_length_; ++i){
        auto position_before = turtle_position_;
        Place(i, initial_position.get_internal());
        std::cout.precision(std::numeric_limits<double>::digits10 + 1);
      }
    }
  });
}

void Lindenmayer::Place(uint32_t t, glm::dvec3 initial_position) {
  turtle_position_.Update(t, turtle_direction_, 1.0, initial_position);
  //turtle_position_.floor();
  turtle_position_.Cast();
  auto old_position = turtle_position_;
  for (auto x {0}; x < length_; ++x) {
    auto store = turtle_position_;
    for (auto y {0}; y < width_; ++y) {
      renderer_->AddVoxel(new Voxel{Colour{0,1,0}, turtle_position_, indices_});
      turtle_position_.Move(Movement::kRight);
    }
    store.Move(Movement::kForward);
    turtle_position_ = store;
  }
  turtle_position_ = old_position;
  current_colour_++;
  current_colour_ %= 3;
}

void Lindenmayer::Rotate(Lindenmayer::Axis axis, float sign) {
  glm::dmat4 rotation_matrix;
  switch(axis){
    case Axis::kX: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle_, glm::vec3{1,0,0});
      break;
    }
    case Axis::kY: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle_ , glm::vec3{0,1,0});
      break;
    }
    case Axis::kZ : {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle_, glm::vec3{0,0,1});
      break;
    }
  }
  turtle_direction_ = glm::dvec3(rotation_matrix * glm::vec4(turtle_direction_ ,1.0f));
}

void Lindenmayer::ProcessTier(uint32_t tier) {
  switch(tier){
    case 0: {
      length_ =  width_ =  7;
      break;
    }
    case 1: {
      length_ = width_ =  4;

      break;
    }
    case 2: {
      length_ = width_ = 3;

      break;
    }
    default : {
      length_ = width_ = 1;
      break;
    }
  }
}

void Lindenmayer::CreateSphere(Position center) {
  auto distance_calc = [](Position pos, Position center) {
    return sqrt((pos.x() - center.x())
                    * (pos.x() - center.x())
                + (pos.y()- center.y())
                      * (pos.y() - center.y())
                + (pos.z() - center.z())
                      * (pos.z() - center.z()));
  };
  float radius {8};
  float side = radius * 0.05f;
  auto start_x = center.x() - side;
  auto start_y = center.y() - side;
  auto start_z = center.z() - side;
  for (float x = center.x() - side; x <=  center.x() + side; x ++) {
    for (float y = center.y() - side; y <= center.y() + side; y ++) {
      for (float z = center.z() - side; z <= center.z() + side; z ++) {
        Position pos = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
        if (distance_calc(pos, center) <= radius) {
          renderer_->AddVoxel(new Voxel{Colour{0,0,1}, pos, indices_});
        }
      }
    }
  }
}

std::string Lindenmayer::ExecuteProductions(uint32_t production_count) {
  result_ = axiom_;
  while (production_count-- != 0) {
    branch_length_ *= 0.80;
    size_t index {0};
    std::string intermediate_string;
    std::for_each(std::begin(result_), std::end(result_), [&](char c) {

      auto production = rules_.find(c);
      if (production != rules_.end()) {
        intermediate_string += production->second;
      }
      else {
        intermediate_string += c;
      }
      index++;
    });
    result_ = intermediate_string;
  }

  if (branch_length_ < 1){
    branch_length_ = 1;
  }
  return result_; // return for debugging purposes
}

bool Lindenmayer::AddRule(char key, std::string&& rule){
  if (!rules_.contains(key)) {
    rules_.emplace(key, rule);
    return true;
  }
  return false;
}


