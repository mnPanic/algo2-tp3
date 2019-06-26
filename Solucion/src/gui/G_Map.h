#ifndef __G_Map_H__
#define __G_Map_H__

#include "ThingsTiles.hpp"
#include <SDL2/SDL.h>
#include "modelo/Habitacion.h"
#include <list>

using namespace std;

class G_Map {

public:
  G_Map(int screen_width, int screen_height, const Habitacion& m, ThingsTiles& tt);
  void render(SDL_Renderer *renderer) const; 
  Pos translate(Pos p) const;
  list<Pos> fire_pos(PosYDir pd) const;

private:
  int _screen_width;
  int _screen_height;
  const Habitacion& _m;
  ThingsTiles& _tt;

  SDL_Rect _map_rect;
};

#endif
