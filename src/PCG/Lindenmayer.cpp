#include <utility>

//
// Created by shriller44 on 1/4/23.
//


Lindenmayer::Lindenmayer()
    : turtle_state_{},
      branch_length_ {10},
      symbol_functions_{},
      ignore_symbols_ {'I', 'p', 'S'}
{
  SetFunctions();
}


void Lindenmayer::SetFunctions(){

  /* place assets */
  symbol_functions_['#'] = [&](){};

  /* branching */
  symbol_functions_[']'] = [&](){turtle_state_ = turtle_states.top();turtle_states.pop();};
  symbol_functions_['['] = [&](){ turtle_states.push(turtle_state_); };

  /* rotations */
  symbol_functions_['^'] = [&](){Rotate(Axis::kX,-1);};
  symbol_functions_['\\'] = [&](){ Rotate(Axis::kY, 1); };
  symbol_functions_['/'] = [&](){ Rotate(Axis::kY,-1); };
  symbol_functions_['&'] = [&](){ Rotate(Axis::kX,1); };
  symbol_functions_['+'] = [&](){ Rotate(Axis::kZ, 1); };
  symbol_functions_['-'] = [&](){ Rotate(Axis::kZ, -1); };
}

void Lindenmayer::InitState(uint32_t chunk_index, std::string axiom){
  chunk_states_[chunk_index] = new LindenmayerState{std::move(axiom), ""};
}

void Lindenmayer::ProcessString(int branch_length, double branching_angle, uint32_t chunk_index) {

  /* obtain a pointer to the current chunk based on the tag*/
  Chunk* current_chunk = plant_chunk_[chunk_index];
  std::string result = chunk_states_[chunk_index]->result_;

  branch_length_ = branch_length;
  branching_angle_ = static_cast<float>(branching_angle);

  /* set initial turtle state*/
  turtle_state_.chunk_voxel_position_ = Position{current_chunk->chunk_dimensions_.l_ / 2.0,
                                                   0,
                                                 current_chunk->chunk_dimensions_.w_ / 2.0};

  turtle_state_.direction_ = Position{0.0,1.0,0.0};

  auto PlaceCube = [&](Position& cube_center, uint32_t cube_size){
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
          Position cube_voxel(i, j, k);
          //plant_chunk_->AddVoxel(cube_voxel);
        }
      }
    }
  };

  std::for_each(std::begin(result), std::end(result), [&](char c){

    /* parse digit characters differently */
    if (isdigit(c)) {

      /* convert char to integer */
      uint32_t dimension_index = c - '0';
      turtle_state_.branch_size_ = dimension_index;
    }
    else {
      if (c == '>'){

        Position initial_position = turtle_state_.chunk_voxel_position_;
        Position current_position = initial_position;

        for (int i = 0; i < branch_length_ ; i++){
          current_chunk->AddVoxel(turtle_state_.chunk_voxel_position_);

          Position current_voxel = turtle_state_.chunk_voxel_position_.Floor(1);
          Position check_voxel_boundary = current_voxel;

          auto CheckNull = [&](Position pos){
            auto fetch = current_chunk->GetVoxel(pos);
            if (fetch == nullptr){
              return false;
            }
            return true;
          };

          while (CheckNull(turtle_state_.chunk_voxel_position_)) {
            for (;;) {

              /* move in the specified direction slightly */
              current_position.Update(0.1, turtle_state_.direction_, initial_position);

              /* update our position to increment from for the next increment */
              initial_position = current_position;

              /* check if the position we just moved to has passed the boundary by seeing if the floored valued has changed*/
              check_voxel_boundary = current_position;
              check_voxel_boundary.Floor(1);
              if (check_voxel_boundary != current_voxel) {
                break;
              }
            }
            turtle_state_.chunk_voxel_position_ = check_voxel_boundary;
          }
        }
      }
      else {
        if (symbol_functions_[c]) symbol_functions_[c]();
      }
    }
  });

  turtle_states.empty();
}

Chunk* Lindenmayer::GetPlantChunk(uint32_t chunk_index) { return plant_chunk_[chunk_index]; }

void Lindenmayer::Rotate(Lindenmayer::Axis axis, float sign) {

  // std::cout << "direction before: " << turtle_state_.direction_ << '\n';
  glm::dmat4 rotation_matrix;

  switch(axis){
    case Axis::kX: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_, glm::vec3{1,0,0});
      break;
    }
    case Axis::kY: {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_ , glm::vec3{0,1,0});
      break;
    }
    case Axis::kZ : {
      rotation_matrix = glm::rotate(glm::mat4(1.0), sign * branching_angle_, glm::vec3{0,0,1});
      break;
    }
  }
  turtle_state_.direction_ = glm::dvec3(rotation_matrix * glm::vec4(turtle_state_.direction_.glm_vector_ ,1.0f));
  turtle_state_.direction_.UpdateGlm();
//  std::cout << "direction after: " << turtle_state_.direction_ << '\n';
}

std::string Lindenmayer::ExecuteProductions(uint32_t production_count, uint32_t chunk_index) {

  auto current_chunk_result = chunk_states_[chunk_index];

  /* restart the process */
  current_chunk_result->result_ = current_chunk_result->axiom_;

  auto select_stochastic_rule = [&](std::vector<Lindenmayer::Rule>& rules){
    double cum_prob = 0.0;
    std::mt19937_64 eng(std::random_device{}());
    std::uniform_real_distribution<double> distr(0, 1);
    auto random_number = distr(eng);
    for (const auto& r: rules){
      cum_prob += r.probability_;
      if (random_number <= cum_prob){
        return r.successor_;
      }
    }
    return std::string{""};
  };

  while (production_count-- != 0) {
    size_t index {0};
    std::string intermediate_string;
    std::for_each(std::begin(current_chunk_result->result_), std::end(current_chunk_result->result_), [&](char c) {
      auto rule_vector = rules_.find(c);
      if (rule_vector != rules_.end()){
        intermediate_string += select_stochastic_rule(rule_vector->second);
      }
      else { intermediate_string += c; }
      index++;
    });
    current_chunk_result->result_ = intermediate_string;
  }
  if (branch_length_ < 1) branch_length_ = 1;
  return current_chunk_result->result_;
}

bool Lindenmayer::AddRuleMap(std::map<char, std::vector<Lindenmayer::Rule>> rule_map){
  rules_ = std::move(rule_map);
  return true;
}

uint32_t Lindenmayer::AddChunk(Chunk* chunk){
  plant_chunk_[chunk_tag_counter_] = chunk;
  return chunk_tag_counter_++;
}


