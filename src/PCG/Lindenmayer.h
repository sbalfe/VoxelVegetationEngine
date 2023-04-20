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

#include "src/Engine/Chunk/Position.h"
#include "src/Engine/Voxel/Voxel.h"


class Lindenmayer {
 public:



  enum class Axis { kX, kY, kZ };

  struct BranchDimension {
    auto Get() { return std::make_tuple(length_, width_); }
    uint32_t length_;
    uint32_t width_;
  };

  struct TurtleState {
    Position direction_;
    Position chunk_voxel_position_;
    BranchDimension branch_dimension_;
  };

  Lindenmayer(std::string axiom, Chunk* plant_chunk,
                       Position initial_position);

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
  bool AddRule(char key, std::string&& rule);

  /*
   * Apply productions to our `axiom` based on the `rules` we added.
   *
   * param: production_count - number of times to apply the rules to each symbol.
   *
   * returns a string for debugging purposes.
   * */
  std::string ExecuteProductions(uint32_t production_count);

  void Rotate(Axis axis, float sign);
  void Place(uint32_t t, Position initial_position);
  void ProcessString(int length, Renderer& renderer);
  Chunk* GetPlantChunk();
  //void TestFill();
 // void ShowTurtleVoxel();

 private:

  /* graphics */
  std::vector<uint32_t> indices_;
  Chunk* plant_chunk_;

  /* state */
  TurtleState turtle_state_;
  std::unordered_map<std::uint32_t, BranchDimension> dimensions_map_;
  static constexpr uint32_t dimension_limit_ {2};

  /* parameters */
  double branch_length_ {};
  float branching_angle_ = 18.0f;

  /* L-system*/
  std::map<char, std::string> rules_;
  std::array<std::function<void()>, 256> symbol_functions_;
  std::array<char, 3> ignore_symbols_;
  std::string axiom_;
  std::string result_;
  std::stack<TurtleState> turtle_states;

  /* temp removed variables */
  //std::map<char, Direction> direction_key_map_;
  //int current_colour_ {0};
  //std::uint32_t default_dimension_index_ {0};
};

#endif  // VOXEL_LINDENMAYER_H

#include "Lindenmayer.tpp"


