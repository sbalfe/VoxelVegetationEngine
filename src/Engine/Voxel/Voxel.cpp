//
// Created by shriller44 on 4/16/23.
//

#include "Voxel.h"

Voxel::Voxel()
//      vao_{},ebo_{},vbo_{},
//      colour_data_{},
//      active_{false},
//      colour_buffer_{},
//      colour_{1,1,0}
{ std::cout << "default voxel!" << std::endl;}

Voxel::Voxel(Position chunk_position)
//    : vao_{},ebo_{},vbo_{},
//      colour_data_{},
//      colour_buffer_{},
//      active_{false},
//      world_position_{0,0,0},
//      colour_{1,1,0}
{ std::cout << "default chunk set!" << std::endl;}

//void Voxel::SetActive(bool active){ active_ = active; }

 //[[nodiscard]] Voxel::operator bool() const  { return active_; }