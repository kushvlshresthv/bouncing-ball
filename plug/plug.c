#include <math.h>
#include "plug.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define ACC_GRAVITY 700
#define DAMPEN 0.92
#define TRAJECTORY_LENGTH 500
#define TRAJECTORY_WIDTH 5

Uint64 current_time;
Uint64 last_time;


Circle trajectory[TRAJECTORY_LENGTH];
int circle_count = 0;



//forward declarations:
void record_trajectory(Circle *);
void render_trajectory(SDL_Surface *);
void create_circle(Circle *, SDL_Surface*);
void next_position(Circle*, int, int, float);



void init(Plug *plug) {

    plug->circle = (Circle){.center_x = 300, .center_y = 300, .radius = 100, .velocity_x = 100, .velocity_y = 0};

    printf("initialization done\n");

  last_time = SDL_GetPerformanceCounter();
}





void update(Plug *plug) {

    //calculate how many time is taken to render each frame

    Uint64 current_time = SDL_GetPerformanceCounter();
    plug->dt = (float)(current_time-last_time)/(float)SDL_GetPerformanceFrequency();
    last_time = current_time;

    printf("FPS: %f\n", 1/plug->dt);

    record_trajectory(&plug->circle);
    render_trajectory(plug->global_surface);
    next_position(&plug->circle, plug->width, plug->height, plug->dt);
    create_circle(&plug->circle, plug->global_surface);
}




void record_trajectory(Circle *circle) {
  printf("Trajectory recorded\n");
    //add the circle the tragectory
    if(circle_count < TRAJECTORY_LENGTH) {
      trajectory[circle_count] = *circle;
      circle_count++;
    } else {
      //this block is executed when circle count = TRAJECTORY LENGTH
      for(int i = 0; i<circle_count-1; i++) {
        trajectory[i] = trajectory[i+1];
      }
      trajectory[circle_count - 1] = *circle;
    }
}


void render_trajectory(SDL_Surface* global_surface) {
  printf("Trajectory Rendered\n");
    for(int i = 0; i < circle_count; i++) {
      trajectory[i].radius = TRAJECTORY_WIDTH*i*0.005;
      create_circle(&trajectory[i], global_surface);
    }
}





//NOTE: The bug was occuring because i was not correcting the position everytime the ball collided with the wall and the code for collision handling was being executed again and again changing the direction of velocity again and again

void next_position(Circle* circle, int width, int height, float dt) {

  //apply gravity if the ball isn't resting in 'y' direction
  if(!circle->is_resting_y) {
    circle->velocity_y +=ACC_GRAVITY * dt;
  }

  //tries to emulate friction
  if(circle->is_resting_y && !circle->is_resting_x) {
    //NOTE: the velocity may also swing to the other direction here, so we have to take care of that

    if(circle->velocity_x > 0) {
     circle->velocity_x -= 0.5;
    } else {
      circle->velocity_x += 0.5;
    }

    if(fabs(circle->velocity_x) < 0.51) {
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
}



void create_circle(Circle *circle, SDL_Surface* global_surface) {

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
