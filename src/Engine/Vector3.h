//
// Created by shriller44 on 4/15/23.
//

#ifndef VOXEL_VECTOR3_H
#define VOXEL_VECTOR3_H

#include <glm/glm.hpp>
#include <cstdint>
#include <tuple>
#include <iostream>
#include <fmt/format.h>

struct Vector3 {
  Vector3() = default;
  Vector3(double x, double y, double z): x_{x}, y_{y}, z_{z}{}

  auto operator()() const{ return std::make_tuple(x_,y_,z_); }

  Vector3 operator/(double value) const { return {x_ / value,  y_ / value, z_ / value} ;}

  friend std::ostream& operator<<(std::ostream& os, const Vector3& rhs){
    os << fmt::format("position: {} | {} | {}\n", rhs.x_, rhs.y_, rhs.z_);
    return os;
  }

  /* accept a glm vector to assign to Position type */
  Vector3& operator=(glm::dvec3&& glm_vec){
    std::cout << "updating values" << std::endl;
    x_ = glm_vec.x;
    y_ = glm_vec.y;
    z_ = glm_vec.z;
  }

  Vector3 Update(double t, Vector3 turtle_direction, Vector3 initial_position){
    x_ = ((turtle_direction.x_ * t)) + initial_position.x_;
    y_ = ((turtle_direction.y_ * t) ) + initial_position.y_;
    z_ = ((turtle_direction.z_ * t)) + initial_position.z_;
    return *this;
  }

  friend bool operator==(const Vector3& lhs, const Vector3& rhs){ return (lhs.x_ == rhs.x_) & (lhs.y_ == rhs.y_) & (lhs.z_ == rhs.z_);}

  explicit operator glm::dvec3() const { return {x_, y_, z_}; }

  Vector3 Floor(double scale){
    x_ = floor(x_ / scale) * scale;
    y_ = floor(y_ / scale) * scale;
    z_ = floor(z_ / scale) * scale;
    return *this;
  }

  double x_;
  double y_;
  double z_;
};

#endif  // VOXEL_VECTOR3_H
