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

#include "src/Engine/Position.h"
#include "src/Engine/Voxel/Voxel.h"


class Lindenmayer {
 public:

  enum class Axis { kX, kY, kZ };

  struct BranchDimension {
    auto Get() { return std::make_tuple(length_, width_); }
    uint32_t length_;
    uint32_t width_;
  };

  struct ChunkTag {
    uint32_t id_;
    Chunk* chunk_;
  };

  struct Rule {
    std::string successor_;
    double probability_;
  };

  struct TurtleState {
    Position direction_;
    Position chunk_voxel_position_;
    uint32_t branch_size_;
  };

  struct LindenmayerState {
    std::string axiom_;
    std::string result_;
  };

  Lindenmayer();

  void SetFunctions();

  /*
   * key = symbol
   * rule = expansion for this symbol which main consist of various sets of symbols in your defined alphabet.
   *
   * example:
   *
   * alphabet = {a,b}
   *
   * key = a
   * rule = aab
   * when we read `a`, replace it with aab.
   * */

  void InitState(uint32_t chunk_index, std::string axiom);

  bool AddRuleMap(std::map<char, std::vector<Lindenmayer::Rule>> rule_map);

  /*
   * Apply productions to our `axiom` based on the `rules` we added.
   *
   * param: production_count - number of times to apply the rules to each symbol.
   *
   * returns a string for debugging purposes.
   * */
  std::string ExecuteProductions(uint32_t production_count, uint32_t chunk_index);

  void Rotate(Axis axis, float sign);
  void ProcessString(int branch_length, double branching_angle, uint32_t chunk_tag);
  Chunk* GetPlantChunk(uint32_t chunk_tag);
  uint32_t AddChunk(Chunk* chunk);

 private:

  /* graphics */
  //ChunkTag plant_chunk_;
  std::unordered_map<uint32_t, Chunk*> plant_chunk_;
  uint32_t chunk_tag_counter_;

  /* state */
  TurtleState turtle_state_;
  std::unordered_map<std::uint32_t, BranchDimension> dimensions_map_;
  static constexpr uint32_t dimension_limit_ {6};

  /* parameters */
  double branch_length_;
  float branching_angle_;

  /* L-system */
  //std::map<char, std::vector<Rule>> rules_;
  std::map<char, std::vector<Rule>> rules_;
  std::array<std::function<void()>, 256> symbol_functions_;
  std::array<char, 3> ignore_symbols_;
  std::string axiom_;
  std::string result_;
  std::stack<TurtleState> turtle_states;
  std::map<uint32_t, LindenmayerState*> chunk_states_;

};

#endif  // VOXEL_LINDENMAYER_H

#include "Lindenmayer.cpp"
