//
// Created by shriller44 on 12/26/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Renderer.h"
#include "PCG/Lindenmayer.h"
#include "fmt/format.h"

void HandleSDLEvent(std::unique_ptr<Renderer>& renderer, SDL_Event& event, bool& should_run, bool& escaped){
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
        renderer->ProcessMouse(static_cast<float>(x),
                               static_cast<float>(y));
      }
    }
  }
}

int main(){

  // Renderer fills the voxel
  auto renderer = std::make_unique<Renderer>(1000,600);

// s


  /*
   * 0-9 are the values used to alter the width / length parameters of the generated branch
   * > advances the turtle forward in the current orientation defined by a branch length parameter
   * [ to save the position, orientation and width / length parameters of the generated branch.
   * ] to load the last saved position, orientation and width / length parameters of the generated branch.
   * # places a leaf
   * each rotation has +/- degree parameter respectively.
   * + - = rotate on z axis
   * & ^ - rotate on x axis
   * \\ / - rotate on y axis
   * */

//  l_system.TestFill();

  //l_system.AddRule('p',"9I+[p+#]--//[--#]I[++#]-[p#]++p#");
  //l_system.AddRule('I', ">S[//&&#][//^^#]>S");
  //l_system.AddRule('S',"9S>S");

// std::string result = l_system.ExecuteProductions(3);
 // std::cout << result << std::endl;

  //l_system.ProcessString(3);

  bool escaped {};
  bool should_run;

  Chunk test_chunk {{2,2,2}};
  test_chunk.ExportToObj("../exported_models/chunk.obj");
  renderer->ProcessChunk(test_chunk);

  for (should_run = true; should_run;) {
    SDL_Event event;
    //renderer->HandleKeyboard();
    while (SDL_PollEvent(&event)) {
      HandleSDLEvent(renderer, event, should_run, escaped);
    }
    renderer->HandleKeyboard();
    renderer->Render();
  }
  printf("Exiting...\n");
  return 0;
}
