////
//// Created by shriller44 on 3/31/23.
////
//
//
//direction_key_map_['A'] = Direction::A;
//direction_key_map_['B'] = Direction::B;
//direction_key_map_['C'] = Direction::C;
//direction_key_map_['D'] = Direction::D;
//direction_key_map_['E'] = Direction::E;
//direction_key_map_['f'] = Direction::F;
//direction_key_map_['G'] = Direction::G;
//direction_key_map_['H'] = Direction::H;
//direction_key_map_['I'] = Direction::I;
//direction_key_map_['J'] = Direction::J;
//direction_key_map_['K'] = Direction::K;
//direction_key_map_['L'] = Direction::L;
//direction_key_map_['M'] = Direction::M;
//direction_key_map_['N'] = Direction::N;
//direction_key_map_['O'] = Direction::O;
//direction_key_map_['P'] = Direction::P;
//direction_key_map_['Q'] = Direction::Q;
//direction_key_map_['R'] = Direction::R;
//direction_key_map_['S'] = Direction::S;
//direction_key_map_['T'] = Direction::T;
//direction_key_map_['U'] = Direction::U;
//direction_key_map_['V'] = Direction::V;
//direction_key_map_['W'] = Direction::W;
//direction_key_map_['X'] = Direction::X;
//direction_key_map_['Y'] = Direction::Y;
//direction_key_map_['Z'] = Direction::Z;
//
//void AdvanceMovement(char key_code){
//  auto direction = direction_key_map_.find(key_code);
//  if (direction == direction_key_map_.end()){
//    return;
//  }
//  switch (direction->second){
//    case Direction::A:
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kUp);
//      turtle_position_.Move(Movement::kForward);
//      break;
//    case Direction::B:
//      turtle_position_.Move(Movement::kForward);
//      turtle_position_.Move(Movement::kUp);
//      break;
//    case Direction::C:
//      turtle_position_.Move(Movement::kRight);
//      turtle_position_.Move(Movement::kForward);
//      turtle_position_.Move(Movement::kUp);
//      break;
//    case Direction::D:
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kUp);
//      break;
//    case Direction::E:
//      turtle_position_.Move(Movement::kUp);
//      break;
//    case Direction::F:
//      turtle_position_.Move(Movement::kUp);
//      turtle_position_.Move(Movement::kRight);
//      break;
//    case Direction::G:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kUp);
//      turtle_position_.Move(Movement::kLeft);
//      break;
//    case Direction::H:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kUp);
//      break;
//    case Direction::I:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kUp);
//      turtle_position_.Move(Movement::kRight);
//      break;
//    case Direction::J:
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kForward);
//      break;
//    case Direction::K:
//      turtle_position_.Move(Movement::kForward);
//      break;
//    case Direction::L:
//      turtle_position_.Move(Movement::kForward);
//      turtle_position_.Move(Movement::kRight);
//      break;
//    case Direction::M:
//      turtle_position_.Move(Movement::kLeft);
//      break;
//    case Direction::N:
//      turtle_position_.Move(Movement::kRight);
//      break;
//    case Direction::O:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kLeft);
//      break;
//    case Direction::P:
//      turtle_position_.Move(Movement::kBackward);
//      break;
//    case Direction::Q:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kRight);
//      break;
//    case Direction::R:
//      turtle_position_.Move(Movement::kDown);
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kForward);
//      break;
//    case Direction::S:
//      turtle_position_.Move(Movement::kForward);
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::T:
//      turtle_position_.Move(Movement::kForward);
//      turtle_position_.Move(Movement::kRight);
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::U:
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::V:
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::W:
//      turtle_position_.Move(Movement::kRight);
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::X:
//      turtle_position_.Move(Movement::kLeft);
//      turtle_position_.Move(Movement::kDown);
//      turtle_position_.Move(Movement::kBackward);
//      break;
//    case Direction::Y:
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kDown);
//      break;
//    case Direction::Z:
//      turtle_position_.Move(Movement::kRight);
//      turtle_position_.Move(Movement::kBackward);
//      turtle_position_.Move(Movement::kDown);
//      break;
//  }
//}