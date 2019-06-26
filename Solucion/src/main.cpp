#include "gui/G_Game.h"
#include "gui/G_Map.h"
#include "gui/LTimer.h"
#include "gui/SDL_Environment.hpp"
#include "gui/ThingsTiles.hpp"
#include "gui/setup.h"
#include "modelo/Habitacion.h"
#include "modelo/ExtremeExorcism.h"
#include "modelo/Contexto.h"

// Using SDL, SDL_image, standard math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

// Screen dimension constants
const unsigned int SCALE = 3;
const unsigned int SCREEN_WIDTH = 1024;
const unsigned int SCREEN_HEIGHT = 768;
const unsigned int TIMES_PER_FRAME = 100;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const unsigned int FRAMES_PER_SECOND = 10;
const float FIRE_DURATION = 0.3; // Seconds
const unsigned int fire_duration_frames = FRAMES_PER_SECOND * FIRE_DURATION;
const unsigned int TIME_STEPS_PER_FRAME = 5;

// Scene sprites
SDL_Rect gSpriteClips[4];

string texto_inicial = "Inicializando ExtremeExorcism...\n"
                       "\n"
                       "El mapa se obtiene a partir del archivo `resources/maps/m1.txt` donde los `.` son los espacios\n"
                       "en blanco y los `#` son las paredes.\n"
                       "\n"
                       "En la versión actual el juego soporta un solo jugador que se mueve son la siguiente disposición\n"
                       "de teclas:\n"
                       "  I  \n"
                       "J K L\n"
                       "\n"
                       "Y dispara con la tecla U.\n"
                       "\n"
                       "Se puede salir del juego con la tecla Q.";

int main(int argc, char *args[]) {
  cout << texto_inicial << endl;
  // Start up SDL and create window
  SDL_Environment env = SDL_Environment(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);
  ThingsTiles things_tiles = ThingsTiles(env.renderer, SCALE);

  // Main loop flag
  bool quit = false;

  // Event handler
  SDL_Event e;

  std::ifstream is("resources/maps/m1.txt", std::ifstream::in);

  Contexto ctx;
  Habitacion m = string_to_hab(is);
  list<Accion> acciones_fantasma(TIME_STEPS_PER_FRAME * FRAMES_PER_SECOND * 5, ESPERAR);
  acciones_fantasma.push_back(MARRIBA);
  for (int i = 0; i < TIME_STEPS_PER_FRAME * FRAMES_PER_SECOND * 2; i++) {
    acciones_fantasma.push_back(ESPERAR);
  }
  acciones_fantasma.push_back(MARRIBA);
  for (int i = 0; i < TIME_STEPS_PER_FRAME * FRAMES_PER_SECOND * 2; i++) {
    acciones_fantasma.push_back(ESPERAR);
  }
  acciones_fantasma.push_back(MARRIBA);
  for (int i = 0; i < TIME_STEPS_PER_FRAME * FRAMES_PER_SECOND * 2; i++) {
    acciones_fantasma.push_back(ESPERAR);
  }
  acciones_fantasma.push_back(DISPARAR);
  ExtremeExorcism g(m, set<Jugador>{"P1"}, PosYDir({2, 2}, ARRIBA), acciones_fantasma, &ctx);

  G_Map gm(SCREEN_WIDTH, SCREEN_HEIGHT, m, things_tiles);

  G_Game gg(gm, g, fire_duration_frames, TIME_STEPS_PER_FRAME);

  // Update screen
  SDL_RenderPresent(env.renderer);


  LTimer fpsTimer;
  // While application is running
  while (!quit) {
    // Handle events on queue

    fpsTimer.start();

    while (SDL_PollEvent(&e) != 0) {
      // User requests quit
      if (e.type == SDL_QUIT or
          (e.type == SDL_KEYDOWN and e.key.keysym.sym == SDLK_q)) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        gg.update_game_status(e);
      }
    }
    gg.forward_game_status();
    gg.render_game_status(things_tiles, env);

    int frameTicks = fpsTimer.getTicks();
    if (frameTicks < SCREEN_TICKS_PER_FRAME) {
      // Wait remaining time
      SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
  }
  return 0;
}
