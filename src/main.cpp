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
        renderer.ProcessMouse(static_cast<float>(x),
                               static_cast<float>(y));
      }
    }
  }
}

int main() {

  auto* test_chunk = new Chunk{100,100,100};
  auto renderer = std::make_unique<Renderer>(1000, 600);
  auto* l_system = new Lindenmayer("p", test_chunk, {0, 0, 0});

 // l_system->AddRule('p', ">>>>");
  l_system->AddRule('p', "9I+[p+#]--//[--#]I[++#]-[p#]++p#");
  l_system->AddRule('I', ">S[//&&#][//^^#]>S");
  l_system->AddRule('S', "9S>S");

  auto output = l_system->ExecuteProductions(5);
  std::cout << fmt::format("output: {}\n", output);
  l_system->ProcessString(0, *renderer);
  renderer->ProcessChunk(l_system->GetPlantChunk());

  /*
   * 0-9 are the values used to alter the width / length parameters of the generated branch > advances the turtle forward in the current orientation defined by a branch length parameter [ to save the position, orientation and width / length parameters of the generated branch. ] to load the last saved position, orientation and width / length parameters of the generated branch. # places a leaf each rotation has +/- degree parameter respectively.
   * + - = rotate on z axis
   * & ^ - rotate on x axis
   * \\ / - rotate on y axis
   * */

    bool escaped {};
    bool should_run;

    for (should_run = true; should_run;) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        HandleSDLEvent(*renderer, event, should_run, escaped);
      }
      renderer->Render();
    }
    printf("Exiting...\n");
  return 0;
}