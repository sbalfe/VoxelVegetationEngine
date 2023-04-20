#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl21-cpp"
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
#include <fstream>

//namespace {
//
//  struct Size {
//    uint32_t l;
//    uint32_t w;
//    uint32_t h;
//    [[nodiscard]] constexpr uint32_t size() const {return l * w * h;}
//  };
//  constexpr Size chunkSize{5,5,5};
//  constexpr uint32_t size = chunkSize.size();
//}


class Chunk {
 public:


  struct Dimensions {
    Dimensions(uint32_t l, uint32_t w, uint32_t h): l_ {l}, w_ {w}, h_{h} {}
    static const uint32_t size_ = 1000000   ;
    uint32_t l_;
    uint32_t w_;
    uint32_t h_;
  };

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Voxel;
    using pointer           = Voxel*;
    using reference         = Voxel&;

    explicit Iterator(pointer ptr) : ptr_ { ptr } {}

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }
    Iterator& operator++() { ptr_++; return *this; }

    /* & = can only be applied to L-values */
    Iterator operator++(int) & {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; };

   private:
    pointer ptr_;
  };

  Iterator begin()  { return Iterator(&voxels_[0]); }
  Iterator end()  { return Iterator(&voxels_[Dimensions::size_]); }
  Chunk(uint32_t l, uint32_t w, uint32_t h);
  ~Chunk() = default;
  std::array<Voxel,1000000>& GetVoxels ();
  [[nodiscard]] Voxel* GetVoxel(const Position &pos);
  Voxel* operator[](Position index);
  void AddVoxel(Position pos);

  /* not implemented*/
  void ExportToObj(const std::string& filename);

  std::vector<Position> positions_;
  Dimensions chunk_dimensions_;
 private:
  void GenerateMesh() const;
  [[nodiscard]] double ConvertIndex(const Position& pos) const;
  double voxel_scale_ {0.25};
  std::array<Voxel, 1000000  > voxels_;
};

#endif //VOXEL_CHUNK_H

#pragma clang diagnostic pop

#include "Chunk.tpp"