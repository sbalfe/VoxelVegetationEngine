//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_VOXEL_H
#define VOXEL_VOXEL_H

#include "src/Engine/Position.h"
#include <GL/glew.h>
#include <array>
#include <iostream>
#include <vector>

class Voxel {
 public:

  // colour of the cube
  struct Colour {
    double r;
    double g;
    double b;
  };

  Voxel();
  explicit Voxel(Position chunk_position);

  [[nodiscard]] explicit operator bool() const ;
  void SetActive(bool active);

  uint32_t vao_, ebo_, vbo_, colour_buffer_;
  std::array<GLdouble , 108> colour_data_;
  std::vector<uint32_t> indices_ {// front quad
                                 0,1,3,
                                 2,3,1,
                                 // left quad
                                 2,6,3,
                                 3,6,7,
                                 //right quad
                                 0,1,4,
                                 5,0,4,
                                 //bottom quad
                                 2,6,1,
                                 4,6,1,
                                 //top quad
                                 3,0,7,
                                 7,5,0,
                                 //back quad
                                 7,6,4,
                                 5,7,4
  };
   Colour colour_;
   Position world_position_;
   Position chunk_position_;
 private:
  bool active_;
};

#endif  // VOXEL_VOXEL_H
