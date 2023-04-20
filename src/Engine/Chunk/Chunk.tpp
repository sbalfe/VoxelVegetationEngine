

Chunk::Chunk(uint32_t l, uint32_t w, uint32_t h): chunk_dimensions_ {l,w,h} {
//  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
//    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
//      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
//        AddVoxel({static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)});
//      }
//    }
//  }
}

double Chunk::ConvertIndex(const Position &pos)  const {
  auto [x,y,z] = pos();
  return x + (y * chunk_dimensions_.l_) + (z * chunk_dimensions_.l_ * chunk_dimensions_.w_);
}

Voxel* Chunk::GetVoxel(const Position &pos) {
  auto chunk_index = static_cast<uint32_t>(ConvertIndex(pos));
  return &voxels_[chunk_index];
}


void Chunk::AddVoxel(Position pos) {
  if (pos.x_ < 0 || pos.x_ >= chunk_dimensions_.l_ ||
      pos.y_ < 0 || pos.y_ >= chunk_dimensions_.h_ ||
      pos.z_ < 0 || pos.z_ >= chunk_dimensions_.w_) {
    // position is out of bounds, do not add voxel
    return;
  }
  auto index = static_cast<uint32_t>(ConvertIndex(pos));
  voxels_[index].chunk_position_ = pos;
  voxels_[index].SetActive(true);
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

Voxel* Chunk::operator[](Position index)  {return GetVoxel(index);}

std::array<Voxel,1000000 >& Chunk::GetVoxels () { return voxels_;}