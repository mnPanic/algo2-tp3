#include "G_Game.h"
#include <iostream>
#include "modelo/TiposJuego.h"

using namespace std;

G_Game::G_Game(const G_Map &g_map, ExtremeExorcism &game, unsigned int fire_duration_frames,
               unsigned int time_steps_per_refresh)
    : _g_map(g_map), _g(game), _fire_duration_frames(fire_duration_frames),
      _time_steps_per_refresh(time_steps_per_refresh), _time_steps_remaining(time_steps_per_refresh) {
}

void G_Game::update_game_status(SDL_Event &e) {
  Accion a = MARRIBA;
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
    case SDLK_i:
      a = MARRIBA;
      break;
    case SDLK_k:
      a = MABAJO;
      break;
    case SDLK_j:
      a = MIZQUIERDA;
      break;
    case SDLK_l:
      a = MDERECHA;
      break;
    case SDLK_u:
      a = DISPARAR;
      G_Fire gf(_g_map, _g.posicionJugador("P1"), PC);
      _fire_frames_left.push_back(make_pair(gf, _fire_duration_frames));
    }
    _g.ejecutarAccion("P1", a);
  }
}

void G_Game::forward_game_status() {
  while (_time_steps_remaining > 0) {
    for (PosYDir p: _g.disparosFantasmas()) {
      G_Fire gf(_g_map, p, NPC);
      _fire_frames_left.push_back(make_pair(gf, _fire_duration_frames));
    }
    _g.pasar();
    _time_steps_remaining--;
  }
  _time_steps_remaining = _time_steps_per_refresh;
}

void G_Game::render_game_status(ThingsTiles &tt, SDL_Environment &env) {
  _g_map.render(env.renderer);

  for (pair<Jugador, PosYDir> j_pd : _g.posicionJugadores()) {
    Pos p = _g_map.translate(j_pd.second.pos);
    tt.render_char(p.first, p.second, j_pd.second.dir);
  }

  for (PosYDir pd : _g.posicionFantasmas()) {
    Pos p = _g_map.translate(pd.pos);
    tt.render_ghost(p.first, p.second, pd.dir);
  }

  for (auto &gf_t : _fire_frames_left) {
    gf_t.first.render(tt, env);
    gf_t.second--;
  }

  for (auto it = _fire_frames_left.begin(); it != _fire_frames_left.end();
       it++) {
    if (it->second == 0) {
      it = _fire_frames_left.erase(it);
    }
  }

  SDL_RenderPresent(env.renderer);
}
