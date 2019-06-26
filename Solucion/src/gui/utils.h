#include "SDL_Environment.hpp"
#include "modelo/ExtremeExorcism.h"
#include "G_Map.h"
#include <SDL2/SDL.h>
#include "ThingsTiles.hpp"

void update_game_status(ExtremeExorcism &g, SDL_Event &e);

void render_game_status(const ExtremeExorcism &g, const G_Map &gm, ThingsTiles &tt,
                        SDL_Environment &env);
