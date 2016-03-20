#include "mat.h"

Mat::~Mat() {
  delete[] data;
}

bool Mat::empty() const {
  return data == nullptr;
}

unsigned char& Mat::operator() (size_t i, size_t j) {
  return data[i * cols + j];
}

const unsigned char& Mat::operator() (size_t i, size_t j) const {
  return ((const unsigned char*)data)[i * cols + j];
}