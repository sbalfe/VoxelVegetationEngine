//
// Created by shriller44 on 4/15/23.
//

#ifndef VOXEL_POSITION_H
#define VOXEL_POSITION_H

#include <glm/glm.hpp>
#include <cstdint>
#include <tuple>

struct Position {
  auto operator()() const{
    return std::make_tuple(x,y,z);
  }

  Position operator/(double value) const {
    return {x / value,  y / value, z / value};
  }

  void UpdatePosition(glm::dvec3 direction){}

  double x;
  double y;
  double z;
};

#endif  // VOXEL_POSITION_H
