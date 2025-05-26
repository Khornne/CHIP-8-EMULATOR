#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_t;

sdl_t sdl;

bool init_sdl(sdl_t *sdl) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error Initisalising SLD %s\n", SDL_GetError());
    return false;
  }
  sdl->window = SDL_CreateWindow("CHIP-8 EMULATOR", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, 500, 500, 0);
  if (sdl->window == NULL) {
    SDL_Log("Unable to create SDL Window: %s\n", SDL_GetError());
    return false;
  }
  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl->renderer == NULL) {
    SDL_Log("Error unable to create SDL Renderer | %s\n", SDL_GetError());
    return false;
  }
  return true; // Success
}

void final_cleanup(void) {
  SDL_DestroyRenderer(sdl.renderer);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
}

// Initial Screen clearing it to background color
void clear_window(const sdl_t sdl) {
  SDL_SetRenderDrawColor(sdl.renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sdl.renderer);
}

// updated changes in window
void screen_update(sdl_t sdl) { SDL_RenderPresent(sdl.renderer); }

int main(int argc, char *argv[]) {
  // SDL init
  sdl_t sdl = {0};
  if (!init_sdl(&sdl))
    exit(EXIT_FAILURE);

  // Render Clear
  clear_window(sdl);

  // Main loop
  while (true) {
    SDL_Delay(16); // Delay for 60hz/60fps

    screen_update(sdl);
  }

  // Final Clean
  final_cleanup();

  exit(EXIT_SUCCESS);
}
