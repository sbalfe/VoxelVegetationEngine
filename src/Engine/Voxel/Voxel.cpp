//
// Created by shriller44 on 4/16/23.
//

#include "Voxel.h"

Voxel::Voxel(Position chunk_position)
    : vao_{},
      ebo_{},
      colour_data_{},
      colour_buffer_{},
      chunk_position_{chunk_position},
      vbo_{},
      active_{true},
      world_position_{},
      colour_{}{}

void Voxel::SetActive(bool active){ active_ = active; }

[[nodiscard]] Voxel::operator bool() const { return active_; }