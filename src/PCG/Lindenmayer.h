//
// Created by shriller44 on 1/4/23.
//

#ifndef VOXEL_LINDENMAYER_H
#define VOXEL_LINDENMAYER_H

#include "../Engine/Renderer.h"
#include "../FastNoiseLite-1.0.3/Cpp/FastNoiseLite.h"
#include <fmt/format.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>

class Lindenmayer {
 public:

  struct TurtleState {
    glm::dvec3 direction;
    Position position;
    uint32_t length;
    uint32_t width;
  };

  enum class Axis {
    kX,
    kY,
    kZ
  };

  enum class Direction {
    A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
  };

  struct TreeNode {
    uint32_t length_;
    uint32_t width_;
    Direction direction;
    Colour colour;
    uint32_t order;
    std::vector<TreeNode*> children;
  };

  explicit Lindenmayer(std::string axiom, std::unique_ptr<Renderer>& renderer, std::vector<uint32_t> indices)
      : axiom_ {std::move(axiom)}, turtle_position_{0,0,0}, renderer_{renderer}, indices_{std::move(indices)} {
    {
      direction_key_map_['A'] = Direction::A;
      direction_key_map_['B'] = Direction::B;
      direction_key_map_['C'] = Direction::C;
      direction_key_map_['D'] = Direction::D;
      direction_key_map_['E'] = Direction::E;
      direction_key_map_['f'] = Direction::F;
      direction_key_map_['G'] = Direction::G;
      direction_key_map_['H'] = Direction::H;
      direction_key_map_['I'] = Direction::I;
      direction_key_map_['J'] = Direction::J;
      direction_key_map_['K'] = Direction::K;
      direction_key_map_['L'] = Direction::L;
      direction_key_map_['M'] = Direction::M;
      direction_key_map_['N'] = Direction::N;
      direction_key_map_['O'] = Direction::O;
      direction_key_map_['P'] = Direction::P;
      direction_key_map_['Q'] = Direction::Q;
      direction_key_map_['R'] = Direction::R;
      direction_key_map_['S'] = Direction::S;
      direction_key_map_['T'] = Direction::T;
      direction_key_map_['U'] = Direction::U;
      direction_key_map_['V'] = Direction::V;
      direction_key_map_['W'] = Direction::W;
      direction_key_map_['X'] = Direction::X;
      direction_key_map_['Y'] = Direction::Y;
      direction_key_map_['Z'] = Direction::Z;
      colours_.emplace_back(Colour{1,0,0});
      colours_.emplace_back(Colour{0,1,0});
      colours_.emplace_back(Colour{0,0,1});
      colours_.emplace_back(Colour{0.5451, 0.2706, 0.0745});
    }
  }

  bool AddRule(char key, std::string&& rule){
    if (!rules_.contains(key)) {
      rules_.emplace(key, rule);
      return true;
    }
    return false;
  }

  std::string ExecuteProductions(uint32_t production_count){
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

  void AdvanceMovement(char key_code){
    auto direction = direction_key_map_.find(key_code);
    if (direction == direction_key_map_.end()){
      return;
    }
    switch (direction->second){
      case Direction::A:
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kUp);
        turtle_position_.Move(Movement::kForward);
        break;
      case Direction::B:
        turtle_position_.Move(Movement::kForward);
        turtle_position_.Move(Movement::kUp);
        break;
      case Direction::C:
        turtle_position_.Move(Movement::kRight);
        turtle_position_.Move(Movement::kForward);
        turtle_position_.Move(Movement::kUp);
        break;
      case Direction::D:
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kUp);
        break;
      case Direction::E:
        turtle_position_.Move(Movement::kUp);
        break;
      case Direction::F:
        turtle_position_.Move(Movement::kUp);
        turtle_position_.Move(Movement::kRight);
        break;
      case Direction::G:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kUp);
        turtle_position_.Move(Movement::kLeft);
        break;
      case Direction::H:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kUp);
        break;
      case Direction::I:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kUp);
        turtle_position_.Move(Movement::kRight);
        break;
      case Direction::J:
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kForward);
        break;
      case Direction::K:
        turtle_position_.Move(Movement::kForward);
        break;
      case Direction::L:
        turtle_position_.Move(Movement::kForward);
        turtle_position_.Move(Movement::kRight);
        break;
      case Direction::M:
        turtle_position_.Move(Movement::kLeft);
        break;
      case Direction::N:
        turtle_position_.Move(Movement::kRight);
        break;
      case Direction::O:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kLeft);
        break;
      case Direction::P:
        turtle_position_.Move(Movement::kBackward);
        break;
      case Direction::Q:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kRight);
        break;
      case Direction::R:
        turtle_position_.Move(Movement::kDown);
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kForward);
        break;
      case Direction::S:
        turtle_position_.Move(Movement::kForward);
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::T:
        turtle_position_.Move(Movement::kForward);
        turtle_position_.Move(Movement::kRight);
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::U:
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::V:
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::W:
        turtle_position_.Move(Movement::kRight);
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::X:
        turtle_position_.Move(Movement::kLeft);
        turtle_position_.Move(Movement::kDown);
        turtle_position_.Move(Movement::kBackward);
        break;
      case Direction::Y:
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kDown);
        break;
      case Direction::Z:
        turtle_position_.Move(Movement::kRight);
        turtle_position_.Move(Movement::kBackward);
        turtle_position_.Move(Movement::kDown);
        break;
    }
  }

  static double DistanceFromCenter(Position pos, Position center) {
    return sqrt((pos.x() - center.x())
                    * (pos.x() - center.x())
                + (pos.y()- center.y())
                      * (pos.y() - center.y())
                + (pos.z() - center.z())
                      * (pos.z() - center.z()));
  }

  void CreateSphere(Position center){
    Colour colour{0.0f,1.0f,0.0f};
    float radius {8};
    float side = radius * 0.05f;

    std::cout << "center " << center << '\n';
    std::cout << "side " << side << '\n';

    auto start_x = center.x() - side;
    auto start_y = center.y() - side;
    auto start_z = center.z() - side;
    std::cout << fmt::format("x: {}, y: {}, z: {}\n", start_x, start_y, start_z);


    for (float x = center.x() - side; x <=  center.x() + side; x ++) {
      for (float y = center.y() - side; y <= center.y() + side; y ++) {
        for (float z = center.z() - side; z <= center.z() + side; z ++) {
          Position pos = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
          if (DistanceFromCenter(pos, center) <= radius) {
            renderer_->AddVoxel(new Voxel{colours_[1], pos, indices_});
          }
        }
      }
    }
  }

  void ProcessTier(uint32_t tier){
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

  glm::mat3 GetRotationMatrix(double radians){
    //glm::vec3 val = glm::rotate(glm::mat3(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //return {glm::rotate(glm::mat3(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))};
  }


  void Rotate(Axis axis, float sign) {
    glm::dmat4 rotation_matrix;
    switch(axis){
      case Axis::kX: {
        rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle, glm::vec3{1,0,0});
        break;
      }
      case Axis::kY: {
        rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle , glm::vec3{0,1,0});
        break;
      }
      case Axis::kZ : {
        rotation_matrix = glm::rotate(glm::mat4(1.0), sign * angle, glm::vec3{0,0,1});
        break;
      }
    }
    turtle_direction_ = glm::dvec3(rotation_matrix * glm::vec4(turtle_direction_ ,1.0f));
  }

  void Place(uint32_t t, glm::dvec3 initial_position){
    turtle_position_.Update(t, turtle_direction_, 1.0, initial_position);
    //turtle_position_.floor();
    turtle_position_.Cast();
    auto old_position = turtle_position_;
    for (auto x {0}; x < length_; ++x) {
      auto store = turtle_position_;
      for (auto y {0}; y < width_; ++y) {
        renderer_->AddVoxel(new Voxel{colours_[3], turtle_position_, indices_});
        turtle_position_.Move(Movement::kRight);
      }
      store.Move(Movement::kForward);
      turtle_position_ = store;
    }
    turtle_position_ = old_position;
    current_colour_++;
    current_colour_ %= 3;
  }

  void ProcessString(int length){
    std::cout << "processing string" << std::endl;
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
 private:
  double branch_length_ {10};
  glm::dvec3 turtle_direction_ {0,1,0};
  Position turtle_position_;
  uint32_t length_{};
  uint32_t width_{};
  std::string result_;
  std::map<char, std::string> rules_;
  std::map<char, Direction> direction_key_map_;
  float angle = 18.0f;
  std::unique_ptr<Renderer>& renderer_;
  std::vector<uint32_t> indices_;
  std::vector<Colour> colours_;
  int current_colour_ {0};
  std::string axiom_;
};

#endif  // VOXEL_LINDENMAYER_H
