#include <windows.h>
#include "SDL_surface.h"
#include "SDL_video.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <../plug/plug.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define TRAJECTORY_LENGTH 500
#define TRAJECTORY_WIDTH 5

Plug plug = {0};


Circle trajectory[TRAJECTORY_LENGTH];
int circle_count = 0;





//function definitions:
void createCircle(Circle*);
void step(Circle*);









bool init() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized: %s", SDL_GetError());
    return false;
  }

  plug.global_window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

  if(plug.global_window==NULL) {
    printf("SDL Window could not be created: %s", SDL_GetError());
    return false;
  }
  plug.global_surface = SDL_GetWindowSurface(plug.global_window);





  /* global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED); */

  /* if(global_renderer == NULL) { */
  /*     printf("SDL could not create renderer! SDL_ERROR: %s\n", SDL_GetError()); */
  /*     return false; */
  /* } */

  /* SDL_SetRenderDrawColor(global_renderer, 0xFF, 0xFF, 0xFF, 0xFF); */


  return true;
}



void close() {
  SDL_DestroyWindow(plug.global_window);
  plug.global_window = NULL;
  plug.global_surface = NULL;
  SDL_Quit();
}

//global variables
void (*step_ptr)(Circle*,int, int, float);
void (*createCircle_ptr)(Circle*, SDL_Surface*);
HMODULE plug_dll;





bool hot_reload() {

    if(plug_dll) {
      FreeLibrary(plug_dll);
    }

   int result = system(
      "gcc -shared -o ../build/plug.dll ../plug/plug.c "
      "-I../dependencies/SDL2-2.32.6/x86_64-w64-mingw32/include "
      "../dependencies/SDL2-2.32.6/x86_64-w64-mingw32/lib/libSDL2main.a "
      "../dependencies/SDL2-2.32.6/x86_64-w64-mingw32/lib/libSDL2.a "
      "-lmingw32 "
      "-lsetupapi -limm32 -lversion -lwinmm -lkernel32 -luser32 -lgdi32 -lwinspool "
      "-lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 "
      "-Wl,--out-implib,../build/libplug.dll.a"
  );

    if(result !=0) {
      printf("DLL recompilation failed.\n");
      return false;
    }


    //we need to copy the .dll as window locks the currently used file and prevents the recompilation
    CopyFile("../build/plug.dll", "../build/plug_temp.dll", FALSE);
    plug_dll = LoadLibrary("../build/plug_temp.dll");

    step_ptr = (void (*)(Circle*, int, int, float))GetProcAddress(plug_dll, "step");

    createCircle_ptr = (void (*)(Circle*, SDL_Surface*))GetProcAddress(plug_dll, "createCircle");

    return true;
}





int main(int argc, char* argv[]) {


  //load sdl
  if(!init()) {
    return 1;
  }


  //hot reload the functions
  if(!hot_reload()) {
    printf("Hot reloading failed");
    return 1;
  }



  //variables:
  bool    quit           = false;
  plug.circle         = &(Circle){.center_x = 300, .center_y = 300, .radius = 100, .velocity_x = 100, .velocity_y = 0};
  Uint64  perf_frequency = SDL_GetPerformanceFrequency();
  Uint64  last_time      = SDL_GetPerformanceCounter();
  float   dt             = 0;


  while(!quit) {

    //calculate 'dt'
    Uint64 current_time = SDL_GetPerformanceCounter();
    dt = (float)(current_time - last_time) / (float)perf_frequency;
    last_time = current_time;





    //event handling:
    SDL_Event e;
    while(SDL_PollEvent(&e)) {

      if(e.type == SDL_QUIT) {
        quit = true;
      }


      if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_ESCAPE) {
          quit = true;
        } else if(e.key.keysym.sym == SDLK_r) {
          hot_reload();
        }
      }
    }

    //add the circle the tragectory
    if(circle_count < TRAJECTORY_LENGTH) {
      trajectory[circle_count] = *plug.circle;
      circle_count++;
    } else {
      //this block is executed when circle count = TRAJECTORY LENGTH
      for(int i = 0; i<circle_count-1; i++) {
        trajectory[i] = trajectory[i+1];
      }
      trajectory[circle_count - 1] = *plug.circle;
    }

    //clear the screen
    int width, height;
    SDL_GetWindowSize(plug.global_window, &width, &height);
    SDL_FillRect(plug.global_surface, &(SDL_Rect){0, 0, width, height}, 0x000000);



    //render the trajectory
    for(int i = 0; i < circle_count; i++) {
      trajectory[i].radius = TRAJECTORY_WIDTH*i*0.005;
      createCircle_ptr(&trajectory[i], plug.global_surface);
    }







    //calculate the position of the circle
    step_ptr(plug.circle,width, height, dt);

    //create the circle
    createCircle_ptr(plug.circle, plug.global_surface);
    SDL_UpdateWindowSurface(plug.global_window);
  }



  close();
  return 0;
}


