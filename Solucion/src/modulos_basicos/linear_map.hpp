#include "linear_map.h"
#include <utility>

using namespace std;
using namespace algo2;

template<class K, class S>
linear_map<K, S>::linear_map() : _elems() {}

template<class K, class S>
linear_map<K, S>::linear_map(const linear_map<K, S>& other) : 
    _elems(other._elems) {}


template<class K, class S>
pair<typename linear_map<K, S>::iterator, bool>
  linear_map<K, S>::insert(const linear_map<K, S>::value_type& v) {
    for (linear_map<K, S>::iterator it = _elems.begin(); it != _elems.end(); ++it) {
      if (it->first == v.first) {
        it->second = v.second;
        return make_pair(it, false);
      } 
    }
    auto it = _elems.insert(_elems.end(), v);
    return make_pair(it, true); 
  }

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::fast_insert(const linear_map<K, S>::value_type& v) {
  auto it = _elems.insert(_elems.end(), v);
  return it; 
}
  
template<class K, class S>
linear_map<K, S>& linear_map<K, S>::operator=(const linear_map<K, S>& other) {
  _elems.clear();
  for (auto v : other._elems) {
    _elems.push_back(v);
  }
  return *this;
}

template<class K, class S>
typename linear_map<K, S>::size_type linear_map<K, S>::size() const {
  return _elems.size();
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::find(const K &k) {
  for (auto it = _elems.begin(); it != _elems.end(); ++it) {
    if (it->first == k) {
      return it;
    }
  }
  return _elems.end();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator 
linear_map<K, S>::find(const K &k) const {
  for (auto it = _elems.begin(); it != _elems.end(); it++) {
    if (it->first == k) {
      return it;
    }
  }
  return _elems.end();
}
  
template<class K, class S>
size_t linear_map<K, S>::count(const K &k) const{
  return find(k) != _elems.end();
}

template<class K, class S>
bool linear_map<K, S>::operator==(const linear_map<K, S> &other) const {
  for (auto v : other) {
    if (not count(v.first) or at(v.first) != v.second) {
      return false;
    }
  }
  return true;
}

template<class K, class S>
typename linear_map<K, S>::size_type linear_map<K, S>::erase(const K& key) {
  for (auto it = _elems.begin(); it != _elems.end(); it++) {
    if (it->first == key) {
      _elems.erase(it);
      return 1;
    }
  }
}

template<class K, class S>
const S& linear_map<K, S>::at(const K& key) const {
  return find(key)->second;
}

template<class K, class S>
S& linear_map<K, S>::at(const K& key) {
  return find(key)->second;
}
  
template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::begin() {
  return _elems.begin();
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::end() {
  return _elems.end();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator linear_map<K, S>::begin() const {
  return _elems.cbegin();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator linear_map<K, S>::end() const {
  return _elems.cend();
}

template<class K, class S>
std::ostream &operator<<(std::ostream &os, const linear_map<K, S>& m) {
    os << "{ ";
    int count = m.size();
    for (auto kv : m) {
      count--;
      os << kv.first << ": " << kv.second;
      if (count) {
        os << ", ";
      }
    }
    os << " }";
    return os;
}


