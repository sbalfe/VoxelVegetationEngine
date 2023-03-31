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


  struct TreeNode {
    uint32_t length_;
    uint32_t width_;
    //Direction direction;
    Colour colour;
    uint32_t order;
    std::vector<TreeNode*> children;
  };

  explicit Lindenmayer(std::string axiom, std::unique_ptr<Renderer>& renderer, std::vector<uint32_t> indices)
      : axiom_ {std::move(axiom)}, turtle_position_{0,0,0}, renderer_{renderer}, indices_{std::move(indices)} {}

  bool AddRule(char key, std::string&& rule);

  std::string ExecuteProductions(uint32_t production_count);

  void CreateSphere(Position center);

  void ProcessTier(uint32_t tier);

  void Rotate(Axis axis, float sign);

  void Place(uint32_t t, glm::dvec3 initial_position);

  void ProcessString(int length);

 private:
  double branch_length_ {10};
  glm::dvec3 turtle_direction_ {0,1,0};
  Position turtle_position_;
  uint32_t length_{};
  uint32_t width_{};
  std::string result_;
  std::map<char, std::string> rules_;
  //std::map<char, Direction> direction_key_map_;
  float angle = 18.0f;
  std::unique_ptr<Renderer>& renderer_;
  std::vector<uint32_t> indices_;
  int current_colour_ {0};
  std::string axiom_;
};

#endif  // VOXEL_LINDENMAYER_H
