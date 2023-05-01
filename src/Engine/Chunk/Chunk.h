#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl21-cpp"
//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "src/Engine/Position.h"
#include "src/Engine/Voxel/Voxel.h"
#include <GL/glew.h>
#include <fmt/format.h>
#include <octree/octree.h>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>


class Chunk {
 public:
  explicit Chunk(uint32_t size);
  ~Chunk() = default;
  [[nodiscard]] bool BoundaryCheck(const Position& pos) const;
  [[nodiscard]] Voxel* GetVoxel(const Position &pos);
  Voxel* operator[](Position index);
  void AddVoxel(Position pos);
  void WipeVoxels();
  std::vector<Voxel*> GetRenderData();
  [[nodiscard]] uint32_t GetSize() const;
 private:
  uint32_t size_;
  std::vector<Voxel*> render_data_;
  std::vector<Position*> position_tracker_;
  Octree<Voxel*> voxel_octree_;
};

#endif //VOXEL_CHUNK_H

#pragma clang diagnostic pop
