#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <stdio.h>
#include <stdlib.h>

void init_sdl(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error Initisalising SLD %s\n", SDL_GetError());
  }
}

void final_cleanup(void) { SDL_Quit(); }

int main(int argc, char *argv[]) {
  // SDL init
  init_sdl();

  // Final Clean
  final_cleanup();

  exit(EXIT_SUCCESS);
}
