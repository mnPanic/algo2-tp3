//
// Created by march on 11/06/19.
//

#include "localizador_estatico.h"
#include <algorithm>

LocalizadorEstatico::LocalizadorEstatico(map<Jugador, PosYDir> pos_y_dir)
    : max_ronda(0) {
  _pos_y_dirs.insert(make_pair(max_ronda, pos_y_dir));
}

LocalizadorEstatico::LocalizadorEstatico(
    map<Ronda, map<Jugador, PosYDir>> pos_y_dirs)
    : _pos_y_dirs(pos_y_dirs), max_ronda(-1) {

  list<Ronda> rs;
  for (auto rs_x : pos_y_dirs) {
    rs.push_back(rs_x.first);
  }
  max_ronda = *std::max_element(rs.begin(), rs.end());
}

map<Jugador, PosYDir>
LocalizadorEstatico::localizar_jugadores(const set<Jugador> &jugadores,
                                         const list<Fantasma> &fantasmas,
                                         const Habitacion &hab) {
  if (_pos_y_dirs.count(fantasmas.size())) {
    return _pos_y_dirs.at(fantasmas.size());
  } else {
    return _pos_y_dirs.at(max_ronda);
  }
}
