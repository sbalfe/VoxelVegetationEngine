

Chunk::Chunk(uint32_t l, uint32_t w, uint32_t h): chunk_dimensions_ {l,w,h} {
   std::cout << "NIGGER!" << std::endl;
  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
        std::cout << "NIGGER!" << std::endl;
        AddVoxel({static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)});
      }
    }
  }
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
  std::cout << "NIGGER!" << std::endl;
  //voxels_[static_cast<uint32_t>(ConvertIndex(pos))].chunk_position_ = pos;
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

std::array<Voxel,10000>& Chunk::GetVoxels () {return voxels_;}