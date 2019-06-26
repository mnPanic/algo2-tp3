#include "Contexto.h"
#include "ExtremeExorcism.h"
#include <list>
#include <utility>

using namespace std;

Contexto::Contexto(Localizador* l) : _l(l) {}

map<Jugador, PosYDir> Contexto::localizar_jugadores(const set<Jugador>&
        jugadores, const list<Fantasma>& fantasmas, const Habitacion& hab) {
    return _l->localizar_jugadores(jugadores, fantasmas, hab);
}
