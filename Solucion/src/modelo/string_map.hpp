#include "string_map.h"
#include "stack"

template <typename T>
string_map<T>::string_map(){
    raiz = new Nodo();
    _size = 0;
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::referencia(string_map<T>::Nodo *a, string_map<T>::Nodo *d) {
    if(d->definicion != NULL && a != raiz) {
        a->definicion = new T();
        *(a->definicion) = *(d->definicion);
    }
    for(int i = 0; i < d->siguientes.size(); i++) {
        if(d->siguientes[i] != NULL) {
            a->siguientes[i] = new Nodo();
            referencia(a->siguientes[i], d->siguientes[i]);
        }
    }
}

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    borradoTotal(raiz);
    delete raiz;
    raiz = new Nodo(d.raiz->definicion);
    referencia(raiz, d.raiz);
}

template <typename T>
void string_map<T>::borradoTotal(string_map<T>::Nodo *a) {
    if(a != NULL) {
        if(a->definicion != NULL) {
            delete a->definicion;
            a->definicion = NULL;
        }
        for(int i = 0; i < a->siguientes.size(); i++) {
            if(a->siguientes[i] != NULL){
                borradoTotal(a->siguientes[i]);
                delete a->siguientes[i];
                a->siguientes[i] = NULL;
            }
        }
    }
}

template <typename T>
string_map<T>::~string_map(){
    borradoTotal(raiz);
    delete raiz;
    raiz = NULL;
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    if(count(clave) == 0) {
        if(raiz == NULL) {
            raiz = new Nodo();
        }
        Nodo* actual = raiz;
        for(char c : clave) {
            if(actual->siguientes[int(c)] == NULL) {
                actual->siguientes[int(c)] = new Nodo();
            }
            actual = actual->siguientes[int(c)];
        }
        actual->definicion = new T();
        _size++;
        return *(actual->definicion);
    } else {
        return at(clave);
    }
}


template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo* actual = raiz;
    for(char c : clave) {
        if(actual->siguientes[int(c)] == NULL) {
            return 0;
        } else {
            actual = actual->siguientes[int(c)];
        }
    }

    return actual->definicion != NULL;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    for(char c : clave) {
        actual = actual->siguientes[int(c)];
    }
    return  *(actual->definicion);
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    for(char c : clave) {
        actual = actual->siguientes[int(c)];
    }
    return  *(actual->definicion);
}

template <typename T>
int string_map<T>::cantHijos(string_map<T>::Nodo *a) const{
    int canthijos = 0;
    for(int i = 0; i< a->siguientes.size(); i++) {
        if(a->siguientes[i]!=NULL) {
            canthijos++;
        }
    }
    return canthijos;
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* borrarDesde = NULL;
    Nodo* actual = raiz;
    for(char c : clave) {
        if(cantHijos(actual) > 1 || actual->definicion != NULL) {
            borrarDesde = actual->siguientes[int(c)];
        }
        actual = actual->siguientes[int(c)];
    }
    delete actual->definicion;
    actual->definicion = NULL;
    if(borrarDesde != NULL){
        borradoTotal(borrarDesde);
    }
    _size--;
}

template <typename T>
int string_map<T>::size() const{
    return _size;
}

template <typename T>
bool string_map<T>::empty() const{
    return raiz == NULL || (cantHijos(raiz) == 0 && raiz->definicion == NULL);
}