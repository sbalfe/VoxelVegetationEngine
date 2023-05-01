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
  std::map<char, std::vector<Lindenmayer::Rule>> rule_map{
      {'p',{
                Lindenmayer::Rule{"I+[p+#]--//[--#]I[++#]-[p#]++p#", 0.5},
                Lindenmayer::Rule{"I+--//[--#]I--[p#]++p#", 0.5}
            }
      },
      {'I', {
                Lindenmayer::Rule{">S[//&&#][//^^#]>S", 1.0}
            }
      },
      {'S', {
                Lindenmayer::Rule{"S>S", 1.0}
            }
      }};
  l_system->AddRuleMap(rule_map);
}

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
      {
          'X', {
                   Lindenmayer::Rule("X>>[^>>]>>[&>>X]>>[//>>X]", 1.0),
               }
      }
  };

  l_system->AddRuleMap(rule_map);
}

void ExecuteChunk(uint32_t chunk_index, uint32_t production_count, Lindenmayer* l_system, Renderer& renderer){

  /* generate the string, return output for debugging purposes*/
  auto output_string = l_system->ExecuteProductions(production_count, chunk_index);

  /* process it which fills our chunk */
  l_system->ProcessString(chunk_index);

  /* store the chunk data with the associated index in the renderer*/
  renderer.UpdateChunkData(l_system->GetPlantChunk(chunk_index)->GetRenderData(), chunk_index);

  /* update the buffers of each voxel in the chunk data ready to be rendered*/
  renderer.ProcessChunkData(chunk_index);
}

int main() {
  uint32_t chunk_size = 32768;

  /* setup our renderer*/
  auto renderer = std::make_unique<Renderer>(1000, 600, chunk_size);

  /* setup our L-system*/
  auto* l_system = new Lindenmayer();
  PlantOne(l_system);

  /* Set up our chunks and add them to our Scene, where a Scene is a Chunk with its corresponding state*/
  auto* c1 = new Chunk{chunk_size};
  auto* c2 = new Chunk{chunk_size};

  auto chunk1 = l_system->AddScene(c1, Lindenmayer::CreateState("p", 18, 3));
  auto chunk2 = l_system->AddScene(c2, Lindenmayer::CreateState("p", 7, 5));

  /* generate the initial model */
  ExecuteChunk(chunk1, 5, l_system, *renderer);
  ExecuteChunk(chunk2, 5, l_system, *renderer);

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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      HandleSDLEvent(*renderer, event, should_run, escaped);
    }
    renderer->Render(current_chunk_index);
  }
  printf("Exiting...\n");

  return 0;
}