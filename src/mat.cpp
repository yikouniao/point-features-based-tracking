#include "mat.h"

bool Mat::empty() const {
  return data == nullptr;
}

unsigned char& Mat::operator() (size_t x, size_t y) {
  return data[x * cols + y];
}

const unsigned char& Mat::operator() (size_t x, size_t y) const {
  return ((const unsigned char*)data)[x * cols + y];
}