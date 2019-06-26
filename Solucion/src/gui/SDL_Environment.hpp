#ifndef __SDL_Environment_CPP__
#define __SDL_Environment_CPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/format.hpp>
#include <exception>

#define GAME_TITLE "Extreme Exorcism"

class SDL_Environment {

public:
  SDL_Environment(int screen_width, int screen_height, double scale) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error(
          (boost::format("SDL could not initialize! SDL Error: %1%") %
           SDL_GetError())
              .str());
    } else {
      // Set texture filtering to linear
      if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
      }

      // Create window
      window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);
      if (window == NULL) {
        throw std::runtime_error(
            (boost::format("Window could not be created! SDL Error: %1%") %
             SDL_GetError())
                .str());
      } else {
        // Create renderer for window
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
          throw std::runtime_error(
              (boost::format("Renderer could not be created! SDL Error: %1%") %
               SDL_GetError())
                  .str());
        } else {
          // Initialize renderer color
          SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

          if (!SDL_RenderSetLogicalSize(renderer, (int)(screen_width),
                                        (int)(screen_height))) {
            printf("Warning: Cannot change Render Logical Size");
          }

          // Initialize PNG loading
          int imgFlags = IMG_INIT_PNG;
          if (!(IMG_Init(imgFlags) & imgFlags)) {
            throw std::runtime_error(
                (boost::format(
                     "SDL_image could not initialize! SDL_mage Error: %1%") %
                 IMG_GetError())
                    .str());
          }
        }
      }
    }
  }

  ~SDL_Environment() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
  }

  SDL_Window *window;
  SDL_Renderer *renderer;
};

#endif
