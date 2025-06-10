#include "SDL_rect.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

SDL_Surface* global_surface = NULL;
SDL_Window* global_window = NULL;
/* SDL_Renderer* global_renderer = NULL; */




//function definitions:
void createCircle(int, int , int);

bool init() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized: %s", SDL_GetError());
    return false;
  }

  global_window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if(global_window==NULL) {
    printf("SDL Window could not be created: %s", SDL_GetError());
    return false;
  }
  global_surface = SDL_GetWindowSurface(global_window);




  /* global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED); */

  /* if(global_renderer == NULL) { */
  /*     printf("SDL could not create renderer! SDL_ERROR: %s\n", SDL_GetError()); */
  /*     return false; */
  /* } */

  /* SDL_SetRenderDrawColor(global_renderer, 0xFF, 0xFF, 0xFF, 0xFF); */


  return true;
}



void close() {
  SDL_DestroyWindow(global_window);
  global_window = NULL;
  global_surface = NULL;
  SDL_Quit();
}




int main(int argc, char* argv[]) {

  //initializing the main loop:
  if(!init()) {
    return 0;
  }


  //variables:
  bool quit = false;



  while(!quit) {

    //event handling:
    SDL_Event e;
    while(SDL_PollEvent(&e)) {

      if(e.type == SDL_QUIT) {
        quit = true;
      }


      if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_ESCAPE) {
          quit = true;
        }
      }
    }





    //update the window
    createCircle(300, 300, 100);
    SDL_UpdateWindowSurface(global_window);
  }


  close();
  return 0;
}



void createCircle(int x, int y, int radius) {
  SDL_FillRect(global_surface, &(SDL_Rect){x-radius, y-radius, radius*2, radius*2},0xffffff);
  SDL_FillRect(global_surface, &(SDL_Rect){x, y, 2, 2}, 0x89854b);


  int left_boundry = x-radius;
  int right_boundry = x+radius;
  int top_boundry = y - radius;
  int bottom_boundry = y + radius;
  for(int horizontal = left_boundry; horizontal <= right_boundry; horizontal++) {
    for(int vertical = top_boundry; vertical <= bottom_boundry; vertical++) {
      int distance_from_center_squared = (horizontal - x)*(horizontal - x) +(vertical - y)*(vertical-y);
      if((distance_from_center_squared) <= (radius*radius)) {
        SDL_FillRect(global_surface, &(SDL_Rect){horizontal, vertical, 1, 1}, 0x89854b);
      }
    }
  }

}
