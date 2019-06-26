#include "localizador_aleatorio.h"

template <class Collection>
typename Collection::const_iterator sample(const Collection &c) {
  int size = c.size();
  float r = rand();
  int rand_idx = (size * r) / RAND_MAX;
  typename Collection::const_iterator it = c.begin();
  while (rand_idx != 0) {
    it++;
    rand_idx--;
  }
  return it;
}

    
map<Jugador, PosYDir>
LocalizadorAleatorio::localizar_jugadores(const set<Jugador> &jugadores,
                                          const list<Fantasma> &fantasmas,
                                          const Habitacion &hab){

  list<Dir> dir_validas = list<Dir>{ARRIBA, ABAJO, IZQUIERDA, DERECHA};

  list<Pos> pos_validas;
  for (int i = 0; i < hab.tam(); i++) {
    for (int j = 0; j < hab.tam(); j++) {
      if (not hab.ocupado(Pos(i, j))) {
        pos_validas.push_back(Pos(i, j));
      }
    }
  }

  map<Jugador, PosYDir> ret;
  for (Jugador j : jugadores) {
    list<Pos>::const_iterator cit = sample(pos_validas);
    Dir d = *sample(dir_validas);

    ret.insert(make_pair(j, PosYDir(*cit, d)));
    pos_validas.erase(cit);
  }

  return ret;
}
