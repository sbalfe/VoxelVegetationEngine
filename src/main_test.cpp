//
// Created by shriller44 on 12/26/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Renderer.h"
#include "PCG/Lindenmayer.h"
#include "fmt/format.h"

int main(){

  auto renderer = std::make_unique<Renderer>(1000,600);

  Lindenmayer l_system("F");

  l_system.AddRule('F', "FF");

  std::string result = l_system.ExecuteProductions(1);
  std::cout << fmt::format("result: {}\n", result);

  std::vector<uint32_t> indices {
      // front quad
      0,1,3,
      2,3,1,
      // left quad
      2,6,3,
      3,6,7,
      //right quad
      0,1,4,
      5,0,4,
      //bottom quad
      2,6,1,
      4,6,1,
      //top quad
      3,0,7,
      7,5,0,
      //back quad
      7,6,4,
      5,7,4
  };


  l_system.ProcessString(renderer, indices);

  std::cout << fmt::format("result: {}\n", result);

  bool should_run;
  for (should_run = true; should_run;) {
    SDL_Event event;
    renderer->processKeyboard();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          should_run = false;
          SDL_Quit();
          break;
        }
        case SDL_MOUSEMOTION: {
          int x,y;
          SDL_GetMouseState(&x,&y);
          renderer->ProcessMouse(static_cast<float>(x),static_cast<float>(y));
        }
      }
    }
    renderer->Render();
  }
  printf("Exiting...\n");
  return 0;
}
