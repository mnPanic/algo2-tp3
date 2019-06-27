#ifndef EXTREME_EXORCISM_H
#define EXTREME_EXORCISM_H

#include "Contexto.h"
#include "Habitacion.h"
#include "TiposJuego.h"
#include "string_map.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <modulos_basicos/linear_set.h>

using namespace std;
using pasoDisparos = pair<int, int>;

class ExtremeExorcism {
public:
  ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                  list<Accion> acciones_fantasma, Contexto *ctx);

  void pasar(); //O(#fv ∗ m + #jv)

  void ejecutarAccion(Jugador j, Accion a); //O(|pj| + m*#fv #jv)

  list<pair<Jugador, PosYDir>> posicionJugadores() const; //O(1)

  list<PosYDir> posicionFantasmas() const; //O(1)

  PosYDir posicionEspecial() const; //O(1)

  list<PosYDir> disparosFantasmas() const; //O(#fv)

  set<Pos> posicionesDisparadas() const; //O(1)

  bool jugadorVivo(Jugador j) const; //O(|pj|)

  const Habitacion &habitacion() const;

  PosYDir posicionJugador(Jugador j) const;

  const set<Jugador> &jugadores() const;

  const list<Fantasma> &fantasmas() const;

private:
  struct Juego {
      struct infoPJ;
      struct infoFan;

      //General
      int paso;
      int ronda;
      Habitacion mapa;

      //Disparos
      vector<vector<pasoDisparos>> mapaDisparos;
      set<Pos> disparosFanUltimoPaso;

      //Jugadores
      string_map<infoPJ> infoJugadores;
      list<pair<Jugador, PosYDir>> infoActualJugadoresVivos;
      algo2::linear_set<infoPJ*> infoJugadoresVivos;

      //Fantasmas
      list<Fantasma> infoFantasmas;
      list<PosYDir> infoActualFantasmasVivos;
      algo2::linear_set<list<Fantasma>::iterator> infoFantasmasVivos;
      list<PosYDir>::iterator infoFantasmaEspecial;

      struct infoPJ {
          list<Evento> eventos;
          bool vivo;
          list<pair<Jugador, PosYDir>>::iterator infoActual;
      };

      struct infoFan {
          vector<Evento> eventos;
          bool vivo;
          list<PosYDir>::iterator infoActual;
      };
  };

  Juego juego;
};

#endif
