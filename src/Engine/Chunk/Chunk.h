//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "Position.h"
#include "src/Engine/Voxel/Voxel.h"
#include <GL/glew.h>
#include <fmt/format.h>
#include <array>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace {

  struct Size {
    uint32_t l;
    uint32_t w;
    uint32_t h;
    [[nodiscard]] constexpr uint32_t size() const {return l * w * h;}
  };
  constexpr Size chunkSize{100,100,20};
  constexpr uint32_t size = chunkSize.size();
}

class Chunk {
 public:
  Chunk();
  ~Chunk() = default;
  std::vector<std::unique_ptr<Voxel>>& GetVoxels ();
  [[nodiscard]] Voxel const& GetVoxel(const Position &pos) const;
  Voxel operator[](Position index) const;

  /* add later*/
 //  void Update(float dt);
 //  void Render();
 private:
  void GenerateMesh() const;
  static double ConvertIndex(const Position& pos);
  void AddVoxel(Position const& pos);
  std::vector<std::unique_ptr<Voxel>> voxels_{};
};

#endif //VOXEL_CHUNK_H
