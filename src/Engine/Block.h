//
// Created by shriller44 on 12/25/22.
//

#ifndef VOXEL_BLOCK_H
#define VOXEL_BLOCK_H
#include <array>
#include <cstdint>
#include <vector>

enum class Movement {
  kLeft,
  kRight,
  kDown,
  kForward,
  kUp,
  kBackward
};

// position of the cube
struct Position {
  double x;
  double y;
  double z;

  friend std::ostream& operator<<(std::ostream& os, Position& pos){
    os << "x : " << pos.x << " y: " << pos.y << " z:" << pos.z << '\n';
    return os;
  }
  double increment {0.01f};
  void move(Movement movement){
    switch (movement){
      case Movement::kUp: {
        y += increment;
        break;
      }
      case Movement::kLeft:
        x -= increment;
        break;
      case Movement::kRight:
        x += increment;
        break;
      case Movement::kDown:
        y -= increment;
        break;
      case Movement::kForward:
        z -= increment;
        break;
      case Movement::kBackward:
        z += increment;
        break;
    }
  }
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
  std::vector<uint32_t> indices;
  std::array<GLfloat, 108> colour_data{};
  uint32_t vao, ebo, colour_buffer;
};


#endif  // VOXEL_BLOCK_H
