#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_t;
sdl_t sdl;

typedef enum {
  QUIT,
  RUNNING,
  PAUSED,
} emu_state_t;

typedef struct {
  emu_state_t state;
  uint8_t ram[4096];
  uint16_t PC;   // Program Counter
  uint16_t I;    // 12 bit index register
  uint8_t V[16]; // V0 - VF data register
  uint16_t stack[16];
  uint8_t stack_pointer;
  uint8_t delay_timer;   // Lowers to 60hz when > 0
  uint8_t sound_timer;   // Lowers to 60hz and plays sounds when > 0
  bool display[64 * 32]; // Original Chip-8 pixel resolution
  bool draw_flag;
  bool key[16];   // 0x0- 0xF Hex Keypad
  char *rom_name; // Name of ROM that is loaded
} chip8_t;

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

bool init_chip8(chip8_t *chip8, char rom_name[]) {
  const uint32_t start_up = 0x200; // Chip-8 ROMs load up to 0x200
  const uint8_t font[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  // Load Font
  memcpy(&chip8->ram[0], font, sizeof(font));
  // Load ROM

  // Open ROM
  FILE *rom = fopen(rom_name, "rb");
  if (!rom) {
    SDL_Log("Error ROM file %s failed to load or is invalid\n", rom_name);
    return false;
  }
  // Check ROM size
  fseek(rom, 0, SEEK_END);
  const size_t rom_size = ftell(rom);
  const size_t max_size = sizeof chip8->ram - start_up;
  rewind(rom);

  if (rom_size > max_size) {
    SDL_Log("Error ROM file %s failed to load or is invalid\n", rom_name);
    return false;
  }

  if (fread(&chip8->ram, rom_size, 1, rom) != 1) {
    SDL_Log("Unable to read ROM file  %s in memory\n", rom_name);
    return false;
  }

  fclose(rom);

  // Defaults
  chip8->state = RUNNING; // Default emu state on/running
  chip8->PC = start_up;   // Start program counter at ROM start_up
  chip8->rom_name = rom_name;
  return true;
};

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

// User input handler
void user_input(chip8_t *chip8) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: // End Program
      exit(0);
      return;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE: // Escape key
        exit(0);
        return;
      case SDLK_SPACE: // Spacebar
        if (chip8->state == RUNNING) {
          chip8->state = PAUSED;
          puts("===PASUED===");

        } else {
          chip8->state = RUNNING;
        };
        return;

      default:
        break;
      }
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  // Default Usage message
  if (argc > 2) {
    fprintf(stderr, "Usage: %s <rom_name>\b", argv[0]);
    exit(EXIT_FAILURE);
  };
  // SDL init
  sdl_t sdl = {0};
  if (!init_sdl(&sdl))
    exit(EXIT_FAILURE);

  // CHIP-8 init
  chip8_t chip8;
  if (!init_chip8(&chip8, argv[1]))
    exit(EXIT_FAILURE);

  // Render Clear
  clear_window(sdl);

  // Main loop
  while (chip8.state != QUIT) {
    user_input(&chip8);

    SDL_Delay(16); // Delay for 60hz/60fps-ish

    screen_update(sdl);
  }

  // Final Clean
  final_cleanup();

  exit(EXIT_SUCCESS);
}
