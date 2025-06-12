#include <windows.h>
/* #include <time.h> */
#include "SDL_surface.h"
#include "SDL_video.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800



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

//global variables
void (*step_ptr)(Circle*,int, int);
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

    CopyFile("../build/plug.dll", "../build/plug_temp.dll", FALSE);
    plug_dll = LoadLibrary("../build/plug_temp.dll");

    step_ptr = (void (*)(Circle*, int, int))GetProcAddress(plug_dll, "step");
    createCircle_ptr = (void (*)(Circle*, SDL_Surface*))GetProcAddress(plug_dll, "createCircle");

    return true;
}




int main(int argc, char* argv[]) {
  if(!init()) {
    return 1;
  }


  if(!hot_reload()) {
    printf("Hot reloading failed");
    return 1;
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
        } else if(e.key.keysym.sym == SDLK_r) {
          hot_reload();
        }

      }
    }


    //clear the screen
    int width, height;
    SDL_GetWindowSize(global_window, &width, &height);
    SDL_FillRect(global_surface, &(SDL_Rect){0, 0, width, height}, 0x000000);


    //calculate the position of the circle
    step_ptr(circle,width, height);

    //create the circle
    createCircle_ptr(circle, global_surface);
    SDL_UpdateWindowSurface(global_window);
  }



  close();
  return 0;
}


