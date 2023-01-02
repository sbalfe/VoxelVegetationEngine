//
// Created by shriller44 on 12/26/22.
//
#include "Engine/Renderer.h"

int main(){
  Renderer renderer {1000,600};

  // front and left sides of voxel only.
  std::vector<int> indices {
      0,1,3,
      2,3,1,
      2,6,3,
      3,6,7,

  };

  auto *red_voxel = new Voxel {{1.0f,0.0f,0.0f}, {0.0f,0.20f,0.0f}, indices};
  auto *green_voxel = new Voxel {{0.0f,1.0f,0.0f}, {0.0f,0.0f,0.0f}, indices};
  auto *blue_voxel = new Voxel {{0.0f,0.0f,1.0f}, {0.20f,0.0f,0.0f}, indices};

  renderer.AddVoxel(red_voxel);
  renderer.AddVoxel(green_voxel);
  renderer.AddVoxel(blue_voxel);

  bool should_run;

  for (should_run = true; should_run;) {
    std::cout << "rendering" << std::endl;
    SDL_Event event;
    renderer.processKeyboard();
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
          renderer.ProcessMouse(static_cast<float>(x),static_cast<float>(y));
        }
      }
    }
    renderer.Render();
  }
  printf("Exiting...\n");
  return 0;
}
