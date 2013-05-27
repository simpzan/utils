#ifndef CONST_VECTOR_H
#define CONST_VECTOR_H

#include <stdint.h>
#include <cassert>
#include <iostream>

template <typename T>
class ConstVector {
  public:
  ConstVector() : _is_mmap(false), _count(0), _elements(NULL) {} 
  ~ConstVector() {  clear();  }

  uint32_t mmap(const uint8_t *address);
  void read(std::istream &is);
  void clear();
  void display() const;

  T &operator[](uint64_t index);
  const T &operator[](uint64_t index) const;
  T *data() {  return _elements;  }
  const T *data() const {  return _elements;  }

  uint64_t count() const {  return _count;  }
  uint64_t size() const;

  private:
  bool _is_mmap;
  uint64_t _count;
  T *_elements;
};

template <typename T>
inline void ConstVector<T>::display() const {
  std::ostream &os = std::cout;
  os << "count: " << count() << std::endl;
  for (int i = 0; i < count(); ++i) {
    os << i << ":" << _elements[i] << " ";
  }
  os << std::endl;
}
template <typename T>
inline uint32_t ConstVector<T>::mmap(const uint8_t *address) {
  assert(address);
  _count = *(uint64_t *)address;
  _elements = (T *)(address + sizeof(_count));
  _is_mmap = true;
  return size();
}

template <typename T>
inline void ConstVector<T>::read(std::istream &is) {
  _is_mmap = false;
  is.read((char *)&_count, sizeof(_count));
  _elements = new T[_count];
  uint64_t size = sizeof(T) * _count;
  is.read((char *)_elements, size);
}

template <typename T>
inline void ConstVector<T>::clear() {
  if (!_is_mmap && _elements)  delete[] _elements;
  _elements = NULL;
  _count = 0;
}

template <typename T>
inline T &ConstVector<T>::operator[](uint64_t index) {
  assert(_elements);
  assert(index < _count);
  return _elements[index];
}

template <typename T>
inline const T &ConstVector<T>::operator[](uint64_t index) const {
  assert(_elements);
  assert(index < _count);
  return _elements[index];
}

template <typename T>
inline uint64_t ConstVector<T>::size() const {
  uint64_t size_elements = sizeof(T) * _count;
  return sizeof(_count) + size_elements;
}

#endif
