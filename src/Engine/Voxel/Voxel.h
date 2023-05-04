//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_VOXEL_H
#define VOXEL_VOXEL_H

#include "src/Engine/Vector3.h"
#include <GL/glew.h>
#include <array>
#include <iostream>
#include <vector>

struct Voxel {
  // colour of the cube
  struct Colour {
    double r;
    double g;
    double b;
  };

  struct Quad {
    int one;
    int two;
    int three;
    int four;
    int five;
    int six;
  };

  Voxel();
  explicit Voxel(Vector3 chunk_position);
  [[nodiscard]] explicit operator bool() const ;
  void SetActive(bool active);
  void AddQuad(int index){
    Quad q = quads_[index];
    indices_.emplace_back(q.one);
    indices_.emplace_back(q.two);
    indices_.emplace_back(q.three);
    indices_.emplace_back(q.four);
    indices_.emplace_back(q.five);
    indices_.emplace_back(q.six);
  }
  uint32_t vao_, ebo_, vbo_, colour_buffer_;
  std::array<GLdouble , 108> colour_data_;

//  std::vector<uint32_t> indices_ {// front quad
//                                 0,1,3,
//                                 2,3,1,
//                                 // left quad
//                                 2,6,3,
//                                 3,6,7,
//                                 //right quad
//                                 0,1,4,
//                                 5,0,4,
//                                 //bottom quad
//                                 2,6,1,
//                                 4,6,1,
//                                 //top quad
//                                 3,0,7,
//                                 7,5,0,
//                                 //back quad
//                                 7,6,4,
//                                 5,7,4
//  };
  std::vector<uint32_t> indices_;


  std::array<Quad, 6> quads_ {
      Quad{0,1,3,2,3,1}, // front
      Quad{2,6,3,3,6,7}, // left
      Quad{0,1,4,5,0,4}, // right
      Quad{2,6,1,4,6,1}, // bottom
      Quad{3,0,7,7,5,0}, // top
      Quad{7,6,4,5,7,4} // back
  };

  bool leaf_block_ {false};
  Colour colour_;
  Vector3 world_position_;
  Vector3 chunk_position_;
  bool active_;
};

#endif  // VOXEL_VOXEL_H
