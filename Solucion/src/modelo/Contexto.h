#ifndef INC_2019_C1_TP3_CONTEXT_H
#define INC_2019_C1_TP3_CONTEXT_H

#include "modelo/TiposJuego.h"
#include "modelo/localizadores/localizador.h"
#include "modelo/localizadores/localizador_aleatorio.h"
#include <map>
#include <list>

using namespace std;

class Contexto {

public:

    Contexto(Localizador* l=&localizador_aleatorio);

    map<Jugador, PosYDir> localizar_jugadores(const set<Jugador>& jugadores,
            const list<Fantasma>& fantasmas, const Habitacion& hab);

private:

    Localizador* _l;
};

#endif //INC_2019_C1_TP3_CONTEXT_H
