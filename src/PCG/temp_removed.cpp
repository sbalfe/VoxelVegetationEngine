//
// Created by shriller44 on 4/14/23.
//

/*
void Lindenmayer::CreateSphere(Position center) {
  auto distance_calc = [](Position pos, Position center) {
    return sqrt((pos.x() - center.x())
                    * (pos.x() - center.x())
                + (pos.y()- center.y())
                      * (pos.y() - center.y())
                + (pos.z() - center.z())
                      * (pos.z() - center.z()));
  };
  float radius {};
  float side = radius * 0.05f;
  auto start_x = center.x() - side;
  auto start_y = center.y() - side;
  auto start_z = center.z() - side;
  for (float x = center.x() - side; x <=  center.x() + side; x ++) {
    for (float y = center.y() - side; y <= center.y() + side; y ++) {
      for (float z = center.z() - side; z <= center.z() + side; z ++) {
        Position pos = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
        if (distance_calc(pos, center) <= radius) {
          renderer_->AddVoxel(new Voxel{Colour{0,0,1}, pos, indices_});
        }
      }
    }
  }
}



 */