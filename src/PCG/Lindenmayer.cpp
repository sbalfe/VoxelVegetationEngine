#include <utility>

//
// Created by shriller44 on 1/4/23.
//

#include "Lindenmayer.h"

Lindenmayer::Lindenmayer()
    :symbol_functions_{}, chunk_tag_counter_ {0}
{
  SetFunctions();
}


void Lindenmayer::SetFunctions(){

  /* place assets */
  symbol_functions_['#'] = [&](){
    auto [chunk, state, turtle] = (*scenes_[active_chunk_])();
    chunk->AddVoxel(turtle->state_.chunk_voxel_position_, true);
    if (!model.empty()) {
      for (const auto& vox : model) {
        auto turtle_pos = turtle->state_.chunk_voxel_position_;
        turtle_pos.x_ += vox[0];
        turtle_pos.y_ += vox[1];
        turtle_pos.z_ += vox[2];
        chunk->AddVoxel(turtle_pos, true);
      }
    }
  };

  /* branching */
  symbol_functions_[']'] = [&](){
    std::cout << "branching\n";
    auto [chunk, state, turtle] = (*scenes_[active_chunk_])();
    turtle->state_.branch_size_++;
    if (turtle->state_.branch_size_ > turtle->state_.core_size_) turtle->state_.branch_size_ = turtle->state_.core_size_;
    turtle->state_ = turtle->turtle_states_->top();
    turtle->turtle_states_->pop();
  };

  symbol_functions_['['] = [&](){
    auto [chunk, state, turtle] = (*scenes_[active_chunk_])();
    turtle->turtle_states_->push(turtle->state_);
    turtle->state_.branch_size_--;
    if (turtle->state_.branch_size_ < 1) turtle->state_.branch_size_ = 1;
  };

  /* rotations */
  symbol_functions_['^'] = [&](){  Rotate(Axis::kX,-1);};
  symbol_functions_['\\'] = [&](){ Rotate(Axis::kY, 1); };
  symbol_functions_['/'] = [&](){  Rotate(Axis::kY,-1); };
  symbol_functions_['&'] = [&](){  Rotate(Axis::kX,1); };
  symbol_functions_['+'] = [&](){  Rotate(Axis::kZ, 1); };
  symbol_functions_['-'] = [&](){  Rotate(Axis::kZ, -1); };

  symbol_functions_['>'] = [&]() {
    auto [chunk, state, turtle] = (*scenes_[active_chunk_])();
    Vector3 initial_position = turtle->state_.chunk_voxel_position_;
    Vector3 current_position = initial_position;
    for (int i = 0; i < state->branch_length_ ; i++){
      //chunk->AddVoxel(turtle->state_.chunk_voxel_position_, false);
      PlaceCube(turtle->state_.chunk_voxel_position_, turtle->state_.branch_size_, chunk, false);
      Vector3 new_voxel{};
      while (chunk->GetVoxel(turtle->state_.chunk_voxel_position_) != nullptr) {
        for (;;) {
          new_voxel = current_position.Update(0.1, turtle->state_.direction_, initial_position);
          initial_position = new_voxel;
          if (new_voxel.Floor(1) !=  turtle->state_.chunk_voxel_position_.Floor(1)) {
            break;
          }
        }
        turtle->state_.chunk_voxel_position_ = new_voxel;
      }
    }
  };
};

void Lindenmayer::ProcessString(uint32_t chunk_index) {

  active_chunk_ = chunk_index;

  auto [chunk, state, turtle] = (*scenes_[chunk_index])();

  /* reset position and direction */
  turtle->state_.chunk_voxel_position_ = Vector3{chunk->GetSize() / 2.0,
                                        3,
                                                 chunk->GetSize() / 2.0};

  turtle->state_.direction_ = Vector3{0.0,1.0,0.0};

  std::for_each(std::begin(state->result_), std::end(state->result_), [&](char c){
      if (symbol_functions_[c]) {
        symbol_functions_[c]();
      } else {
        std::cout << fmt::format("no function for symbol: {}\n", c);
      }
  });
  while (!turtle->turtle_states_->empty()){
    turtle->turtle_states_->pop();
  }
}

Chunk* Lindenmayer::GetPlantChunk(uint32_t chunk_index) {
  return scenes_[chunk_index]->plant_chunk_;
}

void Lindenmayer::Rotate(Lindenmayer::Axis axis, float sign) {
  auto [chunk, state, turtle] = (*scenes_[active_chunk_])();
  auto branching_angle = static_cast<float>(state->branching_angle_);

  glm::vec3 axis_vec;
  switch(axis){
    case Axis::kX: {
      axis_vec = {1,0,0};
      break;
    }
    case Axis::kY: {
      axis_vec = {0,1,0};
      break;
    }
    case Axis::kZ : {
      axis_vec = {0,0,1};
      break;
    }
  }
  glm::dmat4 rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle, axis_vec);
  turtle->state_.direction_ = rotation_matrix * glm::vec4(glm::dvec3(turtle->state_.direction_),1.0f);
}

std::string Lindenmayer::SelectStochasticRule(std::vector<Lindenmayer::Rule>& rules){
  double cum_prob = 0.0;
  std::mt19937_64 eng(std::random_device{}());
  std::uniform_real_distribution<double> distr(0, 1);
  auto random_number_0_1 = distr(eng);
  for (const auto& r: rules){
    cum_prob += r.probability_;
    if (random_number_0_1 <= cum_prob){
      return r.successor_;
    }
  }
  return std::string{""};
}

std::string Lindenmayer::ExecuteProductions(uint32_t production_count, uint32_t chunk_index) {
  active_chunk_ = chunk_index;
  auto [chunk, state, turtle] = (*scenes_[chunk_index])();
  state->result_ = state->axiom_;
  while (production_count-- != 0) {
    std::string intermediate_string;
    std::for_each(std::begin(state->result_), std::end(state->result_), [&](char c) {
      auto current_rules = rules_[active_rules_];
      auto rule = current_rules.find(c);
      if (rule != current_rules.end()){
        auto [_, rule_vector] = *rule;
        intermediate_string += SelectStochasticRule(rule_vector);
      }
      else {
          intermediate_string += c;
      }
    });
    state->result_ = intermediate_string;
  }
  return state->result_;
}



uint32_t Lindenmayer::AddScene(uint32_t chunk_size, std::string axiom, double branching_angle, uint32_t branch_length, uint32_t root_size){
  scenes_[chunk_tag_counter_] = new Scene{new Chunk{chunk_size},
                                          new State(std::move(axiom), "", branch_length, branching_angle),
                                          new Turtle{
                                              new std::stack<TurtleState>,
                                              TurtleState{{},{}, root_size, root_size}
                                          }
  };
  return chunk_tag_counter_++;
}

uint32_t Lindenmayer::RegisterMap(){return rule_counter_++;}

uint32_t Lindenmayer::SetActiveMap(uint32_t map_index){ active_rules_ = map_index; }

void Lindenmayer::AddRule(char c, const Rule& rule, uint32_t map_index){
  rules_[map_index][c].emplace_back(rule);
}

void Lindenmayer::UpdateChunkState(uint32_t chunk_index, double branching_angle, uint32_t branch_length, std::string axiom, uint32_t root_size){
  Lindenmayer::State* chunk_state = scenes_[chunk_index]->state_;
  Turtle* turtle_state = scenes_[chunk_index]->turtle_;
  chunk_state->branching_angle_ = branching_angle;
  chunk_state->branch_length_ = branch_length;
  chunk_state->axiom_ = std::move(axiom);
  turtle_state->state_.branch_size_ = root_size;
  turtle_state->state_.core_size_ = root_size;
}

void Lindenmayer::PlaceCube(Vector3& cube_center, uint32_t cube_size, Chunk* plant_chunk, bool placing_leaf){
  if (cube_size == 1){
    plant_chunk->AddVoxel(cube_center, placing_leaf);
    return;
  }
  double half_size = cube_size / 2.0;
  double remainder = std::fmod(half_size, 1.0);
  if (remainder == 0.5){floor(half_size);}
  double startX = cube_center.x_ - half_size;
  double endX = startX + cube_size;
  double startY = cube_center.y_ - half_size;
  double endY = startY + cube_size;
  double startZ = cube_center.z_ - half_size;
  double endZ = startZ + cube_size;
  for(int i = static_cast<int>(startX); i <= endX; i++) {
    for(int j = static_cast<int>(startY); j <= endY; j++) {
      for(int k = static_cast<int>(startZ); k <= endZ; k++) {
        Vector3 cube_voxel(i, j, k);
        plant_chunk->AddVoxel(cube_voxel, placing_leaf);
      }
    }
  }
}


