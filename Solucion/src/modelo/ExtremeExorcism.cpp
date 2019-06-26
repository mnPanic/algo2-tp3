#include "ExtremeExorcism.h"

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init, list<Accion> acciones_fantasma,
        Contexto *ctx) {}

void ExtremeExorcism::pasar() {}

void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a) {}

list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {}

list<PosYDir> ExtremeExorcism::posicionFantasmas() const {}

PosYDir ExtremeExorcism::posicionEspecial() const {}

list<PosYDir> ExtremeExorcism::disparosFantasmas() const {}

set<Pos> ExtremeExorcism::posicionesDisparadas() const {}

bool ExtremeExorcism::jugadorVivo(Jugador j) const {}

const Habitacion& ExtremeExorcism::habitacion() const {}

PosYDir ExtremeExorcism::posicionJugador(Jugador j) const {}

const set<Jugador>& ExtremeExorcism::jugadores() const {}

const list<Fantasma>& ExtremeExorcism::fantasmas() const {}