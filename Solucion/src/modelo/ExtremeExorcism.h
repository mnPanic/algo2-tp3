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

class ExtremeExorcism {
public:
  ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                  list<Accion> acciones_fantasma, Contexto *ctx);

  void pasar(); //O(#fv ∗ m + #jv)

  // Actualiza con acción del jugador.
  //  - Sin pasar de ronda: O(|pj| + #fv * m + #jv)
  //  - Pasando de ronda:   O(|pj| + m^2 + #f + locJugadores + #j * (|maxPj| + long(maxEvt))
  void ejecutarAccion(Jugador j, Accion a); //O(|pj| + m*#fv #jv)

  list<pair<Jugador, PosYDir>> posicionJugadores() const; //O(1)

  list<PosYDir> posicionFantasmas() const; //O(1)

  PosYDir posicionEspecial() const; //O(1)

  list<PosYDir> disparosFantasmas() const; //O(#fv)

  set<Pos> posicionesDisparadas() const; //O(1)

  bool jugadorVivo(Jugador j) const; //O(|pj|)

  const Habitacion &habitacion() const;

  PosYDir posicionJugador(const Jugador& j) const;

  const set<Jugador> &jugadores() const;

  const list<Fantasma> &fantasmas() const;

private:
    struct InfoActualPJ {
        string id;
        PosYDir local;
    };

    struct InfoPJ {
        list<Evento> eventos;
        bool vivo;
        algo2::linear_set<InfoActualPJ>::iterator infoActual;
    };

    typedef PosYDir InfoActualFan;

    struct InfoFan {
        InfoFan(typename algo2::linear_set<InfoActualFan>::iterator it) : infoActual(it) {};
        vector<Evento> eventos;
        bool vivo;
        algo2::linear_set<InfoActualFan>::iterator infoActual;
    };

    struct PasoDisparo {
        PasoDisparo(int pj, int fan);

        int fan;
        int pj;
    };

    struct Juego {
        Juego(Habitacion h);

        //General
        int paso;
        int ronda;
        Habitacion mapa;

        //Disparos
        vector<vector<PasoDisparo>> mapaDisparos;
        algo2::linear_set<Pos> disparosFanUltimoPaso;

        //Jugadores
        string_map<InfoPJ> infoJugadores;
        algo2::linear_set<InfoActualPJ> infoActualJugadoresVivos;
        algo2::linear_set<InfoPJ*> infoJugadoresVivos;

        //Fantasmas
        algo2::linear_set<InfoFan> infoFantasmas;
        algo2::linear_set<InfoActualFan> infoActualFantasmasVivos;
        algo2::linear_set<algo2::linear_set<InfoFan>::iterator> infoFantasmasVivos;
        //algo2::linear_set<InfoActualFan>::iterator infoFantasmaEspecial;
        InfoActualFan* infoFantasmaEspecial;
    };

    Juego juego;
    Contexto ctx;


    //// Funciones auxiliares

    // Devuelve una referencia al PasoDisparo de la posición dada en el mapa de disparos.
    // O(1)
    PasoDisparo& pasoDisparoEn(Pos pos);


    // Actualiza la info del personaje y devuelve una referencia a ella.
    // O(|pj|)
    InfoPJ& actualizarPJ(const Jugador& j, Accion a);

    // Devuelve el evento que corresponde al paso actual de un jugador.
    // O(1)
    Evento eventoActualPJ(InfoPJ) const;

    // Devuelve el evento que corresponde al paso actual de un fantasma.
    // O(1)
    Evento eventoActualFan(InfoFan info, int paso);

    //Inicializa los jugadores
    // O(#pjs+|pjMasLargo|)
    void iniciarJugadores(const set<Jugador>&);

    // Agrega un nuevo fantasma especial.
    // O(long(eventosFan)^2)
    void nuevoFanEspecial(const vector<Evento>& eventosFan);

    // Reinicia la lista de disparos de fantasmas.
    // O(1)
    void reiniciarDisparosFan();

    // Si el personaje dispara, agrega al mapa de disparos las posiciones
    // afectadas por él.
    // O(m)
    void actualizarMapaDisparosConPJ(Evento eventoPJ);

    // Agrega al mapa de disparos las posiciones afectadas por disparar desde
    // `pos` en la dirección `dir`.
    // Si es un fantasma, se agregan a la lista de posiciones afectadas por
    // disparos de fantasmas.
    // O(m)
    void agregarDisparo(Pos pos, Dir dir, bool esFan);

    // Mata a los fantasmas que están en posiciones afectadas por disparos
    // de jugadores en el paso actual.
    // O(#fv)
    bool chequearMuerteFantasmas();

    // Dice si un fantasma que está en la posición especificada será afectado
    // por un disparo en el paso actual.
    // O(1)
    bool fanAfectadoPorDisparo(Pos pos);

    // Saca el fantasma de las estructuras que solo contienen fantasmas vivos.
    // Devuelve si el fantasma que murió era el especial.
    // O(1)
    bool muereFan(linear_set<linear_set<InfoFan>::iterator>::iterator itFanVivos);

    // Cambia la ronda, agregando un fantasma especial y
    // reiniciando todas las estructuras
    // O(m^2 + #f + locJugadores + #j * (|maxPJ| + long(maxEvt))
    void nuevaRonda(const InfoPJ& pjMatoFanEspecial);

    // Reinicia los disparos del mapa de disparos.
    // O(m^2)
    void reiniciarMapaDisparos();

    // Agrega todos los fantasmas a las estructuras de vivos.
    // O(#f)
    void reiniciarFantasmas();

    // Agrega todos los jugadores a las estructuras de vivos
    // O(locJugadores + #j * (|maxPJ| + long(maxEvt))
    void reiniciarJugadores();

    // Genera una lista con un solo elemento,
    // el correspondiente a la localización.
    // O(1)
    list<Evento> crearEventosConLocalizacion(PosYDir localizacion);

    // Actualiza las acciones de los fantasmas, actualizando el mapa de
    // disparos si es que disparan.
    // O(#fv * m)
    void actualizarFantasmas();

    // Actualiza la información actual del fantasma,
    // y devuelve su nuevo evento actual.
    // O(1)
    Evento actualizarFan(InfoFan& info, int paso);

    // Mata a todos los pjs que se vean afectados por disparos.
    // O(#jv)
    void chequearMuerteJugadores();

    // Dice si un PJ se vé afectado por un disparo en este paso.
    // O(1)
    bool pjAfectadoPorDisparo(Pos pos);

    // Mata a un personaje, eliminandolo de las estructuras de vivos.
    // O(1)
    void muerePJ(linear_set<InfoPJ*>::iterator itPJVivos);

    // Agrega 'pasar' como accion a los jugadores que no hayan realizado
    // una acción en este paso.
    // O(#jv)
    void agregarPasarFaltantes();

    // Convierte una lista en un vector.
    // O(n)
    vector<Evento> vectorizar(list<Evento> l);

    // TODO: Mover a acción? NO, DEJÉMOSLÓ ACÁ PORQUE APLICAR RECIBE UN JUEGO.
    Evento aplicar(Accion a, Evento eventoActual);
    Pos avanzar(Pos p, Dir d);


    list<Evento> vecToList(vector<Evento> vector) const;
};

#endif
