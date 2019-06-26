//
// Created by march on 11/06/19.
//

#ifndef INC_2019_C1_TP3_ESTADOJUEGO_H
#define INC_2019_C1_TP3_ESTADOJUEGO_H

#include <modelo/ExtremeExorcism.h>
#include <modelo/TiposJuego.h>
#include <modulos_basicos/linear_set.h>

/**
 * Este módulo permite evaluar el estado de una instancia de ExtremeExorcism
 * mediante una descripción en ASCII del estado del juego. El estado está dado
 * por las posiciones y direcciones de los jugadores y fantasmas.
 *
 * Un estado de juego se describe por un natural definiendo la cantidad de
 * jugadores del juego y un string describiendo el mapa. Un ejemplo de una
 * descripción de mapa es la siguiente:
 *
 *    ....
 *    W##.
 *    .##e
 *    ....
 *
 * Este es un mapa de 4x4 con un cuadrado de paredes de 2x2 en el centro y el
 * jugador "0" en la posición <0, 2> mirando para arriba y un fantasma
 * en la posición <3, 1> mirando hacia abajo. Los `.` indican espacios vacios.
 * Los `#` indican paredes.
 *
 * Esta clase para validación tiene las siguientes restricciones:
 *  > Soporta hasta 3 jugadores
 *  > Soporta hasta 3 fantasmas
 *  > No permite verificar casos donde varios actores estén en el mismo
 *  casillero
 *
 * Los jugadores se describen usando letras mayúsculas donde la letra usada
 * indica la dirección del jugador. Esto se ve de la siguiente manera:
 *
 *    Jugador 0   Jugador 1   Jugador 2
 *       W           S           X
 *     Q E R       A D F       Z C V
 *
 * Los jugadores se identifican con el string de un solo caracter del número
 * dado. O sea: "0", "1" o "2".
 *
 * Los jugadores siempre tienen que tener índice menor al número pasado como
 * parámetro a EstadoJuego. Si inicio EstadoJuego indicando que hay dos
 * jugadores y solo aparece uno en el casillero, el otro se asume muerto.
 *
 * Finalmente, los fantasmas se indican de manera análoga a los jugadores pero
 * utilizando letras minúsculas. En este caso el indicador de fantasma es
 * irrelevante y el máximo se utilizara como fantasma especial.
 */
class EstadoJuego {

public:

  EstadoJuego(unsigned int jugs, const string&& s);

  void verificar(const ExtremeExorcism &ee);

private:

  Habitacion h;
  map<Jugador, PosYDir> pos_j;
  linear_set<PosYDir> pos_f;
  linear_set<PosYDir>::iterator pos_fe;
  list<Jugador> j_vivos;
  list<Jugador> j_muertos;

};

#endif //INC_2019_C1_TP3_ESTADOJUEGO_H
