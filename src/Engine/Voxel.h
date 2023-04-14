//
// Created by shriller44 on 12/25/22.
//

#ifndef VOXEL_VOXEL_H
#define VOXEL_VOXEL_H
#include <array>
#include <cstdint>
#include <vector>

enum class Movement {
  kLeft,
  kRight,
  kDown,
  kForward,
  kUp,
  kBackward
};

// position of the cube
struct Position {
 private:
  glm::dvec3 internal_vector_;
 public:

  Position(double x, double y, double z): internal_vector_ {x,y,z}{}

  friend std::ostream& operator<<(std::ostream& os, Position& pos){
    os << "x : " << pos.internal_vector_.x << " y: " << pos.internal_vector_.y << " z:" << pos.internal_vector_.z << '\n';
    return os;
  }

  glm::dvec3 operator*(double value) const {
    return {internal_vector_ * value};
  }

  Position operator/(double value) const {
    return {internal_vector_.x / value,  internal_vector_.y / value,   internal_vector_.z / value};
  }

  glm::vec3 operator+(double value) const {
    return {internal_vector_.x + value, internal_vector_.y + value, internal_vector_.z + value};
  }

  void Update(uint32_t t, glm::dvec3 turtle_direction, double scale, glm::dvec3 initial_position){
    internal_vector_ = (((turtle_direction * static_cast<double>(t)) * scale) + initial_position);
  }

  void setY(double val){
    internal_vector_.y = val;
  }

  void setX(double val){
    internal_vector_.x = val;
  }

  void setZ(double val){
    internal_vector_.z = val;
  }

  void floor(double val){
    internal_vector_.x = ::floor(internal_vector_.x / val) * 0.05;
    internal_vector_.y = ::floor(internal_vector_.y / val) * 0.05;
    internal_vector_.z = ::floor(internal_vector_.z / val) * 0.05;
  }

  void Cast(){
    internal_vector_.x = round(internal_vector_.x);
    internal_vector_.y = round(internal_vector_.y);
    internal_vector_.z = round(internal_vector_.z);
  }

  [[nodiscard]] glm::dvec3& get_internal()  {return internal_vector_;}
  [[nodiscard]] double x() const{return internal_vector_.x;}
  [[nodiscard]] double y() const{return internal_vector_.y;}
  [[nodiscard]] double z() const{return internal_vector_.z;}

  glm::dvec3 operator+(const Position& other) const {
    return {internal_vector_.x + other.internal_vector_.x,
            other.internal_vector_.y + other.internal_vector_.y,
            internal_vector_.z + other.internal_vector_.z};
  }

  double increment {1.0};

  void Move(Movement movement){
    switch (movement){
      case Movement::kUp: {
        internal_vector_.y += increment;
        break;
      }
      case Movement::kLeft:
        internal_vector_.x -= increment;
        break;
      case Movement::kRight:
        internal_vector_.x += increment;
        break;
      case Movement::kDown:
        internal_vector_.y -= increment;
        break;
      case Movement::kForward:
        internal_vector_.z -= increment;
        break;
      case Movement::kBackward:
        internal_vector_.z += increment;
        break;
    }
  }
};

// colour of the cube
struct Colour {
  double r;
  double g;
  double b;
};

struct Voxel {
  Colour colour;
  Position position;
  std::vector<uint32_t> indices;
  std::array<GLfloat, 108> colour_data{};
  uint32_t vao, ebo, vbo, colour_buffer;
};


#endif  // VOXEL_VOXEL_H
