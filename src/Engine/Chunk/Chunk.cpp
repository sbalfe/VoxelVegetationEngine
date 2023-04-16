//
// Created by shriller44 on 10/9/22.
//

#include "src/Engine/Chunk/Chunk.h"

Chunk::Chunk() {
  voxels_.resize(size);
  for (uint32_t x = 0; x <  chunkSize.l; x++) {
    for (uint32_t y = 0; y < chunkSize.w ; y++) {
      for (uint32_t z = 0; z < chunkSize.h; z++) {
        AddVoxel({static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)});
      }
    }
  }
  this->GenerateMesh();
}

double Chunk::ConvertIndex(const Position &pos)  {
  auto [x,y,z] = pos();
  return x + (y * chunkSize.l) + (z * chunkSize.l * chunkSize.w);
}

Voxel const& Chunk::GetVoxel(const Position &pos) const {
  return *voxels_[static_cast<uint32_t>(ConvertIndex(pos))];
}

void Chunk::AddVoxel(const Position &pos) {
  voxels_[static_cast<uint32_t>(ConvertIndex(pos))] = std::make_unique<Voxel>(pos);
}

void Chunk::GenerateMesh() const {
  for (uint32_t x = 0; x <  chunkSize.l; x++) {
    for (uint32_t y = 0; y < chunkSize.w ; y++) {
      for (uint32_t z = 0; z < chunkSize.h; z++) {
       // if (GetVoxel({x,y,z})){}
      }
    }
  }
}

Voxel Chunk::operator[](Position index) const {return GetVoxel(index);}

std::vector<std::unique_ptr<Voxel>>& Chunk::GetVoxels () {return voxels_;}