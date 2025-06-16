#include <stdio.h>
#include <math.h>
#include "plug.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#define ACC_GRAVITY 400
#define DAMPEN 0.8

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
  bool is_resting_y;
  bool is_resting_x;
} Circle;



//NOTE: The bug was occuring because i was not correcting the position everytime the ball collided with the wall and the code for collision handling was being executed again and again changing the direction of velocity again and again
void step(Circle* circle, int width, int height, float dt) {

  //apply gravity if the ball isn't resting in 'y' direction
  if(!circle->is_resting_y) {
    circle->velocity_y +=ACC_GRAVITY * dt;
  }

  //tries to emulate friction
  if(circle->is_resting_y && !circle->is_resting_x) {
    //NOTE: the velocity may also swing to the other direction here, so we have to take care of that
    circle->velocity_x -= 1;
    if(fabs(circle->velocity_x) < 1.01) {
      circle->velocity_x = 0;
      circle->is_resting_x = true;
    }
  }


  //update position:
  circle->center_y += circle->velocity_y * dt;
  circle->center_x += circle->velocity_x * dt;

  //handle collisions and response:

    //1: Bottom Boundry
  if(circle->center_y + circle->radius > height) {

    //correcting the position
    circle->center_y = height - circle->radius;
    circle->velocity_y = -circle->velocity_y * DAMPEN;

    if(fabs(circle->velocity_y) < 1.0) {
      circle->velocity_y = 0;
      circle->is_resting_y = true;
    }
  }






    //2: Top Boundry
  if(circle->center_y - circle->radius < 0) {
    circle->center_y = circle->radius;
    circle->velocity_y = -circle->velocity_y * DAMPEN;

    circle->is_resting_y =false;  //it's in the air again
  }






    //3: Side Boundries
  if(circle->center_x + circle->radius > width) {
    circle->center_x = width - circle->radius;
    circle->velocity_x = -circle->velocity_x * DAMPEN;

    if(fabs(circle->velocity_x)<1.0) {
      circle->velocity_x = 0;
      circle->is_resting_x = true;
    }

  }

  if(circle->center_x - circle-> radius < 0) {
    circle->center_x = circle->radius;
    circle->velocity_x = -circle->velocity_x * DAMPEN;

    if(fabs(circle->velocity_x)<1.0) {
      circle->velocity_x = 0;
      circle->is_resting_x = true;
    }

  }








    /* if(accelerate) { */
    /*   circle->velocity_y += ACC_GRAVITY; */
    /* } */


    /* //update the position of the ball */
    /* circle->center_y += circle->velocity_y; */
    /* circle->center_x += circle->velocity_x; */


    /* //collision logic: */

    /*   //bottom position: */
    /* if(circle->center_y + circle->radius > height) { */

    /*   circle->center_y = height - circle->radius; */
    /*   printf("BEFORE: velocity_y: %f\n", circle->velocity_y); */
    /*   circle->velocity_y = -circle->velocity_y * DAMPEN; */
    /*   printf("AFTER: velocity_y: %f\n", circle->velocity_y); */

    /*   if(fabs(circle->velocity_y) < 0.025) { */
    /*     circle->velocity_y = 0; */
    /*     accelerate = false; */
    /*   } */
    /* } */

    /*   //top position */
    /* if(circle->center_y - circle->radius < 0) { */
    /*   circle->velocity_y = -circle->velocity_y * DAMPEN; */
    /*   accelerate = true; */
    /* } */

    /* if(circle->center_x + circle->radius > width || circle->center_x - circle->radius < 0) { */
    /*  circle->velocity_x = (-(circle->velocity_x)) * DAMPEN; */
    /* } */

    /* SDL_Delay(35); */
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
        SDL_FillRect(global_surface, &(SDL_Rect){horizontal, vertical, 1, 1}, 0x0000ff);
      }
    }
  }
}
