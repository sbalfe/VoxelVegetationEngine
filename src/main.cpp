//
// Created by shriller44 on 12/26/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Renderer.h"
#include "PCG/Lindenmayer.h"
#include "fmt/format.h"

int main(){

  // Renderer fills the voxel
  auto renderer = std::make_unique<Renderer>(1000,600);

  Lindenmayer l_system {"p", renderer, {// front quad
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
                                        }};

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

  l_system.AddRule('p',"9I+[p+#]--//[--#]I[++#]-[p#]++p#");
  l_system.AddRule('I', ">S[//&&#][//^^#]>S");
  l_system.AddRule('S',"9S>S");

  std::string result = l_system.ExecuteProductions(3);
  std::cout << result << std::endl;

  l_system.ProcessString(3);

  bool escaped {};
  bool should_run;

  for (should_run = true; should_run;) {
    SDL_Event event;
    renderer->processKeyboard();
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      switch (event.type) {
        case SDL_QUIT: {
          should_run = false;
          SDL_Quit();
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_ESCAPE){
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

    // render GUI
    renderer->ShowGUI();
    auto state = renderer->GetState();

    if (renderer->process_again_){
      renderer->process_again_ = false;
      l_system.ExecuteProductions(state.production_count_);
      l_system.ProcessString(state.branch_length_);
    }

    renderer->Render();
  }

  printf("Exiting...\n");
  return 0;
}
