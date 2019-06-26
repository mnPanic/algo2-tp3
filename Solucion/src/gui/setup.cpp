#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "LTexture.h"

bool loadMedia(LTexture &gSpriteSheetTexture, SDL_Rect *gSpriteClips) {
  // Loading success flag
  bool success = true;

  // Load sprite sheet texture
  if (!gSpriteSheetTexture.loadFromFile("/home/march/temp/ee/resources/dots.png")) {
    printf("Failed to load sprite sheet texture!\n");
    success = false;
  } else {
    // Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 100;
    gSpriteClips[0].h = 100;

    // Set top right sprite
    gSpriteClips[1].x = 100;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 100;
    gSpriteClips[1].h = 100;

    // Set bottom left sprite
    gSpriteClips[2].x = 0;
    gSpriteClips[2].y = 100;
    gSpriteClips[2].w = 100;
    gSpriteClips[2].h = 100;

    // Set bottom right sprite
    gSpriteClips[3].x = 100;
    gSpriteClips[3].y = 100;
    gSpriteClips[3].w = 100;
    gSpriteClips[3].h = 100;
  }

  return success;
}
