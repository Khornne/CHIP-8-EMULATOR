#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window;

int init_sdl(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error Initisalising SLD %s\n", SDL_GetError());
  }
  SDL_Window *window =
      SDL_CreateWindow("CHIP-8 EMULATOR", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
  if (window == NULL) {
    printf("Unable to create window: %s\n", SDL_GetError());
  }
  return 0;
}

int final_cleanup(void) {
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
