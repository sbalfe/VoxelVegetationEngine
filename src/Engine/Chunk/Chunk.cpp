#include "Chunk.h"

Chunk::Chunk(uint32_t size)
    :size_{size}, voxel_octree_{static_cast<int>(size)}{
  voxel_octree_.setEmptyValue(nullptr);
}


bool Chunk::BoundaryCheck(const Vector3& pos) const{
  auto [x,y,z] = pos();
  return !(x < 0 ||
          x >= size_||
          y < 0 ||
          y >= size_ ||
          z < 0 || z>= size_);
}

Voxel* Chunk::GetVoxel(const Vector3 &pos) {
  if (!BoundaryCheck(pos)) return nullptr;
  auto [x,y,z] = pos();
  return voxel_octree_(x,y,z);
}

void Chunk::AddVoxel(Vector3 pos, bool leaf){
  if (!BoundaryCheck(pos)) return;
  auto [x,y,z] = pos();
  voxel_octree_(x,y,z) = new Voxel(pos);
  voxel_octree_(x,y,z)->SetActive(true);
  if (leaf) {
    voxel_octree_(x,y,z)->leaf_block_ = true;
  }
  render_data_.emplace_back(voxel_octree_(x,y,z));
  position_tracker_.emplace_back(new Vector3{pos});
}

void Chunk::WipeVoxels(){
  for (const auto p: position_tracker_){
    auto [x,y,z] = (*p)();
    voxel_octree_(x,y,z) = nullptr;
  }
  render_data_.clear();
  position_tracker_.clear();
}

Voxel* Chunk::operator[](Vector3 index)  {
  auto [x,y,z] = index();
  return voxel_octree_(x,y,z);
}

std::vector<Voxel*> Chunk::GetRenderData(){return render_data_;}
uint32_t Chunk::GetSize() const {return size_; }