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

void Chunk::ExportToObj(const std::string& filename) {
  std::ofstream out(filename);

  if (!out) {
    std::cerr << "Cannot open file " << filename << std::endl;
    return;
  }

  out << "# Exported from voxel chunk" << std::endl;

  // Write vertex data
  for (auto& voxel : voxels_) {
    const Position& pos = voxel->chunk_position_;
    out << "v " << pos.x << " " << pos.y << " " << pos.z << std::endl;
  }

  // Write face data
  uint32_t index_offset = 1;
  for (uint32_t x = 0; x < chunk_dimensions_.l_ - 1; x++) {
    for (uint32_t y = 0; y < chunk_dimensions_.w_ - 1; y++) {
      for (uint32_t z = 0; z < chunk_dimensions_.h_ - 1; z++) {
        Voxel const& voxel = GetVoxel({x, y, z});
        if (voxel) {
          uint32_t voxel_index = static_cast<uint32_t>(ConvertIndex({x, y, z}));
          out << "f " << voxel_index + index_offset << " " << voxel_index + chunk_dimensions_.size_ + index_offset << " " << voxel_index + chunk_dimensions_.size_ + 1 + index_offset << " " << voxel_index + 1 + index_offset << std::endl;
        }
      }
    }
  }
}


Voxel Chunk::operator[](Position index) const {return GetVoxel(index);}

std::vector<std::unique_ptr<Voxel>>& Chunk::GetVoxels () {return voxels_;}