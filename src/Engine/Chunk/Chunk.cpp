////
//// Created by shriller44 on 10/9/22.
////
//
//
//template<int size>
//constexpr Chunk<size>::Chunk(uint32_t w, uint32_t l, uint32_t h): chunk_dimensions_(l,w,h) {
//  voxels_.resize(size);
//  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
//    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
//      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
//        AddVoxel({static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)});
//      }
//    }
//  }
//  this->GenerateMesh();
//}
//
//template<int size>
//double Chunk<size>::ConvertIndex(const Position &pos)  const {
//  auto [x,y,z] = pos();
//  return x + (y * chunk_dimensions_.l_) + (z * chunk_dimensions_.l_ * chunk_dimensions_.w_);
//}
//
//
//template<int size>
//Voxel& Chunk<size>::GetVoxel(const Position &pos) const {
//  auto chunk_index = static_cast<uint32_t>(ConvertIndex(pos));
//  //std::cout << fmt::format("chunk index: {}\n", chunk_index);
//  return voxels_[chunk_index];
//}
//
//template<int size>
//void Chunk<size>::AddVoxel(const Position &pos) {
//  voxels_[static_cast<uint32_t>(ConvertIndex(pos))] = std::make_unique<Voxel>(pos);
//}
//
//template<int size>
//void Chunk<size>::GenerateMesh() const {
//  for (uint32_t x = 0; x <  chunk_dimensions_.l_; x++) {
//    for (uint32_t y = 0; y < chunk_dimensions_.w_ ; y++) {
//      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {
//       // if (GetVoxel({x,y,z})){}
//      }
//    }
//  }
//}
//
//template<int size>
//Voxel Chunk<size>::operator[](Position index) const {return GetVoxel(index);}
//
//template<int size>
//std::vector<Voxel>& Chunk<size>::GetVoxels () {return voxels_;}