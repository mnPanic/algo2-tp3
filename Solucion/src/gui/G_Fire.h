#ifndef __G_Fire_H__
#define __G_Fire_H__

#include <set>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "modelo/TiposJuego.h"
#include "gui/G_Map.h"
#include "gui/ActorType.h"
#include "gui/SDL_Environment.hpp"
#include "gui/ThingsTiles.hpp"

using namespace std;

class G_Fire {
  public:
    G_Fire(const G_Map&, PosYDir,  ActorType);
    void render(ThingsTiles &tt, SDL_Environment &env);

  private:
    set<Pos> fire_pos(const Habitacion& h, PosYDir pd);

    set<Pos> _render_pos;
    ActorType _type;
};

#endif
