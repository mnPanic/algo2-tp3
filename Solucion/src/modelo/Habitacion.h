#ifndef INC_2019_C1_TP3_HABITACION_H
#define INC_2019_C1_TP3_HABITACION_H

#include <iostream>
#include <set>
#include <vector>
#include "TiposJuego.h"

using namespace std;


class Habitacion {
public:
    Habitacion(unsigned int tam, set<Pos> ocupadas);

    unsigned int tam() const;

    bool ocupado(Pos) const;

    bool operator==(const Habitacion&) const;

private:

    vector<vector<bool>> casilleros;

    int tamanio; //Escribir con enie
};


Habitacion string_to_hab(std::istream& is);
Habitacion string_to_hab(string s);

#endif
