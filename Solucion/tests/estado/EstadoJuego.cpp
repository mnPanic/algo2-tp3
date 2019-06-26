//
// Created by march on 11/06/19.
//

#include "EstadoJuego.h"
#include <modelo/TiposJuego.h>
#include <sstream>
#include <cctype>
#include "modulos_basicos/linear_set.h"
#include "../gtest-1.8.1/gtest.h"

enum ActorType { F, J };

struct ActorStat {
  int id;
  ActorType type;
  PosYDir stat;

  ActorStat(int _id, ActorType _type, PosYDir _stat)
      : id(_id), type(_type), stat(_stat) {}
};

ActorStat parse(char x, int row, int col) {
  ActorType type;
  if (tolower(x) == x) {
    type = ActorType::F;
  } else {
    type = ActorType::J;
  }

  Dir d;
  switch(tolower(x)) {
    case 'q':
    case 'a':
    case 'z':
      d = IZQUIERDA;
      break;
    case 'w':
    case 's':
    case 'x':
      d = ARRIBA;
      break;
    case 'e':
    case 'd':
    case 'c':
      d = ABAJO;
      break;
    case 'r':
    case 'f':
    case 'v':
      d = DERECHA;
      break;
  }

  int id;
  switch(tolower(x)) {
    case 'q':
    case 'w':
    case 'e':
    case 'r':
      id = 0;
      break;
    case 'a':
    case 's':
    case 'd':
    case 'f':
      id = 1;
      break;
    case 'z':
    case 'x':
    case 'c':
    case 'v':
      id = 2;
      break;
  }

  return ActorStat(id, type, PosYDir(Pos(col, row), d));
}

EstadoJuego::EstadoJuego(unsigned int jugs, const string&& s) : h(string_to_hab(s)), pos_fe(pos_f.end()) {
  std::stringstream is(s);

  int max_f = -1;
  int row = 0;
  int col = 0;
  char c;
  while (is.get(c)) {
    if (c != '.' and c != '#' and c != '\n') {
      ActorStat a = parse(c, h.tam() - row - 1, col);
      if (a.type == ActorType::J) {
        pos_j.insert(make_pair(to_string(a.id), a.stat));
      } else {
        linear_set<PosYDir>::iterator it = pos_f.insert(a.stat).first;
        if (a.id > max_f) {
          pos_fe = it;
          max_f = a.id;
        }
      }
      col++;
    } else if (c == '\n') {
      row++;
      col = 0;
    } else {
      col++;
    }
  }

  for (int i = 0; i < jugs; i++) {
    if (pos_j.count(to_string(i))) {
      j_vivos.push_back(to_string(i));
    } else {
      j_muertos.push_back(to_string(i));
    }
  }
}

void EstadoJuego::verificar(const ExtremeExorcism &ee) {
  list<PosYDir> fs_l = ee.posicionFantasmas();
  linear_set<PosYDir> fs = linear_set<PosYDir>(fs_l.begin(), fs_l.end());

  map<Jugador, PosYDir> js;
  for (auto js_pd : ee.posicionJugadores()) {
    js.insert(js_pd);
  }

  EXPECT_EQ(pos_j, js);
  EXPECT_EQ(pos_f, fs);
  EXPECT_EQ(*pos_fe, ee.posicionEspecial());

  for (Jugador j : j_vivos) {
    EXPECT_TRUE(ee.jugadorVivo(j));
  }

  for (Jugador j : j_muertos) {
    EXPECT_FALSE(ee.jugadorVivo(j));
  }

  EXPECT_EQ(h, ee.habitacion());
}

