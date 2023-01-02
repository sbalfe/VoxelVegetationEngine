//
// Created by shriller44 on 12/25/22.
//

#ifndef VOXEL_BLOCK_H
#define VOXEL_BLOCK_H
#include <array>
#include <cstdint>
#include <vector>

// position of the cube
struct Position {
  double x;
  double y;
  double z;
};

// colour of the cube
struct Colour {
  double r;
  double g;
  double b;
};

struct Voxel {
  Colour colour;
  Position position;
  std::vector<int> indices;
  std::array<GLfloat, 108> colour_data{};
  uint32_t vao, ebo, colour_buffer;
};


#endif  // VOXEL_BLOCK_H
