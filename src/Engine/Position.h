//
// Created by shriller44 on 4/15/23.
//

#ifndef VOXEL_POSITION_H
#define VOXEL_POSITION_H

#include <glm/glm.hpp>
#include <cstdint>
#include <tuple>
#include <iostream>
#include <fmt/format.h>

struct Position {
  Position() = default;
  Position(double x, double y, double z): x_{x}, y_{y}, z_{z}, glm_vector_ {x,y,z}{}

  auto operator()() const{ return std::make_tuple(static_cast<int>(x_),static_cast<int>(y_),static_cast<int>(z_)); }

  Position operator/(double value) const { return {x_ / value,  y_ / value, z_ / value} ;}

  friend std::ostream& operator<<(std::ostream& os, const Position& rhs){
    os << fmt::format("position: {} | {} | {}\n", rhs.x_, rhs.y_, rhs.z_);
    return os;
  }

  /* accept a glm vector to assign to Position type */
  Position& operator=(glm::dvec3&& glm_vec){
    x_ = glm_vec.x;
    y_ = glm_vec.y;
    z_ = glm_vec.z;
  }

  void UpdateGlm(){
    glm_vector_.x = x_;
    glm_vector_.y = y_;
    glm_vector_.z = z_;
  }

  void Update(double t, Position turtle_direction, Position initial_position){
    //std::cout << "before " << *this << '\n';
    x_ = ((turtle_direction.x_ * t)) + initial_position.x_;
    y_ = ((turtle_direction.y_ * t) ) + initial_position.y_;
    z_ = ((turtle_direction.z_ * t)) + initial_position.z_;
    //std::cout << "after " << *this << '\n';
    glm_vector_.x = x_;
    glm_vector_.y = y_;
    glm_vector_.z = z_;
  }

  friend bool operator==(const Position& lhs, const Position& rhs){ return (lhs.x_ == rhs.x_) & (lhs.y_ == rhs.y_) & (lhs.z_ == rhs.z_);}

  Position Floor(double scale){

    x_ = floor(x_ / scale) * scale;
    y_ = floor(y_ / scale) * scale;
    z_ = floor(z_ / scale) * scale;

    glm_vector_.x = x_;
    glm_vector_.y = y_;
    glm_vector_.z = z_;

    return *this;
  }

  double x_;
  double y_;
  double z_;

  glm::dvec3 glm_vector_;
};

#endif  // VOXEL_POSITION_H
