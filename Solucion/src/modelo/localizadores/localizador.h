#ifndef _LOCALIZADOR_H_
#define _LOCALIZADOR_H_

#include "modelo/Habitacion.h"
#include <list>
#include <map>
#include <set>

class Localizador {

public:
  Localizador();

  virtual map<Jugador, PosYDir>
  localizar_jugadores(const set<Jugador> &jugadores,
                      const list<Fantasma> &fantasmas, const Habitacion &hab);
};

#endif
