#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_t;

int init_sdl(sdl_t *sdl) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error Initisalising SLD %s\n", SDL_GetError());
    return 1;
  }
  sdl->window = SDL_CreateWindow("CHIP-8 EMULATOR", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, 320, 240, 0);
  if (sdl->window == NULL) {
    SDL_Log("Unable to create SDL Window: %s\n", SDL_GetError());
    return 0;
  }
  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl->renderer == NULL) {
    SDL_Log("Error unable to create SDL Renderer | %s\n", SDL_GetError());
  }
  return 1;
}

int final_cleanup(void) {
  sdl_t sdl;
  SDL_DestroyRenderer(sdl.renderer);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
  return 0;
}

// Initial Screen clearing it to background color
int clear_window(void) {
  sdl_t sdl;
  SDL_RenderClear(sdl.renderer);
  // SDL_GetRenderDrawColor(sdl.renderer, 255, 255, 255, 255);
  return 0;
}

int main(int argc, char *argv[]) {
  // SDL init
  sdl_t sdl = {0};
  init_sdl(&sdl);

  // Render Clear
  clear_window();

  // Final Clean
  final_cleanup();

  exit(EXIT_SUCCESS);
}
