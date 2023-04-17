//
// Created by shriller44 on 10/9/22.
//

#include "src/Engine/Chunk/Chunk.h"

Chunk::Chunk(Chunk::Dimensions dimensions): chunk_dimensions_(dimensions) {
  voxels_.resize(dimensions.size_);
  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
        AddVoxel({static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)});
      }
    }
  }
  this->GenerateMesh();
}

double Chunk::ConvertIndex(const Position &pos)  const {
  auto [x,y,z] = pos();
  return x + (y * chunk_dimensions_.l_) + (z * chunk_dimensions_.l_ * chunk_dimensions_.w_);
}

Voxel const& Chunk::GetVoxel(const Position &pos) const {
  return *voxels_[static_cast<uint32_t>(ConvertIndex(pos))];
}

void Chunk::AddVoxel(const Position &pos) {
  voxels_[static_cast<uint32_t>(ConvertIndex(pos))] = std::make_unique<Voxel>(pos);
}

void Chunk::GenerateMesh() const {
  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
       // if (GetVoxel({x,y,z})){}
      }
    }
  }
}

Voxel Chunk::operator[](Position index) const {return GetVoxel(index);}

std::vector<std::unique_ptr<Voxel>>& Chunk::GetVoxels () {return voxels_;}