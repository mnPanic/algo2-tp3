#include "G_Fire.h"
#include <iostream>

using namespace std;

G_Fire::G_Fire(const G_Map& gm, PosYDir pd, ActorType type) 
  : _type(type) {
    for (Pos p : gm.fire_pos(pd)) {
      _render_pos.insert(p);
    }
};

void G_Fire::render(ThingsTiles &tt, SDL_Environment &env) {
  for (Pos p : _render_pos) {
    if (_type == PC) { 
      tt.render_fire(p.first, p.second);
    } else {
      tt.render_spikes(p.first, p.second);
    }
  }
};
