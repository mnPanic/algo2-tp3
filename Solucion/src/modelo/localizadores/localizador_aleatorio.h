#ifndef _LOCALIZADOR_ALEATORIO_H_
#define _LOCALIZADOR_ALEATORIO_H_

#include "localizador.h"

class LocalizadorAleatorio : public Localizador {

public:

  map<Jugador, PosYDir> localizar_jugadores(const set<Jugador> &jugadores,
                                            const list<Fantasma> &fantasmas,
                                            const Habitacion &hab);
};

static LocalizadorAleatorio localizador_aleatorio;

#endif
