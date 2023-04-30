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

//int PlantOne(Lindenmayer* l_system){
//  l_system->AddRule('p', Lindenmayer::Rule{"I+[p+#]--//[--#]I[++#]-[p#]++p#", 1.0});
//  l_system->AddRule('I', Lindenmayer::Rule{">S[//&&#][//^^#]>S", 1.0});
//  l_system->AddRule('S', Lindenmayer::Rule{"S>S",1.0});
//}
//
//int PlantTwo(Lindenmayer* l_system){
//
//  std::vector<Lindenmayer::Rule> X_rules {{}, {}}
//
//  l_system->AddRule('A', Lindenmayer::Rule{"[&>LA]/////[&>LA]//////[&>LA]", 1.0});
//  l_system->AddRule('>', Lindenmayer::Rule{"S/////>}", 1.0});
//  l_system->AddRule('S', Lindenmayer::Rule{">L",1.0});
//  l_system->AddRule('L', Lindenmayer::Rule{"#", 1.0});
//}

//int StochasticTest(Lindenmayer* l_system){
//  l_system->AddRule('p', Lindenmayer::Rule{"5>>>>[+p>>][-p>>>]>>>>>[&>p]>>>[//>p]", 1.0});
//  l_system->AddRule('c', Lindenmayer::Rule{"2>>x>>", 1.0});
//  l_system->AddRule('x', Lindenmayer::Rule{"2x>x",1.0});
//}

void BasicStochastic(Lindenmayer* l_system){

  std::map<char, std::vector<Lindenmayer::Rule>> rule_map {
    {'X', { Lindenmayer::Rule{">[-X]+X", 0.4},
             Lindenmayer::Rule{">[+X]>[-X]+X", 0.6}}},
    {'>', {Lindenmayer::Rule{">>",1.0}}}
  };
  l_system->AddRuleMap(rule_map);
}


void Basic(Lindenmayer* l_system){
  std::map<char, std::vector<Lindenmayer::Rule>> rule_map {
      {'X', {Lindenmayer::Rule{">>>>", 1.0}}}
  };
  l_system->AddRuleMap(rule_map);
}

int main() {
  uint32_t chunk_size = 32768;

  auto renderer = std::make_unique<Renderer>(1000, 600, chunk_size);

  auto* l_system = new Lindenmayer();
  auto chunk1 = l_system->AddChunk(new Chunk {chunk_size, chunk_size, chunk_size});
  auto chunk2 = l_system->AddChunk(new Chunk {chunk_size, chunk_size, chunk_size});

  BasicStochastic(l_system);
  l_system->InitState(chunk1, "X");
  l_system->InitState(chunk2, "X");
  auto output = l_system->ExecuteProductions(5, chunk1);

  /* branch length: 5, branch angle: 20 */
  l_system->ProcessString(5, 20, chunk1);
  renderer->UpdateChunkData(l_system->GetPlantChunk(chunk1)->render_data_, chunk1);
  renderer->ProcessChunkData(chunk1);

  auto output2 = l_system->ExecuteProductions(5, chunk2);
  l_system->ProcessString(5, 20, chunk2);
  renderer->UpdateChunkData(l_system->GetPlantChunk(chunk2)->render_data_, chunk2);
  renderer->ProcessChunkData(chunk2);

  auto& gui = renderer->GetGui();
  uint32_t current_chunk_index = chunk1;

  bool escaped {};

  for (bool should_run = true; should_run;){
    if (gui.FlagSet(Interface::Flags::kProcessAgain)){
      std::cout << "re-render has been called\n";
      gui.SetFlag(Interface::Flags::kProcessAgain);
      auto [branch_length, production_count, voxel_colour, branching_angle] = gui.GetState();

      /* wipe the current draw data */
      renderer->ResetVoxels(current_chunk_index);
      l_system->GetPlantChunk(current_chunk_index)->WipeVoxels();

      /* create a new string */
      l_system->ExecuteProductions(production_count, current_chunk_index);

      /* process the new string and write the data */
      l_system->ProcessString(branch_length, branching_angle, current_chunk_index);

      /* process the new data */
      renderer->UpdateChunkData(l_system->GetPlantChunk(current_chunk_index)->render_data_, current_chunk_index);
      renderer->ProcessChunkData(current_chunk_index);
    }

    if (gui.FlagSet(Interface::Flags::kSwapChunks)){
      gui.SetFlag(Interface::Flags::kSwapChunks);
      auto new_chunk_index = gui.GetChunkIndex();
      std::cout << fmt::format("new index: {}\n", new_chunk_index);
      current_chunk_index = new_chunk_index;
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