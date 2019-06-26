#include "linear_set.h"

using namespace algo2;

template<class T>
linear_set<T>::linear_set() : _elems() {};

template<class T>
linear_set<T>::linear_set(std::initializer_list<value_type> init) {
    for (auto x : init) {
      _elems.insert(make_pair(x, true));
    }
  }
  
template<class T>
template<class InputIt>
linear_set<T>::linear_set(InputIt first, InputIt last) {
  for (auto it = first; it != last; ++it) {
    _elems.insert(make_pair(*it, true));
  }
}

template<class T>
linear_set<T>::linear_set(const linear_set<T> &other) : _elems(other._elems) {}

template<class T>
pair<typename linear_set<T>::iterator, bool> linear_set<T>::insert(const T &x) {
  auto ret = _elems.insert(make_pair(x, true));
  return make_pair(iterator(ret.first), ret.second);
}
  
template<class T>
typename linear_set<T>::iterator linear_set<T>::fast_insert(const typename linear_set<T>::value_type& v) {
  auto it = _elems.fast_insert(make_pair(v, true));
  return iterator(it);
}

template<class T>
typename linear_set<T>::size_type linear_set<T>::count(const T& x) const {
  return _elems.count(x);
}

template<class T>
bool linear_set<T>::empty() const {
  return _elems.empty();
}

template<class T>
typename linear_set<T>::size_type linear_set<T>::size() const {
  return _elems.size();
}
  
template<class T>
typename linear_set<T>::size_type linear_set<T>::erase(const T &x) {
  return _elems.erase(x);
}

template<class T>
linear_set<T>& linear_set<T>::operator=(const linear_set<T> &other) {
  _elems = other._elems;
  return *this;
}

template<class T>
bool linear_set<T>::operator==(const linear_set<T> &other) const {
  if (other.size() != size()) {
    return false;
  } else {
    for (auto x : other) {
      if (not count(x)) {
        return false;
      }
    }
  }
  return true;
}
  
template<class T>
typename linear_set<T>::iterator linear_set<T>::begin() {
  return iterator(_elems.begin());
}
  
template<class T>
typename linear_set<T>::iterator linear_set<T>::end() {
  return iterator(_elems.end());
}

template<class T>
typename linear_set<T>::const_iterator linear_set<T>::begin() const {
  return const_iterator(_elems.begin());
}

template<class T>
typename linear_set<T>::const_iterator linear_set<T>::end() const {
  return const_iterator(_elems.end());
}

template<class T>
typename linear_set<T>::iterator linear_set<T>::find(const T &x) {
  auto _end = end();
  for (auto it = begin(); it != _end; ++it) {
    if (*it == x) {
      return it;
    }
  }
  return _end;
}

template<class T>
std::ostream &operator<<(std::ostream &os, const linear_set<T> &s) {
  os << "{ ";
  int count = s.size();
  for (auto v : s) {
    os << v;
    count--;
    if (count) {
     os << ", ";
    }
  } 
  os << " }";
  return os;
}
