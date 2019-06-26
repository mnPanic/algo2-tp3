#ifndef __G_Map_HPP__
#define __G_Map_HPP__

#include <cassert>
#include "G_Map.h"

G_Map::G_Map(int screen_width, int screen_height, const Habitacion &m, ThingsTiles &tt)
    : _screen_width(screen_width), _screen_height(screen_height), _m(m),
      _tt(tt) {
  int map_width = m.tam() * _tt.tile_width();
  int map_height = m.tam() * _tt.tile_height();
  int map_x = (_screen_width - map_width) / 2;
  int map_y = (_screen_height - map_height) / 2;
  _map_rect = SDL_Rect({map_x, map_y, map_width, map_height});
};

void G_Map::render(SDL_Renderer *renderer) const {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);

  for (int r = 0; r < _m.tam(); r++) {
    for (int c = 0; c < _m.tam(); c++) {
      if (_m.ocupado(Pos(c, r))) {
        _tt.render_wall(_map_rect.x + c * _tt.tile_width(),
                        _map_rect.y + r * _tt.tile_height());
      } else {
        _tt.render_ground(_map_rect.x + c * _tt.tile_width(),
                          _map_rect.y + r * _tt.tile_height());
      }
    }
  }
}

Pos G_Map::translate(Pos p) const {
  return Pos(p.first * _tt.tile_width() + _map_rect.x, 
      _map_rect.h - (p.second + 1) * _tt.tile_height() + _map_rect.y);
}

Pos mover(Pos p, Dir d) {
  switch (d) {
    case ARRIBA:
      p.second += 1;
      break;
    case ABAJO:
      p.second -= 1;
      break;
    case DERECHA:
      p.first += 1;
      break;
    case IZQUIERDA:
      p.first -= 1;
      break;
  }
  return p;
}

list<Pos> G_Map::fire_pos(PosYDir pd) const {
  list<Pos> ret;
  Pos pos = pd.pos;
  Pos n_pos = mover(pd.pos, pd.dir);
  while (!_m.ocupado(n_pos)) {
    ret.push_back(translate(n_pos));
    pos = n_pos;
    n_pos = mover(pos, pd.dir);
    assert(ret.size() < _m.tam());
  }
  return ret;
}

#endif
