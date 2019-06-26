#ifndef linear_set_iterators_h
#define linear_set_iteratros_h

#include "linear_set.h"
#include <iostream>

using namespace std;

using namespace algo2;


template<class T>
class linear_set<T>::const_iterator {
 public:
  using value_type = linear_set::value_type;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type&;                                                                                           
  using pointer = value_type*;                                                                                               
  using difference_type = std::ptrdiff_t;               

  /**
   * @brief Constructor por copia del iterador.
   *
   * \complexity{\O(1)}
   */
  const_iterator(const typename linear_set<T>::const_iterator&);
  
  /**
   * @brief Conversión desde iterator
   *
   * \complexity{\O(1)}
   */
  const_iterator(const typename linear_set<T>::iterator&);
  
  /**
   * @brief Avanza el iterador una posición.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
   * siguiente.
   *
   * \complexity{\O(1)}
   */
  linear_set::const_iterator &operator++();

  /**
   * @brief Desreferencia el puntero
   *
   * El valor devuelto tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es una referencia constante al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const value_type &operator*() const;

  /**
   * @brief Operador flechita
   *
   * El valor devuelvo tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es un puntero al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const value_type* operator->() const;

  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator==(const linear_set<T>::const_iterator &other) const;

  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores no apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator!=(const linear_set<T>::const_iterator &other) const;

 private:
  friend class linear_set<T>;

  /**
   * @brief Constructor del iterador a partir de un iterador interno.
   */
  const_iterator(const typename linear_map<T, bool>::const_iterator&);

  typename linear_map<T, bool>::const_iterator it;
};

template<class T>
class linear_set<T>::iterator {
 public:
  using value_type = const linear_set::value_type;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type&;                                                                                           
  using pointer = value_type*;                                                                                               
  using difference_type = std::ptrdiff_t;               

  /**
   * @brief Constructor por copia del iterador.
   *
   * \complexity{\O(1)}
   */
  iterator(const typename linear_set<T>::iterator&);
  
  /**
   * @brief Avanza el iterador una posición.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
   * siguiente.
   *
   * \complexity{\O(1)}
   */
  linear_set::iterator &operator++();

  /**
   * @brief Desreferencia el puntero
   *
   * El valor devuelto tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es una referencia al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const value_type &operator*() const;

  /**
   * @brief Operador flechita
   *
   * El valor devuelvo tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es un puntero al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const value_type* operator->() const;

  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator==(const linear_set<T>::iterator &other) const;

  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores no apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator!=(const linear_set<T>::iterator &other) const;
  
 private:
  friend class linear_set<T>;

  /**
   * @brief Constructor del iterador a partir de un iterador interno.
   */
  iterator(const typename linear_map<T, bool>::iterator&);

  typename linear_map<T, bool>::iterator it;
};


// const_iterator methods

template<typename T>
linear_set<T>::const_iterator::const_iterator(
    const typename linear_set<T>::const_iterator &other) : it(other.it) {}

template<typename T>
linear_set<T>::const_iterator::const_iterator(
    const typename linear_set<T>::iterator &other) : it(other.it) {}

template<typename T>
typename linear_set<T>::const_iterator &linear_set<T>::const_iterator::operator++() {
  it++;
  return *this;
}

template<typename T>
const typename linear_set<T>::const_iterator::value_type &linear_set<T>::const_iterator::operator*() const {
  return it->first;
}

template<typename T>
const typename linear_set<T>::const_iterator::value_type *linear_set<T>::const_iterator::operator->() const {
  return &it->first;
}

template<typename T>
bool linear_set<T>::const_iterator::operator==(const linear_set<T>::const_iterator &other) const {
  return it == other.it;
}

template<typename T>
bool linear_set<T>::const_iterator::operator!=(const linear_set<T>::const_iterator &other) const {
  return not (*this == other);
}

template<typename T>
linear_set<T>::const_iterator::const_iterator(
    const typename linear_map<T, bool>::const_iterator& _it)
  : it(_it) {};


// iterator methods

template<typename T>
linear_set<T>::iterator::iterator(const typename linear_set<T>::iterator& other)
  : it(other.it) {}

template<typename T>
linear_set<T>::iterator::iterator(const typename linear_map<T, bool>::iterator& _it)
  : it(_it) {};

template<typename T>
typename linear_set<T>::iterator &linear_set<T>::iterator::operator++() {
  it++;
  return *this;
}

template<typename T>
const typename linear_set<T>::iterator::value_type &linear_set<T>::iterator::operator*() const {
  return it->first;
 }

template<typename T>
const typename linear_set<T>::iterator::value_type *linear_set<T>::iterator::operator->() const {
  return &it->first;
}

template<typename T>
bool linear_set<T>::iterator::operator==(const linear_set<T>::iterator &other) const {
  return it == other.it;
}

template<typename T>
bool linear_set<T>::iterator::operator!=(const linear_set<T>::iterator &other) const {
  return not (*this == other);
}

#endif // linear_set_iterators_h
