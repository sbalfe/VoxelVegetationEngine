//
// Created by shriller44 on 1/4/23.
//

#ifndef VOXEL_LINDENMAYER_H
#define VOXEL_LINDENMAYER_H

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <functional>

/* self imports */

#include "src/Engine/Chunk/Chunk.h"
#include "src/Engine/Vector3.h"
#include "src/Engine/Voxel/Voxel.h"

class Lindenmayer {
 public:
  enum class Axis { kX, kY, kZ };

  struct Rule {
    std::string successor_;
    double probability_;
  };

  struct TurtleState {
    Vector3 direction_;
    Vector3 chunk_voxel_position_;
    uint32_t branch_size_ = 3;
  };

  struct Turtle {
    std::stack<TurtleState>* turtle_states_;
    TurtleState state_;
  };

  struct State {
    std::string axiom_;
    std::string result_;
    uint32_t branch_length_;
    Voxel::Colour voxel_colour_ {0,1,0};
    double branching_angle_;
  };

  struct Scene {
    Chunk* plant_chunk_;
    State* state_;
    Turtle* turtle_;
    auto operator()(){ return std::make_tuple(plant_chunk_, state_, turtle_);}
  };

  Lindenmayer();
  void SetFunctions();
  static std::string SelectStochasticRule(std::vector<Lindenmayer::Rule>& rules);
  bool AddRuleMap(std::map<char, std::vector<Lindenmayer::Rule>> rule_map);
  std::string ExecuteProductions(uint32_t production_count, uint32_t chunk_index);
  void Rotate(Axis axis, float sign);
  void ProcessString(uint32_t chunk_index);
  Chunk* GetPlantChunk(uint32_t chunk_index);
  uint32_t AddScene(uint32_t chunk_size, std::string axiom, double branching_angle, uint32_t branch_length);
  void UpdateChunkState(uint32_t chunk_index, double branching_angle, uint32_t branch_length);
  void AddRule(char c, const Rule& rule){ rules_[c].emplace_back(rule); }
//  void SetActiveChunk(uint32_t chunk_index){active_chunk_ = chunk_index;}
//  [[nodiscard]] uint32_t GetActiveChunk() const {return active_chunk_;}

  std::vector<std::vector<double>> model;
 private:
  static void PlaceCube(Vector3& cube_center, uint32_t cube_size, Chunk* chunk, bool placing_leaf);

  /* graphics */
  std::unordered_map<uint32_t, Scene*> scenes_;
  uint32_t chunk_tag_counter_;
  uint32_t active_chunk_;
  Vector3 model_position {Vector3{32768 / 2.0,
                                 0,
                                 32768 / 2.0}};

  /* L-system */
  std::map<char, std::vector<Rule>> rules_;
  std::array<std::function<void()>, 256> symbol_functions_;
};

#endif  // VOXEL_LINDENMAYER_H

