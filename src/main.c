#include "SDL_rect.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ACC_GRAVITY 10



//variables:
SDL_Surface* global_surface = NULL;
SDL_Window* global_window = NULL;

typedef struct {
  float center_x;
  float center_y;
  float radius;
  float velocity_x;
  float velocity_y;
} Circle;
/* SDL_Renderer* global_renderer = NULL; */





//function definitions:
void createCircle(Circle*);
void step(Circle*);









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
  Circle *circle = &(Circle){.center_x = 300, .center_y = 300, .radius = 100, .velocity_x = 30, .velocity_y = 10};



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


    //calculate the position of the circle
    step(circle);

    //create the circle
    createCircle(circle);


    SDL_UpdateWindowSurface(global_window);
  }



  close();
  return 0;
}


void step(Circle* circle) {
    //clear the screen
    int width, height;
    SDL_GetWindowSize(global_window, &width, &height);
    SDL_FillRect(global_surface, &(SDL_Rect){0, 0, width, height}, 0x000000);


    //update the position of the ball
    circle->center_y += circle->velocity_y*0.01;
    circle->velocity_y += ACC_GRAVITY*0.01;
    circle->center_x += circle->velocity_x*0.01;

    if(circle->center_y + circle->radius > height || circle->center_y - circle->radius <0) {
     circle->velocity_y = -(circle->velocity_y);
    }

    if(circle->center_x + circle->radius > height || circle->center_x - circle->radius <0) {
     circle->velocity_x = -(circle->velocity_x);
    }

}



void createCircle(Circle *circle) {

  //defining the boundries of the circle
  int left_boundry = circle->center_x-circle->radius;
  int right_boundry = circle->center_x+circle->radius;
  int top_boundry = circle->center_y - circle->radius;
  int bottom_boundry = circle->center_y + circle->radius;


  //rendering the circle pixel by pixel
  for(int horizontal = left_boundry; horizontal <= right_boundry; horizontal++) {
    for(int vertical = top_boundry; vertical <= bottom_boundry; vertical++) {
      int distance_from_center_squared = (horizontal - circle->center_x)*(horizontal - circle->center_x) +(vertical - circle->center_y)*(vertical-circle->center_y);
      if((distance_from_center_squared) <= (circle->radius*circle->radius)) {
        SDL_FillRect(global_surface, &(SDL_Rect){horizontal, vertical, 1, 1}, 0x89854b);
      }
    }
  }
}
