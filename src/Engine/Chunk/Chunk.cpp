#include "Chunk.h"

Chunk::Chunk(uint32_t l, uint32_t w, uint32_t h)
    :chunk_dimensions_ {l,w,h},
      voxel_octree_(static_cast<int>(l)){
  voxel_octree_.setEmptyValue(nullptr);
  //FillEdges();
}

double Chunk::ConvertIndex(const Position& pos)  const {
  auto [x,y,z] = pos();
  return x + (y * chunk_dimensions_.l_) + (z * chunk_dimensions_.l_ * chunk_dimensions_.w_);
}

void Chunk::FillEdges() {

  // Iterate through all voxels in the chunk
  for (uint32_t x = 0; x < chunk_dimensions_.l_; x++) {
    for (uint32_t y = 0; y < chunk_dimensions_.w_; y++) {
      for (uint32_t z = 0; z < chunk_dimensions_.h_; z++) {

        // Check if the voxel is within 3 blocks of a corner of the chunk
        if ((x <= 2 || x >= chunk_dimensions_.l_ - 3) &&
            (y <= 2 || y >= chunk_dimensions_.w_ - 3) &&
            (z <= 2 || z >= chunk_dimensions_.h_ - 3)) {
          // If so, create a new Voxel at that position and mark it as active
          Position pos{static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};
          position_tracker_.emplace_back(new Position{pos});
          voxel_octree_(x, y, z) = new Voxel(pos);
          voxel_octree_(x, y, z)->SetActive(true);
        }
      }
    }
  }
}

bool Chunk::BoundaryCheck(const Position& pos) const{
  return !(pos.x_ < 0 ||
          pos.x_ >= chunk_dimensions_.h_ ||
          pos.y_ < 0 ||
          pos.y_ >= chunk_dimensions_.l_ ||
          pos.z_ < 0 || pos.z_ >= chunk_dimensions_.w_);
}

bool Chunk::CheckActive(Position &pos){
  if (!BoundaryCheck(pos)) return false;
  auto [x,y,z] = pos();
  return voxel_octree_(x,y,z);
}

Voxel* Chunk::GetVoxel(const Position &pos) {
  if (!BoundaryCheck(pos)) return nullptr;
  auto [x,y,z] = pos();
  return voxel_octree_(x,y,z);
}

void Chunk::AddVoxel(Position pos){
  if (!BoundaryCheck(pos)) return;
  auto [x,y,z] = pos();
  voxel_octree_(x,y,z) = new Voxel(pos);
  voxel_octree_(x,y,z)->SetActive(true);
  render_data_.emplace_back(voxel_octree_(x,y,z));
  position_tracker_.emplace_back(new Position{pos});
}

void Chunk::WipeVoxels(){
  for (const auto p: position_tracker_){
    auto [x,y,z] = (*p)();
    voxel_octree_(x,y,z) = nullptr;
  }
  render_data_.clear();
  position_tracker_.clear();
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

Voxel* Chunk::operator[](Position index)  {
  auto [x,y,z] = index();
  return voxel_octree_(x,y,z);
}

std::vector<Voxel*>& Chunk::GetVoxels () {
  // return voxels_;
}