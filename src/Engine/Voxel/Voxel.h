//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_VOXEL_H
#define VOXEL_VOXEL_H

#include "../Chunk/Position.h"
#include <GL/glew.h>
#include <array>
#include <vector>

class Voxel {
 public:

  // colour of the cube
  struct Colour {
    double r;
    double g;
    double b;
  };

  explicit Voxel(Position chunk_position);

  [[nodiscard]] explicit operator bool() const;
  void SetActive(bool active);

  uint32_t vao_, ebo_, vbo_, colour_buffer_;
  std::array<GLdouble , 108> colour_data_;
  std::vector<uint32_t> indices_;
  Colour colour_;
  Position world_position_;
  Position chunk_position_;
 private:
  bool active_;
};

#endif  // VOXEL_VOXEL_H
