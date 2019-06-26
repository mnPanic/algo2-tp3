//
// Created by march on 11/06/19.
//

#ifndef INC_2019_C1_TP3_LOCALIZADOR_ESTATICO_H
#define INC_2019_C1_TP3_LOCALIZADOR_ESTATICO_H

#include "modelo/localizadores/localizador.h"

class LocalizadorEstatico : public Localizador {

public:
  LocalizadorEstatico(map<Jugador, PosYDir>);
  LocalizadorEstatico(map<Ronda, map<Jugador, PosYDir>>);

  map<Jugador, PosYDir>
  localizar_jugadores(const set<Jugador> &jugadores,
                      const list<Fantasma> &fantasmas, const Habitacion &hab);

private:
  map<Ronda, map<Jugador, PosYDir>> _pos_y_dirs;
  Ronda max_ronda;
};

#endif // INC_2019_C1_TP3_LOCALIZADOR_ESTATICO_H
