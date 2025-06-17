#ifndef PLUG_H_
#define PLUG_H_
#include <stdbool.h>
#include <SDL2/SDL.h>
typedef struct {
  float center_x;
  float center_y;
  float radius;
  float velocity_x;
  float velocity_y;
  bool is_resting_y;
  bool is_resting_x;
} Circle;


typedef struct {
  Circle circle;
  SDL_Window *global_window;
  SDL_Surface *global_surface;   //can be obtained from the global window itself
  float dt;
  int width;    //can be obtained from method call as well
  int height;
} Plug;


#endif // PLUG_H_
