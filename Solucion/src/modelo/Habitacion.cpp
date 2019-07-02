
#include "Habitacion.h"
#include "assert.h"
#include <istream>
#include <sstream>
#include <string>

Habitacion::Habitacion(unsigned int tam, set<Pos> ocupadas) : tamanio(tam), casilleros(vector<vector<bool>> (tam, vector<bool>(tam, false))){
    for(Pos p : ocupadas) {
        casilleros[p.first][p.second] = true;
    }
}

unsigned int Habitacion::tam() const {
    return tamanio;
}

bool Habitacion::ocupado(Pos pos) const {
    return casilleros[pos.first][pos.second];
}

bool Habitacion::operator==(const Habitacion & h) const {
    return casilleros == h.casilleros;
}

bool Habitacion::valida(Pos pos) const {
    return 0 <= pos.first < tamanio && 0 <= pos.second < tamanio && !casilleros[pos.first][pos.second];
}

Habitacion string_to_hab(std::istream& is) {
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    Habitacion h(height, occupied);
    return h;
}

Habitacion string_to_hab(string s) {
    std::istringstream is(s);
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    Habitacion h(height, occupied);
    return h;
}

// Completar
