//
// Created by shriller44 on 12/26/22.
//

#ifndef VOXEL_BUFFER_DATA_H
#define VOXEL_BUFFER_DATA_H

#include <array>
#include <GL/glew.h>

namespace shriller {

  constexpr std::array<GLfloat, 24> cube_data {
      0.5f, 0.5f, -0.5f, // x1
      0.5f, -0.5f, -0.5f, // x2
      -0.5f, -0.5f, -0.5f, // x3
      -0.5f, 0.5f, -0.5f, // x4
      0.5f, -0.5f, -1.5f, // x5
      0.5f, 0.5f, -1.5f, // x6
      -0.5f, -0.5f, -1.5f, // x7
      -0.5f, 0.5f, -1.5f, // x8
  };

  constexpr std::array<GLfloat, 108> colour_data {
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f
  };
}

#endif  // VOXEL_BUFFER_DATA_H
