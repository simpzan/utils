#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <cassert>
#include <vector>
#include <iostream>
#include <iterator>

template <typename T>
class Vector {
 public:
  Vector() {}
  Vector(uint32_t count) {  _elements.resize(count);  }
  ~Vector() {}

  T &operator[](uint32_t index);
  const T &operator[](uint32_t index) const;
  T *data() {  return _elements.data();  }
  const T *data() const {  return _elements.data();  }

  uint32_t count() const {  return _elements.size();  }
  uint32_t size() const;

  void display(std::ostream &os) const;

  void insert(uint32_t pos, const T &value) {
    _elements.insert(_elements.begin() + pos, value);
  }

  void append(T element) {  _elements.push_back(element);  }
  void appendValues(const T *elements, int len);
  void read(std::istream &is);
  void write(std::ostream &os) const;
  void clear() {  _elements.clear();  }
  void erase(uint32_t pos, uint32_t count) {
    assert(pos < this->count());
    assert(pos + count <= this->count());
    _elements.erase(_elements.begin() + pos, _elements.begin() + pos + count);
  }

  static uint32_t sizeWithCount(uint32_t count);

 private:
  std::vector<T> _elements;
};

template <typename T>
void Vector<T>::appendValues(const T *elements, int len) {
  _elements.insert(_elements.end(), elements, elements + len);
}

template <typename T>
uint32_t Vector<T>::sizeWithCount(uint32_t count) {
  uint32_t size = sizeof(uint32_t) + count * sizeof(T);
  return size;
}

template <typename T>
inline T &Vector<T>::operator[](uint32_t index) {
  assert(index < count());
  return _elements[index];
}

template <typename T>
inline const T &Vector<T>::operator[](uint32_t index) const {
  assert(index < count());
  return _elements[index];
}

template <typename T>
inline void Vector<T>::write(std::ostream &os) const {
  uint32_t count = this->count();
  os.write((char *)&count, sizeof(count));
  uint32_t size = sizeof(T) * count;
  os.write((char *)_elements.data(), size);
}

template <typename T>
inline void Vector<T>::display(std::ostream &os) const {
  os << "count: " << count() << std::endl;
  for (int i = 0; i < count(); ++i) {
    os << i << ":" << _elements[i] << " ";
  }
  os << std::endl;
}

template <typename T>
inline uint32_t Vector<T>::size() const {
  uint32_t count = this->count();
  uint32_t size = sizeof(T) * count;
  return size + sizeof(count);
}

template <typename T>
inline void Vector<T>::read(std::istream &is) {
  uint32_t count = 0;
  is.read((char *)&count, sizeof(count));
  _elements.resize(count);
  uint32_t size = sizeof(T) * count;
  is.read((char *)_elements.data(), size);
}

#endif
