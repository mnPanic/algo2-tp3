#ifndef __G_Game_H__
#define __G_Game_H__

#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_Environment.hpp"
#include "modelo/ExtremeExorcism.h"
#include "G_Map.h"
#include "G_Fire.h"
#include "ActorType.h"

class G_Game {
public:
  G_Game(const G_Map&, ExtremeExorcism&, unsigned int, unsigned int);


  void update_game_status(SDL_Event &e);

  void forward_game_status();

  void render_game_status(ThingsTiles &tt,
                          SDL_Environment &env);

private:
  const G_Map& _g_map;
  ExtremeExorcism& _g;
  const unsigned int _fire_duration_frames;
  const unsigned int _time_steps_per_refresh;
  int _time_steps_remaining;
  std::list<pair<G_Fire, unsigned int>> _fire_frames_left;

};

#endif
