//
// Created by shriller44 on 12/26/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Renderer.h"
#include "PCG/Lindenmayer.h"
#include "fmt/format.h"

void HandleSDLEvent(Renderer& renderer, SDL_Event& event, bool& should_run, bool& escaped){
  ImGui_ImplSDL2_ProcessEvent(&event);
  switch (event.type) {
    case SDL_QUIT: {
      should_run = false;
      SDL_Quit();
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        escaped = !escaped;
      }
    }
    case SDL_MOUSEMOTION: {
      if (!escaped) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        renderer.ProcessMouse(static_cast<float>(x),static_cast<float>(y));
      }
    }
  }
}

int PlantOne(Lindenmayer* l_system) {
  l_system->AddRule('p', Lindenmayer::Rule{"I+[p+]--//[--]I[++]-[p]++p", 0.99});
  l_system->AddRule('p', Lindenmayer::Rule{"I+--//[--]I--[p]++p", 0.01});
  l_system->AddRule('I', Lindenmayer::Rule{">S[//&&#][//^^#]>S", 1.0});
  l_system->AddRule('S',  Lindenmayer::Rule{"S>S", 1.0});
}

void BasicStochastic(Lindenmayer* l_system){
  l_system->AddRule('X', Lindenmayer::Rule{">[-X#]+X", 0.4});
  l_system->AddRule('X', Lindenmayer::Rule{">[+X#]>[-X#]+X", 0.6});
  l_system->AddRule('>', Lindenmayer::Rule{">>", 1.0});
}

void SingleCube(Lindenmayer* l_system){
  l_system->AddRule('x', Lindenmayer::Rule{">>>>", 1.0});
}

void Basic(Lindenmayer* l_system){
  l_system->AddRule('X',  Lindenmayer::Rule("X>>[^>>]>>[&>>X]>>[//>>X]", 1.0));
}

void ModelTest(Lindenmayer* l_system){
  l_system->AddRule('x',  Lindenmayer::Rule("#", 1.0));
}

void ExecuteChunk(uint32_t chunk_index, uint32_t production_count, Lindenmayer* l_system, Renderer& renderer){

  /* generate the string, return output for debugging purposes*/
  auto output_string = l_system->ExecuteProductions(production_count, chunk_index);
  std::cout << fmt::format("chunk: {} | output: {}\n", chunk_index, output_string);

  /* process it which fills our chunk */
  l_system->ProcessString(chunk_index);

  /* store the chunk data with the associated index in the renderer */
  renderer.AddChunk(l_system->GetPlantChunk(chunk_index), chunk_index);

  /* update the buffers of each voxel in the chunk data ready to be rendered*/
  renderer.ProcessChunkData(chunk_index);
}

std::vector<std::vector<double>> ReadVoxFile(const std::string& filename){
  std::string pos;
  std::fstream fs {filename};
  std::vector<std::vector<double>> offsets;
  while(std::getline(fs, pos)){
    std::stringstream ss{ pos};
    std::string c;
    std::vector<double> values;
    while(std::getline(ss, c, ',')){
      int val;
      if (c.size() == 1) {
        val = c[0] - '0';
      }
      else {
        val = std::stoi(c);
      }
      values.emplace_back(val);
    }
    offsets.emplace_back(values);
  }
  return offsets;
}

int main() {
  uint32_t chunk_size = 32768;

  /* setup our renderer*/
  auto renderer = std::make_unique<Renderer>(1000, 600, chunk_size);

  /* setup our L-system*/
  auto* l_system = new Lindenmayer();
  PlantOne(l_system);

  auto chunk1 = l_system->AddScene(chunk_size, "p", 19, 4);
  ExecuteChunk(chunk1, 5, l_system, *renderer);
  //auto chunk2 = l_system->AddScene(chunk_size, "p", 7, 1);
  //ExecuteChunk(chunk2, 5, l_system, *renderer);

  GUI& gui = renderer->GetGui();

  /* default chunk to render */
  uint32_t current_chunk_index = chunk1;

  bool escaped {};

  for (bool should_run = true; should_run;){
    if (gui.FlagSet(Interface::Flags::kProcessAgain)){
      gui.SetFlag(Interface::Flags::kProcessAgain);
      auto [branch_length, production_count, voxel_colour, branching_angle] = gui.GetState();

      /* update the specified chunks state with our new values */
      l_system->UpdateChunkState(current_chunk_index, branching_angle, branch_length);

      /* clear the chunk of all its voxels */
      l_system->GetPlantChunk(current_chunk_index)->WipeVoxels();

      ExecuteChunk(current_chunk_index, production_count, l_system, *renderer);
    }

    if (gui.FlagSet(Interface::Flags::kSwapChunks)){
      gui.SetFlag(Interface::Flags::kSwapChunks);
      auto new_chunk_index = gui.GetChunkIndex();
      current_chunk_index = new_chunk_index;
    }

    if (gui.FlagSet(Interface::Flags::kLoadCustomFile)){
      gui.SetFlag(Interface::Flags::kLoadCustomFile);
      l_system->model = ReadVoxFile("../data/Models/"+gui.GetFileName());
      gui.SetFlag(Interface::Flags::kProcessAgain);
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      HandleSDLEvent(*renderer, event, should_run, escaped);
    }
    renderer->Render(current_chunk_index);
  }
  printf("Exiting...\n");

  return 0;
}