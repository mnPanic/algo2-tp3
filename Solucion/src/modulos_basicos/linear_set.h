#ifndef linear_set_h
#define linear_set_h

#include <utility>
#include <ostream>
#include "linear_map.h"

using namespace std;

namespace algo2 {
/**
 * @brief Conjunto con inserción en tiempo constante
 *
 * @tparam T tipo paramétrico del conjunto
 * Debe tener:
 *  - operator==
 *  - Constructor por copia
 *
 *  **se explica con** TAD Conjunto(T)
 */
template<typename T>
class linear_set {
 public:
  using value_type = T;
  using size_type = size_t; 
  // Forward declarations
  class iterator;
  class const_iterator; 

  /**
   * @brief Crea un conjunto vacio.
   *
   * \pre true
   * \post \P{this} = \EMPTYSET
   *
   * \complexity{\O(1)}
   */
  linear_set();

  /**
   * @brief Crea un conjunto con los valores parámetros.
   *
   * \pre true
   * \post (\FORALL x : T) x \IN \P{this} \IFF está?(x, init)
   *
   * \complexity{\O(long(init) * copy(T))}
   */
  linear_set(std::initializer_list<value_type> init);

  /**
   * @brief Crea un conjunto con los valores parámetros.
   *
   * \pre first y last son iteradores del mismo contenedor
   * \post \P{this} tiene solo los valores en el rango first -> last
   *
   * \complexity{\O(|last - first| * copy(T))}
   */
  template<class InputIt>
  linear_set(InputIt first, InputIt last);

  /** 
   * @brief Constructor por copia
   *
   * \pre true
   * \post \P{this} es una copia de other. No hay aliasing.
   *
   * \complexity{\O(#claves(ohter) * copy(T))}
   */
  linear_set(const linear_set<T> &other);

  /**
   * @brief Agrega el elemento al conjunto.
   *
   * El elemento no se agrega si ya estaba.
   *
   * \pre c = \P{this}
   * \post \P{this} = Ag(x, c) y \P1(\P{res}) apunta al elemento agregado y
   * \P2(\P{res}) es true si x no estaba ya en el conjunto.
   *
   * \complexity{\O(#claves(\P{this}) + copy(v))}
   */
  pair<iterator, bool> insert(const T &x);   

  /**
   * @brief Agrega el elemento al conjunto
   *
   * \pre c = \P{this} \LAND \LNOT x \IN \P{this}
   * \post \P{this} = Ag(x, c) y \P{res} apunta al elemento agregado.
   *
   * \complexity{\O(copy(v))}
   */
  iterator fast_insert(const value_type& v);

  /**
   * @brief Cantidad de apariciones del valor.
   *
   * \pre true
   * \post (x \IN \P{this} \LAND \P{res} = 1) \LOR (x \NOT \IN \P{this} \LAND
   * \P{res} = 0)
   *
   * \complexity{\O(n) * cmp(T)}
   */
  size_type count(const T& x) const;

  /**
   * @brief True si el conjunto está vacío.
   *
   * \pre true
   * \post \P{res} = \EMPTYSET?(\P{this})
   *
   * \complexity{\O(1)}
   */
  bool empty() const;  

  /**
   * @brief Tamaño del conjunto.
   *
   * \pre true
   * \post \P{res} = #clave(\P{this})
   *
   * \complexity{\O(1)}
   */
  size_type size() const;

  /**
   * @brief Elimina el valor del conjunto. Devuelve la cantidad de
   * elementos elminados.
   *
   * \pre c == \P{this} \LAND x \IN \P{this}
   * \post \P{this} == c - {x} \LAND \P{res} = 1 
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  size_type erase(const T &x); 

  /**
   * @brief Devuelve un iterador relacionado al valor buscado.
   * End si no está definida.
   *
   * \pre true
   * \post 
   *       * x \IN \P{this} \IMPLIES \P{res} apunta a la aparición de x en el
   *       conjunto
   *       * \NEG x \IN \P{this} \IMPLIES \P{res} es end
   *
   * \complexity{\O(#claves(\P{this}))}
   */
  iterator find(const T &x);

  /** 
   * @brief Operador asignación del conjunto 
   *
   * \pre true
   * \post \P{this} == other y \P{res} refiere a \P{this}
   *
   * \complexity{\O(#claves(\P{this}) + #(other) * copy(T))}
   */
  linear_set<T>& operator=(const linear_set<T> &other);

  /**
   * @brief True si los conjuntos son iguales.
   *
   * \complexity{\O(#(\P{this}) * #(other))}
   */
  bool operator==(const linear_set<T> &other) const;

  /**
   * @brief Devuelve un iterador al inicio del conjunto.
   *
   * \pre true
   * \post \P{res} apunta al inicio del conjunto 
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
   * @brief Devuelve un iterador const al inicio del conjunto.
   *
   * \pre true
   * \post \P{res} apunta al inicio del conjunto 
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

 private:
  /**
   * \name Representación:
   *
   * rep: linear_set(T) \TO bool\n
   * rep(c) \EQUIV
   *  * \FORALL (t : T) 
   *    (def?(t, _elems) \IMPLIES obtener(t, _elems))
   *
   * abs: linear_set(T) \TO Conj(T)\n
   * abs(c) \EQUIV c' \|
   *  * \FORALL (x : T) x \IN c' \IFF def?(c, c)
   */ 
  /** @{ */
  linear_map<T, bool> _elems;
  /** @} */
};

}
  
template<class T>
ostream &operator<<(ostream &, const algo2::linear_set<T> &);

#include "linear_set.hpp"

#include "linear_set_iterators.h"

#endif // linear_set_h
