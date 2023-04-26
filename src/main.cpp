//
// Created by shriller44 on 12/26/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Renderer.h"
#include "PCG/Lindenmayer.h"
//#include "Engine/Chunk/Chunk.h"
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

int BasicTest(Lindenmayer* l_system){
  l_system->AddRule('p', ">>>>>");
}

int PlantOne(Lindenmayer* l_system){
  l_system->AddRule('p', "I+[p+#]--//[--#]I[++#]-[p#]++p#");
  l_system->AddRule('I', ">S[//&&#][//^^#]>S");
  l_system->AddRule('S', "S>S");
}

int main() {

  auto* test_chunk = new Chunk{8192,8192,8192};
  auto renderer = std::make_unique<Renderer>(1000, 600);
  auto* l_system = new Lindenmayer("p", test_chunk, {0, 0, 0});

  PlantOne(l_system);

  auto output = l_system->ExecuteProductions(8);
  std::cout << fmt::format("output: {}\n", output);

  l_system->ProcessString(0, *renderer);
  renderer->ProcessChunk(l_system->GetPlantChunk());
  bool escaped {};

  for (bool should_run = true; should_run;) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      HandleSDLEvent(*renderer, event, should_run, escaped);
    }
    renderer->Render();
  }

  printf("Exiting...\n");
  return 0;
}