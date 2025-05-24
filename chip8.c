#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;

int init_sdl(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error Initisalising SLD %s\n", SDL_GetError());
  }
  SDL_Window *window =
      SDL_CreateWindow("CHIP-8 EMULATOR", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 320, 240, 0);
  if (window == NULL) {
    SDL_Log("Unable to create SDL Window: %s\n", SDL_GetError());
    return 0;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_Log("Error unable to create SDL Renderer | %s\n", SDL_GetError());
  }
  return 1;
}

int final_cleanup(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

int main(int argc, char *argv[]) {
  // SDL init
  init_sdl();

  // Final Clean
  final_cleanup();

  exit(EXIT_SUCCESS);
}
