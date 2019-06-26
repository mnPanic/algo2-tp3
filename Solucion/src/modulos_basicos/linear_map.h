#ifndef linear_map_h
#define linear_map_h

#include <list>
#include <iostream>

using namespace std;

namespace algo2 { 

/**
 * @brief Módulo diccionario con inserción en O(1)
 *
 * @tparam K tipo paramétrico para la clave
 * @tparam S tipo paramétrico para el significado
 *
 * Se espera que K tenga:
 * - operator==
 * - Constructor por copia
 *
 * Se espera que S tenga:
 * - Constructor por copoa
 *
 * **se explica con** TAD Diccionario(K, S)
 */
template<class K, class S>
class linear_map {
 public:
  using key_type = K;
  using mapped_type = S;
  using value_type = pair<const K, S>;
  using size_type = size_t;

  // Forward declaration
  using iterator = typename list<pair<const K, S>>::iterator;
  using const_iterator = typename list<pair<const K, S>>::const_iterator;

  /**
   * @brief Crea un diccionario vacio.
   *
   * \pre true
   * \post \P{this} = vacio
   *
   * \complexity{\O(1)}
   */
  linear_map();

  /** 
   * @brief Constructor por copia
   *
   * \pre true
   * \post \P{this} es una copia de other. No hay aliasing.
   *
   * \complexity{\O(#claves(ohter) * (copy(Clave) + copy(Significado)))}
   */
  linear_map(const linear_map<K, S>& other);

  /**
   * @brief Define o redefine una relación en el diccionario.
   *
   * \pre d = \P{this}
   * \post \P{this} = definir(d, \P1(v), \P2(v)) \LAND \P{res} es una tupla con
   * el primer elemento un iterador a la relación definida y el segúndo elemento
   * indica si \P1(v) no era clave del diccionario
   *
   * \complexity{\O(#claves(\P{this}) + copy(v))}
   */
  pair<iterator, bool> insert(const value_type& v);

  /**
   * @brief Define una nueva relación en el diccionario.
   *
   * \pre d = \P{this} \LAND \LNOT def?(\P1(v), \P{this})
   * \post \P{this} = definir(d, \P1(v), \P2(v)) \LAND \P{res} es un iterador
   *       que apunta a la relación recién definida
   *
   * \complexity{\O(copy(v))}
   */
  iterator fast_insert(const value_type& v);
  
  /**
   * @brief Tamaño del diccionario.
   *
   * \pre true
   * \post \P{res} = #clave(\P{this})
   *
   * \complexity{\O(1)}
   */
  size_type size() const;

  /**
   * @brief Devuelve el significado relacionado con la clave.
   *
   * El significado se devuelve por referencia modificable.
   *
   * \pre def?(key, \P{this})
   * \post \P{res} = obtener(\P{this}, k)
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  S& at(const K& key);

  /**
   * @brief True si el diccionario está vacío.
   *
   * \pre true
   * \post \P{res} = \EMPTYSET?(claves(\P{this}))
   *
   * \complexity{\O(1)}
   */
  bool empty() const {
    return _elems.empty();
  }

  /**
   * @brief Devuelve un iterador relacionado a la clave buscada. 
   * End si no está definida.
   *
   * \pre true
   * \post 
   *       * def?(k, \P{this}) \IMPLIES \P1(\P{res}) = k \LAND 
   *                                    \P2(\P{res}) = obtener(k, \P{this}) \LOR
   *       * \NEG def?(k, \P{this}) \IMPLIES \P{res} es end
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  iterator find(const K &k);
  
  /**
   * @brief Devuelve un iterador const relacionado a la clave buscada. 
   * End si no está definida.
   *
   * \pre true
   * \post 
   *       * def?(k, \P{this}) \IMPLIES \P1(\P{res}) = k \LAND 
   *                                    \P2(\P{res}) = obtener(k, \P{this}) \LOR
   *       * \NEG def?(k, \P{this}) \IMPLIES \P{res} es end
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  const_iterator find(const K &k) const;

  /**
   * @brief Devuelve un iterador al inicio del diccionario.
   *
   * \pre true
   * \post \P{res} apunta al inicio del diccionario
   *
   * \complexity{\O(1)}
   */
  iterator begin();

  /**
   * @brief Devuelve un iterador a la posición pasando-el-último
   *
   * \pre true
   * \post \P{res} apunta a la posición pasando-el-último
   *
   * \complexity{\O(1)}
   */
  iterator end();
  
  /**
   * @brief Devuelve un iterador const al inicio del diccionario.
   *
   * \pre true
   * \post \P{res} apunta al inicio del diccionario
   *
   * \complexity{\O(1)}
   */
  const_iterator begin() const;

  /**
   * @brief Devuelve un iterador const a la posición pasando-el-último
   *
   * \pre true
   * \post \P{res} apunta a la posición pasando-el-último
   *
   * \complexity{\O(1)}
   */
  const_iterator end() const;

  /**
   * @brief Cantidad de apariciones de la clave.
   *
   * \pre true
   * \post \IF def?(\P{this}, k) \THEN \P{res} = 1 \ELSE \P{res} = 0 \FI
   *
   * \complexity{\O(#claves(\P{this}) * cmp(Clave)}
   */
  size_t count(const K &k) const;

  /**
   * @brief Devuelve el significado relacionado con la clave.
   *
   * El significado se devuelve por referencia no-modificable.
   *
   * \pre def?(key, \P{this})
   * \post \P{res} = obtener(\P{this}, k)
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  const S& at(const K& key) const; 

  /**
   * @brief Elimina el significado del diccionario. Devuelve la cantidad de
   * elementos elminados.
   *
   * \pre d == \P{this} \LAND def?(key, \P{this})
   * \post \P{this} == borrar(key, d) \LAND \P{res} = 1
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  size_type erase(const K& key);
  
  /** 
   * @brief Operador asignación del diccionario
   *
   * \pre true
   * \post \P{this} == other \LAND \P{res} es referencia a \P{this}
   *
   * \complexity{\O(#claves(\P{this}) + #claves(other) * (copy(Clave) +
   * copy(Significado)))}
   */
  linear_map& operator=(const linear_map& other);
  
  /**
   * @brief True si los diccionarios son iguales.
   *
   * \complexity{\O(#claves(\P{this}) * #claves(other))}
   */
  bool operator==(const linear_map& other) const;

 private:
  /**
   * \name Representación:
   *
   * rep: linear_map(Clave, Significado) \TO bool\n
   * rep(d) \EQUIV
   *  * \FORALL (i, j : Nat) 
   *    (0 \LEQ i, j < long(_elems) \LAND \P1(_elems[i]) == \P1(_elems[j]))
   *    \IMPLIES i = j
   *
   * \BULLET [\BULLET] : Secu(\ALPHA) s \TIMES Nat i \RIGHTARROW \ALPHA  {0 \LEQ i <
   * long(s)} \n
   * s[i] \EQUIV \IF i = 0 \THEN prim(s) \ELSE fin(s)[i - 1] \FI
   *
   * abs: linear_map(Clave, Significado) \TO Dicc(Clave, Significado)\n
   * abs(d) \EQUIV d' \|
   *  * \FORALL (c : Clave) def?(c, d') \IFF \EXISTS 
   *    (t : tupla(Clave, Significado)) está?(t, _elems) \LAND P1(t) == c
   *  * \FORALL (t : tupla(Clave, Significado)) está?(t, _elems) \IMPLIES
   *    P2(t) == obtener(P1(t), d')
   */
  /** @{ */
  list<pair<const K, S>> _elems;
  /** @} */
};

}

template<class K, class S>
std::ostream &operator<<(std::ostream& os, const algo2::linear_map<K, S> &m);


#include "linear_map.hpp"

#endif // linear_map_h
