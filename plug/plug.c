#include <stdio.h>
#include "plug.h"
#include <SDL2/SDL.h>
#define ACC_GRAVITY 10

// The implementation of our pluggable function.
void plug_update(void) {
    printf("--->Again Hello frrr from Plugin v1! <--- \n");

}


typedef struct {
  float center_x;
  float center_y;
  float radius;
  float velocity_x;
  float velocity_y;
} Circle;

void step(Circle* circle, int width, int height) {


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



void createCircle(Circle *circle, SDL_Surface* global_surface) {

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
        SDL_FillRect(global_surface, &(SDL_Rect){horizontal, vertical, 1, 1}, 0xff0000);
      }
    }
  }
}
