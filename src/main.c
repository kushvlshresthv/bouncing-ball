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
Plug plug = {0};



bool init_sdl() {
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

  return true;
}



void close() {
  SDL_DestroyWindow(plug.global_window);
  plug.global_window = NULL;
  plug.global_surface = NULL;
  SDL_Quit();
}



//global variables
void (*init_ptr)(Plug *);
void (*update_ptr)(Plug *);
HMODULE plug_dll;



bool hot_reload() {

    if(plug_dll) {
      FreeLibrary(plug_dll);
    }


    //recompile plug.c and plug.h  (this causes the lag)
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

    init_ptr = (void (*)(Plug *))GetProcAddress(plug_dll, "init");
    update_ptr = (void (*)(Plug *))GetProcAddress(plug_dll, "update");

    return true;
}





int main(int argc, char* argv[]) {


  //load sdl
  if(!init_sdl()) {
    return 1;
  }


  //hot reload the functions
  if(!hot_reload()) {
    printf("Hot reloading failed");
    return 1;
  }



  bool    quit           = false;
  init_ptr(&plug);

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
    SDL_GetWindowSize(plug.global_window, &plug.window_width, &plug.window_height);
    SDL_FillRect(plug.global_surface, &(SDL_Rect){0, 0, plug.window_width, plug.window_height}, 0x000000);

    update_ptr(&plug);
    SDL_UpdateWindowSurface(plug.global_window);
  }

  close();
  return 0;
}


